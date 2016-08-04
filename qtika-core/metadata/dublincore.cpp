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


QString DublinCore::NAMESPACE_URI_DC()
{
    return QS("http://purl.org/dc/elements/1.1/");
}


QString DublinCore::NAMESPACE_URI_DC_TERMS()
{
    return QS("http://purl.org/dc/terms/");
}


QString DublinCore::PREFIX_DC()
{
    return QS("dc");
}


QString DublinCore::PREFIX_DC_TERMS()
{
    return QS("dcterms");
}


Property DublinCore::FORMAT()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "format");
    return p;
}


Property DublinCore::IDENTIFIER()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "identifier");
    return p;
}


Property DublinCore::MODIFIED()
{
    static const Property p = Property::internalDate(PREFIX_DC_TERMS() %
                              Metadata::NAMESPACE_PREFIX_DELIMITER() % "modified");
    return p;
}


Property DublinCore::CONTRIBUTOR()
{
    static const Property p = Property::internalTextBag(PREFIX_DC() %
                              Metadata::NAMESPACE_PREFIX_DELIMITER() % "contributor");
    return p;
}


Property DublinCore::COVERAGE()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "coverage");
    return p;
}


Property DublinCore::CREATOR()
{
    static const Property p = Property::internalTextBag(PREFIX_DC() %
                              Metadata::NAMESPACE_PREFIX_DELIMITER() % "creator");
    return p;
}


Property DublinCore::CREATED()
{
    static const Property p = Property::internalDate(PREFIX_DC_TERMS() %
                              Metadata::NAMESPACE_PREFIX_DELIMITER() % "created");
    return p;
}


Property DublinCore::DATE()
{
    static const Property p = Property::internalDate(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "date");
    return p;
}


Property DublinCore::DESCRIPTION()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "description");
    return p;
}


Property DublinCore::LANGUAGE()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "language");
    return p;
}


Property DublinCore::PUBLISHER()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "publisher");
    return p;
}


Property DublinCore::RELATION()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "relation");
    return p;
}


Property DublinCore::RIGHTS()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "rights");
    return p;
}


Property DublinCore::SOURCE()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "source");
    return p;
}


Property DublinCore::SUBJECT()
{
    static const Property p = Property::internalTextBag(PREFIX_DC() %
                              Metadata::NAMESPACE_PREFIX_DELIMITER() % "title");
    return p;
}


Property DublinCore::TITLE()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "format");
    return p;
}


Property DublinCore::TYPE()
{
    static const Property p = Property::internalText(PREFIX_DC() % Metadata::NAMESPACE_PREFIX_DELIMITER()
                              % "type");
    return p;
}

}       // namespace metadata

}       // namespace qtika
