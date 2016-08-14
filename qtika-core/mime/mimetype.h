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

#include <QList>
#include <QSharedDataPointer>


class QUrl;


namespace qtika {

namespace mime {


class Magic;
class MediaType;
class MimeTypeData;


/**
 * Internet media type.
 */
class QTIKACORESHARED_EXPORT MimeType Q_DECL_FINAL
{
public:
    MimeType();

    /**
     * Creates a media type with the give name and containing media type
     * registry. The name is expected to be valid and normalized to lower
     * case. This constructor should only be called by
     * {@link MimeTypes#forName(String)} to keep the media type registry
     * up to date.
     *
     * @param type normalized media type name
     */
    MimeType(const MediaType &type);

    MimeType(const MimeType &);
    MimeType &operator=(const MimeType &);
    ~MimeType();

    bool isNull() const;

    /**
     * Returns the normalized media type name.
     *
     * @return media type
     */
    MediaType type() const;

    /**
     * Returns the name of this media type.
     *
     * @return media type name (lower case)
     */
    QString name() const;

    /**
     * Returns the description of this media type.
     *
     * @return media type description
     */
    QString description() const;

    /**
     * Set the description of this media type.
     *
     * @param desc media type description
     */
    void setDescription(const QString &desc);

    /**
     * Returns an acronym for this mime type.
     *
     * @return mime type acronym
     */
    QString acronym() const;

    /**
     * Set an acronym for the mime type.
     *
     * @param a
     */
    void setAcronym(const QString &a);

    /**
     * Get the UTI for this mime type.
     *
     * @see <a href="http://en.wikipedia.org/wiki/Uniform_Type_Identifier">http://en.wikipedia.org/wiki/Uniform_Type_Identifier</a>
     *
     * @return The Uniform Type Identifier
     */
    QString uniformTypeIdentifier() const;

    /**
     * Set The Uniform Type Identifier
     *
     * @param uti
     */
    void setUniformTypeIdentifier(const QString &uti);

    /**
     * Get a list of links to help document this mime type
     *
     * @return an array of links (will never be null)
     */
    QList<QUrl> links() const;

    /**
     * Add a link to this mime type
     * @param link
     */
    void addLink(const QUrl &link);

    /**
     * Add some rootXML info to this mime-type
     *
     * @param namespaceURI
     * @param localName
     */
    void addRootXML(const QString &namespaceURI, const QString &localName);

    bool matchesXML(const QString &namespaceURI, const QString &localName) const;

    bool hasRootXML() const;

    QList<Magic> magics() const;

    void addMagic(const Magic &magic);

    int minLength() const;

    bool hasMagic() const;

    bool matchesMagic(const QByteArray &d) const;

    bool matches(const QByteArray &d) const { return matchesMagic(d); }


    /**
     * Returns the name of this media type.
     *
     * @return media type name
     */
    QString toString() const;

    /**
     * Returns the preferred file extension of this type, or an empty string
     * if no extensions are known. Use the {@link #getExtensions()} method to
     * get the full list of known extensions of this type.
     *
     * @since Apache Tika 0.9
     * @return preferred file extension or empty string
     */
    QString extension() const;

    /**
     * Returns the list of all known file extensions of this media type.
     *
     * @since Apache Tika 0.10
     * @return known extensions in order of preference (best first)
     */
    QStringList extensions() const;

    /**
     * Adds a known file extension to this type.
     *
     * @param ext file extension
     */
    void addExtension(const QString &ext);

public:
    /**
     * Checks that the given string is a valid Internet media type name
     * based on rules from RFC 2054 section 5.3. For validation purposes the
     * rules can be simplified to the following:
     * <pre>
     * name := token "/" token
     * token := 1*&lt;any (US-ASCII) CHAR except SPACE, CTLs, or tspecials&gt;
     * tspecials :=  "(" / ")" / "&lt;" / "&gt;" / "@" / "," / ";" / ":" /
     *               "\" / <"> / "/" / "[" / "]" / "?" / "="
     * </pre>
     *
     * @param name name string
     * @return <code>true</code> if the string is a valid media type name,
     *         <code>false</code> otherwise
     */
    static bool isValid(const QString &name);

private:
    QSharedDataPointer<MimeTypeData> data;
};


bool operator==(const MimeType &lhs, const MimeType &rhs);
bool operator!=(const MimeType &lhs, const MimeType &rhs);
bool operator<(const MimeType &lhs, const MimeType &rhs);
uint qHash(const MimeType &m, uint seed);


}       // namespace mime

}       // namespace qtika
