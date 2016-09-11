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
#include "mimetypesreadermetkeys.h"


namespace qtika {

namespace mime {


QString MimeTypesReaderMetKeys::MIME_INFO()
{
    return QS("mime-info");
}


QString MimeTypesReaderMetKeys::MIME_TYPE_TAG()
{
    return QS("mime-type");
}


QString MimeTypesReaderMetKeys::MIME_TYPE_TYPE_ATTR()
{
    return QS("type");
}


QString MimeTypesReaderMetKeys::ACRONYM_TAG()
{
    return QS("acronym");
}


QString MimeTypesReaderMetKeys::COMMENT_TAG()
{
    return QS("_comment");
}


QString MimeTypesReaderMetKeys::GLOB_TAG()
{
    return QS("glob");
}


QString MimeTypesReaderMetKeys::ISREGEX_ATTR()
{
    return QS("isregex");
}


QString MimeTypesReaderMetKeys::PATTERN_ATTR()
{
    return QS("pattern");
}


QString MimeTypesReaderMetKeys::MAGIC_TAG()
{
    return QS("magic");
}


QString MimeTypesReaderMetKeys::ALIAS_TAG()
{
    return QS("alias");
}


QString MimeTypesReaderMetKeys::ALIAS_TYPE_ATTR()
{
    return QS("type");
}


QString MimeTypesReaderMetKeys::ROOT_XML_TAG()
{
    return QS("root-XML");
}


QString MimeTypesReaderMetKeys::SUB_CLASS_OF_TAG()
{
    return QS("sub-class-of");
}


QString MimeTypesReaderMetKeys::SUB_CLASS_TYPE_ATTR()
{
    return QS("type");
}


QString MimeTypesReaderMetKeys::MAGIC_PRIORITY_ATTR()
{
    return QS("priority");
}


QString MimeTypesReaderMetKeys::MATCH_TAG()
{
    return QS("match");
}


QString MimeTypesReaderMetKeys::MATCH_OFFSET_ATTR()
{
    return QS("offset");
}


QString MimeTypesReaderMetKeys::MATCH_TYPE_ATTR()
{
    return QS("type");
}


QString MimeTypesReaderMetKeys::MATCH_VALUE_ATTR()
{
    return QS("value");
}


QString MimeTypesReaderMetKeys::MATCH_MASK_ATTR()
{
    return QS("mask");
}


QString MimeTypesReaderMetKeys::NS_URI_ATTR()
{
    return QS("namespaceURI");
}


QString MimeTypesReaderMetKeys::LOCAL_NAME_ATTR()
{
    return QS("localName");
}


QString MimeTypesReaderMetKeys::TIKA_LINK_TAG()
{
    return QS("tika:link");
}


QString MimeTypesReaderMetKeys::TIKA_UTI_TAG()
{
    return QS("tika:uti");
}


}       // namespace mime

}       // namespace qtika
