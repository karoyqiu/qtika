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
#pragma once

#include "qtika-core-global.h"

#include <QMap>
#include <QSet>
#include <QSharedDataPointer>


namespace qtika {

namespace mime {


class MediaTypeData;

/**
 * Internet media type.
 */
class QTIKACORESHARED_EXPORT MediaType Q_DECL_FINAL
{
public:
    typedef QMap<QString, QString> ParameterMap;

    MediaType();
    MediaType(const QString &type, const QString subtype, const ParameterMap &params = { });
    MediaType(const QString &str, int slash);
    MediaType(const MediaType &type, const ParameterMap &params);
    MediaType(const MediaType &type, const QString &name, const QString &value);
    MediaType(const MediaType &type, const QString &charset);
    MediaType(const MediaType &);
    MediaType &operator=(const MediaType &);
    ~MediaType();

    bool isNull() const;

    /**
     * Returns the base form of the MediaType, excluding
     *  any parameters, such as "text/plain" for
     *  "text/plain; charset=utf-8"
     */
    MediaType baseType() const;

    /**
     * Return the Type of the MediaType, such as
     *  "text" for "text/plain"
     */
    QString type() const;

    /**
     * Return the Sub-Type of the MediaType,
     *  such as "plain" for "text/plain"
     */
    QString subtype() const;

    /**
     * Checks whether this media type contains parameters.
     *
     * @since Apache Tika 0.8
     * @return <code>true</code> if this type has one or more parameters,
     *         <code>false</code> otherwise
     */
    bool hasParameters() const;

    /**
     * Returns an immutable sorted map of the parameters of this media type.
     * The parameter names are guaranteed to be trimmed and in lower case.
     *
     * @return sorted map of parameters
     */
    ParameterMap parameters() const;

    QString toString() const;

public:
    /**
     * Parses the given string to a media type. The string is expected
     * to be of the form "type/subtype(; parameter=...)*" as defined in
     * RFC 2045, though we also handle "charset=xxx; type/subtype" for
     * broken web servers.
     *
     * @param string media type string to be parsed
     * @return parsed media type, or <code>null</code> if parsing fails
     */
    static MediaType parse(const QString &s);

    /**
     * Convenience method that parses the given media type strings and
     * returns an unmodifiable set that contains all the parsed types.
     *
     * @since Apache Tika 1.2
     * @param types media type strings
     * @return unmodifiable set of the parsed types
     */
    static QSet<MediaType> set(const QStringList &types);

public:
    static MediaType application(const QString &type);
    static MediaType audio(const QString &type);
    static MediaType image(const QString &type);
    static MediaType text(const QString &type);
    static MediaType video(const QString &type);

    static const MediaType OCTET_STREAM;
    static const MediaType TEXT_PLAIN;
    static const MediaType TEXT_HTML;
    static const MediaType APPLICATION_XML;
    static const MediaType APPLICATION_ZIP;

private:
    QSharedDataPointer<MediaTypeData> data;
};


bool operator==(const MediaType &lhs, const MediaType &rhs);
bool operator<(const MediaType &lhs, const MediaType &rhs);

uint qHash(const MediaType &lhs, uint seed);


}       // namespace mime

}       // namespace qtika
