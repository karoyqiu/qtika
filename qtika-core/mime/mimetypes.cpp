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
#include "mimetypes.h"

#include "mediatyperegistry.h"
#include "mimetype.h"
#include "patterns.h"


namespace qtika {

namespace mime {


class MimeTypesData : public QSharedData
{
public:
    /**
     * Name of the {@link #rootMimeType root} type, application/octet-stream.
     */
    static const QString OCTET_STREAM;

    /**
     * Name of the {@link #textMimeType text} type, text/plain.
     */
    static const QString PLAIN_TEXT;

    /**
     * Name of the {@link #xmlMimeType xml} type, application/xml.
     */
    static const QString XML;

    /**
     * Root type, application/octet-stream.
     */
    MimeType rootMimeType;
    QList<MimeType> rootMimeTypes;

    /**
     * Text type, text/plain.
     */
    MimeType textMimeType;

    /*
     * xml type, application/xml
     */
    MimeType xmlMimeType;

    /**
     * Registered media types and their aliases.
     */
    MediaTypeRegistry registry;

    /** All the registered MimeTypes indexed on their canonical names */
    QHash<MediaType, MimeType> types;

    /** The patterns matcher */
    Patterns patterns;
};


const QString MimeTypesData::OCTET_STREAM("application/octet-stream");
const QString MimeTypesData::PLAIN_TEXT("text/plain");
const QString MimeTypesData::XML("application/xml");


MimeTypes::MimeTypes()
    : data(new MimeTypesData)
{
}


MimeTypes::MimeTypes(const MimeTypes &rhs)
    : data(rhs.data)
{
}


MimeTypes &MimeTypes::operator=(const MimeTypes &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


MimeTypes::~MimeTypes()
{
}


}       // namespace mime

}       // namespace qtika
