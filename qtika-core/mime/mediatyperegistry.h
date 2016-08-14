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

#include <QSet>
#include <QSharedDataPointer>


namespace qtika {

namespace mime {


class MediaType;
class MediaTypeRegistryData;


/**
 * Registry of known Internet media types.
 */
class QTIKACORESHARED_EXPORT MediaTypeRegistry
{
public:
    MediaTypeRegistry();
    MediaTypeRegistry(const MediaTypeRegistry &);
    MediaTypeRegistry &operator=(const MediaTypeRegistry &);
    ~MediaTypeRegistry();

    /**
     * Returns the built-in media type registry included in Tika.
     *
     * @since Apache Tika 0.8
     * @return default media type registry
     */
    static MediaTypeRegistry *defaultRegistry();

    /**
     * Returns the set of all known canonical media types. Type aliases are
     * not included in the returned set.
     *
     * @since Apache Tika 0.8
     * @return canonical media types
     */
    QSet<MediaType> types() const;

    /**
     * Returns the set of known aliases of the given canonical media type.
     *
     * @since Apache Tika 0.8
     * @param type canonical media type
     * @return known aliases
     */
    QSet<MediaType> aliases(const MediaType &type) const;

    /**
     * Returns the set of known children of the given canonical media type
     *
     * @since Apache Tika 1.8
     * @param type canonical media type
     * @return known children
     */
    QSet<MediaType> childTypes(const MediaType &type) const;

    void addType(const MediaType &type);

    void addAlias(const MediaType &type, const MediaType &alias);

    void addSuperType(const MediaType &type, const MediaType &super);

    MediaType normalize(const MediaType &type) const;

    /**
     * Returns the supertype of the given type. If the media type database
     * has an explicit inheritance rule for the type, then that is used.
     * Next, if the given type has any parameters, then the respective base
     * type (parameter-less) is returned. Otherwise built-in heuristics like
     * text/... -&gt; text/plain and .../...+xml -&gt; application/xml are used.
     * Finally application/octet-stream is returned for all types for which no other
     * supertype is known, and the return value for application/octet-stream
     * is <code>null</code>.
     *
     * @since Apache Tika 0.8
     * @param type media type
     * @return supertype, or <code>null</code> for application/octet-stream
     */
    MediaType superType(const MediaType &type) const;

    /**
     * Checks whether the given media type a is a specialization of a more
     * generic type b. Both types should be already normalised.
     *
     * @since Apache Tika 0.8
     * @param a media type, normalised
     * @param b suspected supertype, normalised
     * @return <code>true</code> if b is a supertype of a,
     *         <code>false</code> otherwise
     */
    bool isSpecializationOf(const MediaType &a, const MediaType &b) const;

    /**
     * Checks whether the given media type equals the given base type or
     * is a specialization of it. Both types should be already normalised.
     *
     * @since Apache Tika 1.2
     * @param a media type, normalised
     * @param b base type, normalised
     * @return <code>true</code> if b equals a or is a specialization of it,
     *         <code>false</code> otherwise
     */
    bool isInstanceOf(const MediaType &a, const MediaType &b) const;

    /**
     * Parses and normalises the given media type string and checks whether
     * the result equals the given base type or is a specialization of it.
     * The given base type should already be normalised.
     *
     * @since Apache Tika 1.2
     * @param a media type
     * @param b base type, normalised
     * @return <code>true</code> if b equals a or is a specialization of it,
     *         <code>false</code> otherwise
     */
    bool isInstanceOf(const QString &a, const MediaType &b) const;

private:
    QSharedDataPointer<MediaTypeRegistryData> data;
};


}       // namespace mime

}       // namespace qtika
