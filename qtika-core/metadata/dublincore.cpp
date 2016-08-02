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
#include "dublincore.h"
#include "metadata.h"


namespace qtika {

namespace metadata {


DublinCore::DublinCore()
    : NAMESPACE_URI_DC("http://purl.org/dc/elements/1.1/")
    , NAMESPACE_URI_DC_TERMS("http://purl.org/dc/terms/")
    , PREFIX_DC("dc")
    , PREFIX_DC_TERMS("dcterms")
    , FORMAT(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "format"))
    , IDENTIFIER(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "identifier"))
    , MODIFIED(Property::internalDate(PREFIX_DC_TERMS % Metadata::NAMESPACE_PREFIX_DELIMITER % "modified"))
    , CONTRIBUTOR(Property::internalTextBag(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "contributor"))
    , COVERAGE(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "coverage"))
    , CREATOR(Property::internalTextBag(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "creator"))
    , CREATED(Property::internalDate(PREFIX_DC_TERMS % Metadata::NAMESPACE_PREFIX_DELIMITER % "created"))
    , DATE(Property::internalDate(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "date"))
    , DESCRIPTION(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "description"))
    , LANGUAGE(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "language"))
    , PUBLISHER(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "publisher"))
    , RELATION(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "relation"))
    , RIGHTS(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "rights"))
    , SOURCE(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "source"))
    , SUBJECT(Property::internalTextBag(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "title"))
    , TITLE(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "format"))
    , TYPE(Property::internalText(PREFIX_DC % Metadata::NAMESPACE_PREFIX_DELIMITER % "type"))
{
}


}       // namespace metadata

}       // namespace qtika
