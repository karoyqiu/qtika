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
#include "mime/mediatype.h"


namespace qtika {

namespace detect {


using mime::MediaType;


class MagicDetectorData : public QSharedData
{
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
