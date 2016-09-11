/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "stable.h"
#include "magicdetector.h"

#include <QRegularExpression>
#include <qiodevicetransactionrollback.h>

#include "mime/mediatype.h"


namespace qtika {

namespace detect {


using mime::MediaType;


class MagicDetectorData : public QSharedData
{
public:
    static QByteArray decodeValue(const QString &value, const QString &type);
    static QByteArray decodeString(const QString &value, const QString &type);

public:
    /**
     * The matching media type. Returned by the
     * {@link #detect(InputStream, Metadata)} method if a match is found.
     */
    MediaType type;

    /**
     * Length of the comparison window.
     */
    int length;

    /**
     * The magic match pattern. If this byte pattern is equal to the
     * possibly bit-masked bytes from the input stream, then the type
     * detection succeeds and the configured {@link #type} is returned.
     */
    QByteArray pattern;

    /**
     * Length of the pattern, which in the case of regular expressions will
     * not be the same as the comparison window length.
     */
    int patternLength;

    /**
     * True if pattern is a regular expression, false otherwise.
     */
    bool isRegex;

    /**
     * True if we're doing a case-insensitive string match, false otherwise.
     */
    bool isStringIgnoreCase;

    /**
     * Bit mask that is applied to the source bytes before pattern matching.
     */
    QByteArray mask;

    /**
     * First offset (inclusive) of the comparison window within the
     * document input stream. Greater than or equal to zero.
     */
    int offsetRangeBegin;

    /**
     * Last offset (inclusive) of the comparison window within the document
     * input stream. Greater than or equal to the
     * {@link #offsetRangeBegin first offset}.
     * <p>
     * Note that this is <em>not</em> the offset of the last byte read from
     * the document stream. Instead, the last window of bytes to be compared
     * starts at this offset.
     */
    int offsetRangeEnd;
};


QByteArray MagicDetectorData::decodeValue(const QString &value, const QString &type)
{
    // Preliminary check
    if (value.isEmpty() || type.isEmpty())
    {
        return QByteArray();
    }

    QByteArray decoded;
    QString tmp;
    int radix = 8;

    // hex
    if (value.startsWith(QLatin1String("0x")))
    {
        tmp = value.mid(2);
        radix = 16;
    }
    else
    {
        tmp = value;
    }

    if (type == QLatin1String("string") || type == QLatin1String("regex")
            || type == QLatin1String("unicodeLE") || type == QLatin1String("unicodeBE"))
    {
        decoded = decodeString(value, type);
    }
    else if (type == QLatin1String("stringignorecase"))
    {
        decoded = decodeString(value.toLower(), type);
    }
    else if (type == QLatin1String("byte"))
    {
        decoded = tmp.toUtf8();
    }
    else if (type == QLatin1String("host16") || type == QLatin1String("little16"))
    {
        ushort i = tmp.toUShort(Q_NULLPTR, radix);
        decoded.resize(sizeof(i));
        memcpy(decoded.data(), &i, sizeof(i));
    }
    else if (type == QLatin1String("big16"))
    {
        ushort i = tmp.toUShort(Q_NULLPTR, radix);
#if defined(Q_CC_MSVC)
        i = _byteswap_ushort(i);
#else
#error "Unsupported compiler."
#endif
        decoded.resize(sizeof(i));
        memcpy(decoded.data(), &i, sizeof(i));
    }
    else if (type == QLatin1String("host32") || type == QLatin1String("little32"))
    {
        static_assert(sizeof(uint) == 4, "The size of uint is not 4.");
        uint i = tmp.toUInt(Q_NULLPTR, radix);
        decoded.resize(sizeof(i));
        memcpy(decoded.data(), &i, sizeof(i));
    }
    else if (type == QLatin1String("big32"))
    {
        uint i = tmp.toUInt(Q_NULLPTR, radix);
#if defined(Q_CC_MSVC)
        i = _byteswap_ulong(i);
#else
#endif
        decoded.resize(sizeof(i));
        memcpy(decoded.data(), &i, sizeof(i));
    }

    return decoded;
}


QByteArray MagicDetectorData::decodeString(const QString &value, const QString &type)
{
    if (value.startsWith(QLatin1String("0x")))
    {
        return QByteArray::fromHex(value.mid(2).toLatin1());
    }

    QString decoded;

    for (int i = 0; i < value.length(); i++)
    {
        if (value.at(i) == QLatin1Char('\\'))
        {
            if (value.at(i + 1) == QLatin1Char('\\'))
            {
                decoded.append(QLatin1Char('\\'));
                i++;
            }
            else if (value.at(i + 1) == QLatin1Char('x'))
            {
                decoded.append(QChar(value.mid(i + 2, 2).toInt(Q_NULLPTR, 16)));
                i += 3;
            }
            else if (value.at(i + 1) == QLatin1Char('\r'))
            {
                decoded.append(QLatin1Char('\r'));
                i++;
            }
            else if (value.at(i + 1) == QLatin1Char('\n'))
            {
                decoded.append(QLatin1Char('\n'));
                i++;
            }
            else
            {
                int j = i + 1;

                while (j < i + 4 && j < value.length() && value.at(j).isDigit())
                {
                    j++;
                }

                decoded.append(QChar(value.mid(i + 1, j - (i + 1)).toShort(Q_NULLPTR, 8)));
                i = j - 1;
            }
        }
        else
        {
            decoded.append(value.at(i));
        }
    }

    // Now turn the chars into bytes
    const QChar *chars = decoded.unicode();
    QByteArray bytes;

    if (type == QLatin1String("unicodeLE"))
    {
        bytes.resize(decoded.length() * 2);

        for (int i = 0; i < decoded.length(); i++)
        {
            bytes[i * 2] = static_cast<char>(chars->unicode() & 0xff);
            bytes[i * 2 + 1] = static_cast<char>(chars->unicode() >> 8);
            chars++;
        }
    }
    else if (type == QLatin1String("unicodeBE"))
    {
        bytes.resize(decoded.length() * 2);

        for (int i = 0; i < decoded.length(); i++)
        {
            bytes[i * 2] = static_cast<char>(chars->unicode() >> 8);
            bytes[i * 2 + 1] = static_cast<char>(chars->unicode() & 0xff);
            chars++;
        }
    }
    else
    {
        // Copy with truncation
        bytes.resize(decoded.length());

        for (int i = 0; i < decoded.length(); i++)
        {
            bytes[i] = static_cast<char>(chars->unicode());
            chars++;
        }
    }

    return bytes;
}


MagicDetector::MagicDetector(const MediaType &type,
                             const QByteArray &pattern, const QByteArray &mask,
                             bool isRegex, bool isStringIgnoreCase,
                             int offsetRangeBegin, int offsetRangeEnd)
    : data(new MagicDetectorData)
{
    if (type.isNull())
    {
        throw std::invalid_argument("Matching media type is null.");
    }

    if (pattern.isNull())
    {
        throw std::invalid_argument("Magic match pattern is null.");
    }

    if (offsetRangeBegin < 0 || offsetRangeEnd < offsetRangeBegin)
    {
        QString message = QS("Invalid offset range [%1, %2].").arg(offsetRangeBegin).arg(offsetRangeEnd);
        throw std::invalid_argument(message.toStdString());
    }

    data->type = type;
    data->isRegex = isRegex;
    data->isStringIgnoreCase = isStringIgnoreCase;
    data->patternLength = qMax(pattern.length(), mask.length());

    if (isRegex)
    {
        // 8K buffer should cope with most regex patterns
        data->length = 8 * 1024;
    }
    else
    {
        data->length = data->patternLength;
    }

    data->mask.resize(data->patternLength);
    data->pattern.resize(data->patternLength);

    for (int i = 0; i < data->patternLength; i++)
    {
        if (i < mask.length())
        {
            data->mask[i] = mask.at(i);
        }
        else
        {
            data->mask[i] = -1;
        }

        if (i < data->patternLength)
        {
            data->pattern[i] = static_cast<char>(pattern.at(i) & data->mask.at(i));
        }
    }

    data->offsetRangeBegin = offsetRangeBegin;
    data->offsetRangeEnd = offsetRangeEnd;
}


MagicDetector::MagicDetector(const MediaType &type,
                             const QByteArray &pattern, const QByteArray &mask, bool isRegex,
                             int offsetRangeBegin, int offsetRangeEnd)
    : MagicDetector(type, pattern, mask, isRegex, false, offsetRangeBegin, offsetRangeEnd)
{
}


MagicDetector::MagicDetector(const mime::MediaType &type,
                             const QByteArray &pattern, const QByteArray &mask,
                             int offsetRangeBegin, int offsetRangeEnd)
    : MagicDetector(type, pattern, mask, false, offsetRangeBegin, offsetRangeEnd)
{
}


MagicDetector::MagicDetector(const MediaType &type, const QByteArray &pattern, int offset)
    : MagicDetector(type, pattern, QByteArray(), false, offset, offset)
{
}


MagicDetector::MagicDetector(const MagicDetector &rhs)
    : data(rhs.data)
{
}


MagicDetector &MagicDetector::operator=(const MagicDetector &rhs)
{
    if (this != &rhs)
    {
        data.operator = (rhs.data);
    }

    return *this;
}


MagicDetector::~MagicDetector()
{
}


MediaType MagicDetector::detect(QIODevice *input, const Metadata &meta) const
{
    Q_UNUSED(meta)

    if (input == Q_NULLPTR)
    {
        return MediaType::OCTET_STREAM();
    }

    MediaType type = MediaType::OCTET_STREAM();
    internal::QIODeviceTransactionRollback rollback(input);

    int offset = 0;

    // Skip bytes at the beginning, using skip() or read()
    while (offset < data->offsetRangeBegin)
    {
        QByteArray bytes = input->read(data->offsetRangeBegin - offset);

        if (!bytes.isEmpty())
        {
            offset += bytes.length();
        }
        else if (!input->atEnd())
        {
            offset += 1;
        }
        else
        {
            return type;
        }
    }

    // Fill in the comparison window
    QByteArray buffer(data->length + (data->offsetRangeEnd - data->offsetRangeBegin), 0);
    qint64 n = input->read(buffer.data(), buffer.length());

    if (n > 0)
    {
        offset += n;
    }

    while (n > 0 && offset < data->offsetRangeEnd + data->length)
    {
        int bufferOffset = offset - data->offsetRangeBegin;
        n = input->read(buffer.data() + bufferOffset, buffer.length() - bufferOffset);

        if (n > 0)
        {
            offset += n;
        }
    }

    if (data->isRegex)
    {
        QRegularExpression::PatternOptions flags = 0;

        if (data->isStringIgnoreCase)
        {
            flags = QRegularExpression::CaseInsensitiveOption;
        }

        QRegularExpression regex(QString::fromUtf8(data->pattern), flags);
        QString result = QString::fromLatin1(buffer);

        // Loop until we've covered the entire offset range
        for (int i = 0; i <= data->offsetRangeEnd - data->offsetRangeBegin; i++)
        {
            QStringRef region = result.midRef(i, data->length);
            auto m = regex.match(QString::fromLatin1(buffer));

            if (m.hasMatch())
            {
                type = data->type;
                break;
            }
        }
    }
    else if (offset >= data->offsetRangeBegin + data->length)
    {
        // Loop until we've covered the entire offset range
        for (int i = 0; i <= data->offsetRangeEnd - data->offsetRangeBegin; i++)
        {
            bool match = true;
            int masked = 0;

            for (int j = 0; match && j < data->length; j++)
            {
                masked = (buffer.at(i + j) & data->mask.at(j));

                if (data->isStringIgnoreCase)
                {
                    masked = tolower(masked);
                }

                match = (masked == data->pattern.at(j));
            }

            if (match)
            {
                type = data->type;
                break;
            }
        }
    }

    return type;
}


MagicDetector MagicDetector::parse(const Detector::MediaType &mediaType, const QString &type,
                                   const QString &offset, const QString &value, const QString &mask)
{
    int start = 0;
    int end = 0;

    if (!offset.isEmpty())
    {
        int colon = offset.indexOf(QLatin1Char(':'));

        if (colon == -1)
        {
            start = offset.toInt();
            end = start;
        }
        else
        {
            start = offset.left(colon).toInt();
            end = offset.mid(colon + 1).toInt();
        }
    }

    QByteArray patternBytes = MagicDetectorData::decodeValue(value, type);
    QByteArray maskBytes;

    if (!mask.isEmpty())
    {
        maskBytes = MagicDetectorData::decodeValue(mask, type);
    }

    return MagicDetector(mediaType, patternBytes, maskBytes, type == QLatin1String("regex"),
                         type == QLatin1String("stringignorecase"), start, end);
}


int MagicDetector::length() const
{
    return data->patternLength;
}


QString MagicDetector::toString() const
{
    // Needs to be unique, as these get compared.
    return QS("Magic Detection for %1 looking for %2 bytes = %3 mask = %4")
            .arg(data->type.toString()).arg(data->pattern.length())
            .arg(QString::fromUtf8(data->pattern.toHex()))
            .arg(QString::fromUtf8(data->mask.toHex()));
}

}       // namespace detect

}       // namespace qtika
