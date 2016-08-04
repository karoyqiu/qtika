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
#include "httpheaders.h"


namespace qtika {

namespace metadata {


QString HttpHeaders::CONTENT_ENCODING()
{
    return QS("Content-Encoding");
}


QString HttpHeaders::CONTENT_LANGUAGE()
{
    return QS("Content-Language");
}


QString HttpHeaders::CONTENT_LENGTH()
{
    return QS("Content-Length");
}


QString HttpHeaders::CONTENT_LOCATION()
{
    return QS("Content-Location");
}


QString HttpHeaders::CONTENT_DISPOSITION()
{
    return QS("Content-Disposition");
}


QString HttpHeaders::CONTENT_MD5()
{
    return QS("Content-MD5");
}


QString HttpHeaders::CONTENT_TYPE()
{
    return QS("Content-Type");
}


Property HttpHeaders::LAST_MODIFIED()
{
    static const Property p = Property::internalDate(QS("Last-Modified"));
    return p;
}


QString HttpHeaders::LOCATION()
{
    return QS("Location");
}


}       // namespace metadata

}       // namespace qtika
