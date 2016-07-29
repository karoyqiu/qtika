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
#include "property.h"


namespace qtika {

namespace metadata {


/**
 * A collection of HTTP header names.
 *
 * @see <a href="http://rfc-ref.org/RFC-TEXTS/2616/">Hypertext Transfer Protocol --
 *      HTTP/1.1 (RFC 2616)</a>
 */
class QTIKACORESHARED_EXPORT HttpHeaders
{
protected:
    HttpHeaders();

public:
    virtual ~HttpHeaders() Q_DECL_EQ_DEFAULT;

public:
    const QString CONTENT_ENCODING;
    const QString CONTENT_LANGUAGE;
    const QString CONTENT_LENGTH;
    const QString CONTENT_LOCATION;
    const QString CONTENT_DISPOSITION;
    const QString CONTENT_MD5;
    const QString CONTENT_TYPE;
    const Property LAST_MODIFIED;
    const QString LOCATION;
};


}       // namespace metadata

}       // namespace qtika
