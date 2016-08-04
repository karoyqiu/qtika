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
#include "mediatype.h"

#include <QHash>
#include <QLocale>
#include <QMutex>
#include <QRegularExpression>
#include <QStringBuilder>


namespace qtika {

namespace mime {


class MediaTypeData : public QSharedData
{
public:
    MediaTypeData() : slash(-1), semicolon(-1) { }

    static bool isSimpleName(const QString &name);

    /**
     * Fuzzy unquoting mechanism that works also with somewhat malformed
     * quotes.
     *
     * @param s string to unquote
     * @return unquoted string
     */
    static QString unquote(const QString &s);

    template<typename K, typename V>
    static QMap<K, V> merge(const QMap<K, V> &a, const QMap<K, V> &b);

    static MediaType::ParameterMap parseParameters(const QString &s);

public:
    /**
     * See http://www.ietf.org/rfc/rfc2045.txt for valid mime-type characters.
     */
    static const QString VALID_CHARS;
    static const QRegularExpression SPECIAL;
    static const QRegularExpression SPECIAL_OR_WHITESPACE;
    static const QRegularExpression TYPE_PATTERN;
    // TIKA-350: handle charset as first element in content-type
    static const QRegularExpression CHARSET_FIRST_PATTERN;

    /**
     * Set of basic types with normalized "type/subtype" names.
     * Used to optimize type lookup and to avoid having too many
     * {@link MediaType} instances in memory.
     */
    static QHash<QString, MediaType> SIMPLE_TYPES;

    /**
     * Canonical string representation of this media type.
     */
    QString string;

    /**
     * Location of the "/" character separating the type and the subtype
     * tokens in {@link #string}.
     */
    int slash;

    /**
     * Location of the first ";" character separating the type part of
     * {@link #string} from possible parameters. Length of {@link #string}
     * in case there are no parameters.
     */
    int semicolon;

    /**
     * Immutable sorted map of media type parameters.
     */
    MediaType::ParameterMap parameters;
};


const QString MediaTypeData::VALID_CHARS(QS(R"_(([^\(\)<>@,;:\\"\/\[\]\?=\s]+))_"));
const QRegularExpression MediaTypeData::SPECIAL(QS("([\\(\\)<>@,;:\\\\\"/\\[\\]\\?=])"));
const QRegularExpression MediaTypeData::SPECIAL_OR_WHITESPACE(QS("[\\(\\)<>@,;:\\\\\"/\\[\\]\\?=\\s]"));
const QRegularExpression MediaTypeData::TYPE_PATTERN("\\s*" % VALID_CHARS % "\\s*/\\s*" % VALID_CHARS % "\\s*($|;.*)");
const QRegularExpression MediaTypeData::CHARSET_FIRST_PATTERN("\\s*(charset\\s*=\\s*[^;\\s]+)\\s*;\\s*" % VALID_CHARS % "\\s*/\\s*" % VALID_CHARS % "\\s*");
QHash<QString, MediaType> MediaTypeData::SIMPLE_TYPES;


bool MediaTypeData::isSimpleName(const QString &name)
{
    for (QChar c : name)
    {
        if (c != '-' && c != '+' && c != '.' && c != '_'
            && !(c >= '0' && c <= '9')
            && !(c >= 'a' && c <= 'z'))
        {
            return false;
        }
    }

    return !name.isEmpty();
}


QString MediaTypeData::unquote(const QString &str)
{
    QString s = str;

    while (s.startsWith('"') || s.startsWith('\''))
    {
        s.remove(0, 1);
    }

    while (s.endsWith('"') || s.endsWith('\''))
    {
        s.chop(1);
    }

    return s;
}


MediaType::ParameterMap MediaTypeData::parseParameters(const QString &str)
{
    MediaType::ParameterMap params;

    if (str.isEmpty())
    {
        return params;
    }

    // Extracts k1=v1, k2=v2 from mime/type; k1=v1; k2=v2
    // Note - this logic isn't fully RFC2045 compliant yet, as it
    //  doesn't fully handle quoted keys or values (eg containing ; or =)
    QString s = str;

    while (!s.isEmpty())
    {
        QString key = s;
        QString value;

        int semicolon = s.indexOf(';');

        if (semicolon != -1)
        {
            key = s.left(semicolon);
            s = s.mid(semicolon + 1);
        }
        else
        {
            s.clear();
        }

        int equals = key.indexOf('=');

        if (equals != -1)
        {
            value = key.mid(equals + 1);
            key = key.left(equals);
        }

        key = key.trimmed();

        if (!key.isEmpty())
        {
            params.insert(key, unquote(value.trimmed()));
        }
    }

    return params;
}


template<typename K, typename V>
QMap<K, V> MediaTypeData::merge(const QMap<K, V> &a, const QMap<K, V> &b)
{
    QMap<K, V> result = a;

    for (auto iter = b.begin(); iter != b.end(); ++iter)
    {
        result.insert(iter.key(), iter.value());
    }

    return result;
}


MediaType::MediaType()
    : data(new MediaTypeData)
{
}


MediaType::MediaType(const QString &type, const QString subtype, const MediaType::ParameterMap &params)
    : MediaType()
{
    static const QLocale english(QLocale::English);

    QString t = english.toLower(type.trimmed());
    QString sub = english.toLower(subtype.trimmed());

    data->slash = t.length();
    data->semicolon = data->slash + 1 + sub.length();
    data->string = t % "/" % sub;

    for (auto iter = params.begin(); iter != params.end(); ++iter)
    {
        QString key = english.toLower(iter.key().trimmed());
        data->parameters.insert(key, iter.value());
    }

    for (auto iter = data->parameters.begin(); iter != data->parameters.end(); ++iter)
    {
        data->string.append("; " % iter.key() % "=");

        auto match = MediaTypeData::SPECIAL_OR_WHITESPACE.match(iter.value());

        if (match.hasMatch())
        {
            QString value = iter.value();
            value.replace(MediaTypeData::SPECIAL, QS("\\\\1"));
            data->string.append("\"" % value % "\"");
        }
        else
        {
            data->string.append(iter.value());
        }
    }
}


MediaType::MediaType(const QString &str, int slash)
    : MediaType()
{
    Q_ASSERT(slash != -1);
    Q_ASSERT(str.at(slash) == '/');
    Q_ASSERT(MediaTypeData::isSimpleName(str.mid(0, slash)));
    Q_ASSERT(MediaTypeData::isSimpleName(str.mid(slash + 1)));

    data->string = str;
    data->slash = slash;
    data->semicolon = str.length();
}


MediaType::MediaType(const MediaType &type, const MediaType::ParameterMap &params)
    : MediaType(type.type(), type.subtype(), MediaTypeData::merge(type.parameters(), params))
{
}


MediaType::MediaType(const MediaType &type, const QString &name, const QString &value)
    : MediaType(type, { { name, value } })
{
}


MediaType::MediaType(const MediaType &type, const QString &charset)
    : MediaType(type, QS("charset"), charset)
{
}


MediaType::MediaType(const MediaType &rhs)
    : data(rhs.data)
{
}


MediaType &MediaType::operator=(const MediaType &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


MediaType::~MediaType()
{
}


bool MediaType::isNull() const
{
    return data->string.isEmpty();
}


MediaType MediaType::baseType() const
{
    if (data->parameters.isEmpty())
    {
        return *this;
    }

    return parse(data->string.left(data->semicolon));
}


QString MediaType::type() const
{
    return data->string.left(data->slash);
}


QString MediaType::subtype() const
{
    return data->string.mid(data->slash + 1, data->semicolon);
}


bool MediaType::hasParameters() const
{
    return !data->parameters.isEmpty();
}


MediaType::ParameterMap MediaType::parameters() const
{
    return data->parameters;
}


QString MediaType::toString() const
{
    return data->string;
}


MediaType MediaType::parse(const QString &s)
{
    MediaType type;

    if (s.isEmpty())
    {
        return type;
    }

    static QMutex mutex;
    {
        QMutexLocker locker(&mutex);
        type = MediaTypeData::SIMPLE_TYPES.value(s);

        if (type.isNull())
        {
            int slash = s.indexOf('/');

            if (slash == -1)
            {
                return type;
            }

            if (MediaTypeData::SIMPLE_TYPES.size() < 10000
                    && MediaTypeData::isSimpleName(s.mid(0, slash))
                    && MediaTypeData::isSimpleName(s.mid(slash + 1)))
            {
                type = MediaType(s, slash);
                MediaTypeData::SIMPLE_TYPES.insert(s, type);
            }
        }

        if (!type.isNull())
        {
            return type;
        }
    }

    auto match = MediaTypeData::TYPE_PATTERN.match(s);

    if (match.hasMatch())
    {
        type = MediaType(match.captured(1), match.captured(2),
                         MediaTypeData::parseParameters(match.captured(3)));
        return type;
    }

    match = MediaTypeData::CHARSET_FIRST_PATTERN.match(s);

    if (match.hasMatch())
    {
        type = MediaType(match.captured(2), match.captured(3),
                         MediaTypeData::parseParameters(match.captured(1)));
    }

    return type;
}


QSet<MediaType> MediaType::set(const QStringList &types)
{
    QSet<MediaType> result;

    for (const QString &s : types)
    {
        MediaType type = parse(s);

        if (!type.isNull())
        {
            result.insert(type);
        }
    }

    return result;
}


MediaType MediaType::application(const QString &type)
{
    return parse("application/" % type);
}


MediaType MediaType::audio(const QString &type)
{
    return parse("audio/" % type);
}


MediaType MediaType::image(const QString &type)
{
    return parse("image/" % type);
}


MediaType MediaType::text(const QString &type)
{
    return parse("text/" % type);
}


MediaType MediaType::video(const QString &type)
{
    return parse("video/" % type);
}


MediaType MediaType::OCTET_STREAM()
{
    static const MediaType type = MediaType::parse(QS("application/octet-stream"));
    return type;
}


MediaType MediaType::TEXT_PLAIN()
{
    static const MediaType type = MediaType::parse(QS("text/plain"));
    return type;
}


MediaType MediaType::TEXT_HTML()
{
    static const MediaType type = MediaType::parse(QS("text/html"));
    return type;
}


MediaType MediaType::APPLICATION_XML()
{
    static const MediaType type = MediaType::parse(QS("application/xml"));
    return type;
}


MediaType MediaType::APPLICATION_ZIP()
{
    static const MediaType type = MediaType::parse(QS("application/zip"));
    return type;
}


bool operator==(const MediaType &lhs, const MediaType &rhs)
{
    return lhs.toString() == rhs.toString();
}


bool operator<(const MediaType &lhs, const MediaType &rhs)
{
    return lhs.toString() < rhs.toString();
}


uint qHash(const MediaType &lhs, uint seed)
{
    return qHash(lhs.toString(), seed);
}

}       // namespace mime

}       // namespace qtika
