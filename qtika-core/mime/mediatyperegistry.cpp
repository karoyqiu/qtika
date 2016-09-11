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
#include "mediatyperegistry.h"

#include "mediatype.h"


namespace qtika {

namespace mime {


class MediaTypeRegistryData : public QSharedData
{
public:
    /**
     * Registry of known media types, including type aliases. A canonical
     * media type is handled as an identity mapping, while an alias is stored
     * as a mapping from the alias to the corresponding canonical type.
     */
    QHash<MediaType, MediaType> registry;

    /**
     * Known type inheritance relationships. The mapping is from a media type
     * to the closest supertype.
     */
    QHash<MediaType, MediaType> inheritance;
};


MediaTypeRegistry::MediaTypeRegistry()
    : data(new MediaTypeRegistryData)
{
}


MediaTypeRegistry::MediaTypeRegistry(const MediaTypeRegistry &rhs)
    : data(rhs.data)
{
}


MediaTypeRegistry &MediaTypeRegistry::operator=(const MediaTypeRegistry &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


MediaTypeRegistry::~MediaTypeRegistry()
{
}


MediaTypeRegistry *MediaTypeRegistry::defaultRegistry()
{
    // UNDONE
    return Q_NULLPTR;
}


QSet<MediaType> MediaTypeRegistry::types() const
{
    return data->registry.values().toSet();
}


QSet<MediaType> MediaTypeRegistry::aliases(const MediaType &type) const
{
    QSet<MediaType> a;

    for (auto iter = data->registry.begin(); iter != data->registry.end(); ++iter)
    {
        if (iter.value() == type && iter.key() != type)
        {
            a.insert(iter.key());
        }
    }

    return a;
}


QSet<MediaType> MediaTypeRegistry::childTypes(const MediaType &type) const
{
    QSet<MediaType> a;

    for (auto iter = data->inheritance.begin(); iter != data->inheritance.end(); ++iter)
    {
        if (iter.value() == type)
        {
            a.insert(iter.key());
        }
    }

    return a;
}


void MediaTypeRegistry::addType(const MediaType &type)
{
    data->registry.insert(type, type);
}


void MediaTypeRegistry::addAlias(const MediaType &type, const MediaType &alias)
{
    data->registry.insert(alias, type);
}


void MediaTypeRegistry::addSuperType(const MediaType &type, const MediaType &super)
{
    data->inheritance.insert(type, super);
}


MediaType MediaTypeRegistry::normalize(const MediaType &type) const
{
    if (type.isNull())
    {
        return type;
    }

    MediaType canonical = data->registry.value(type.baseType());

    if (canonical.isNull())
    {
        return type;
    }

    if (type.hasParameters())
    {
        return MediaType(canonical, type.parameters());
    }

    return canonical;
}


MediaType MediaTypeRegistry::superType(const MediaType &type) const
{
    if (type.isNull())
    {
        return type;
    }

    if (data->inheritance.contains(type))
    {
        return data->inheritance.value(type);
    }

    if (type.hasParameters())
    {
        return type.baseType();
    }

    if (type.subtype().endsWith(QS("+xml")))
    {
        return MediaType::APPLICATION_XML();
    }

    if (type.subtype().endsWith(QS("+zip")))
    {
        return MediaType::APPLICATION_ZIP();
    }

    if (type.type() == QLatin1String("text") && type != MediaType::TEXT_PLAIN())
    {
        return MediaType::TEXT_PLAIN();
    }

    if (type.type().contains(QS("empty")) && type != MediaType::EMPTY())
    {
        return MediaType::EMPTY();
    }

    if (type != MediaType::OCTET_STREAM())
    {
        return MediaType::OCTET_STREAM();
    }

    return MediaType();
}


bool MediaTypeRegistry::isSpecializationOf(const MediaType &a, const MediaType &b) const
{
    return isInstanceOf(superType(a), b);
}


bool MediaTypeRegistry::isInstanceOf(const MediaType &a, const MediaType &b) const
{
    return !a.isNull() && (a == b || isSpecializationOf(a, b));
}


bool MediaTypeRegistry::isInstanceOf(const QString &a, const MediaType &b) const
{
    return isInstanceOf(normalize(MediaType::parse(a)), b);
}


}       // namespace mime

}       // namespace qtika
