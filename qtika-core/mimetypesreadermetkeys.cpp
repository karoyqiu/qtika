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
    static const QString s("mime-info");
    return s;
}


QString MimeTypesReaderMetKeys::MIME_TYPE_TAG()
{
    static const QString s("mime-type");
    return s;
}


QString MimeTypesReaderMetKeys::MIME_TYPE_TYPE_ATTR()
{
    static const QString s("type");
    return s;
}


QString MimeTypesReaderMetKeys::ACRONYM_TAG()
{
    static const QString s("acronym");
    return s;
}


QString MimeTypesReaderMetKeys::COMMENT_TAG()
{
    static const QString s("_comment");
    return s;
}


QString MimeTypesReaderMetKeys::GLOB_TAG()
{
    static const QString s("glob");
    return s;
}


QString MimeTypesReaderMetKeys::ISREGEX_ATTR()
{
    static const QString s("isregex");
    return s;
}


QString MimeTypesReaderMetKeys::PATTERN_ATTR()
{
    static const QString s("pattern");
    return s;
}


QString MimeTypesReaderMetKeys::MAGIC_TAG()
{
    static const QString s("magic");
    return s;
}


QString MimeTypesReaderMetKeys::ALIAS_TAG()
{
    static const QString s("alias");
    return s;
}


QString MimeTypesReaderMetKeys::ALIAS_TYPE_ATTR()
{
    static const QString s("type");
    return s;
}


QString MimeTypesReaderMetKeys::ROOT_XML_TAG()
{
    static const QString s("root-XML");
    return s;
}


QString MimeTypesReaderMetKeys::SUB_CLASS_OF_TAG()
{
    static const QString s("sub-class-of");
    return s;
}


QString MimeTypesReaderMetKeys::SUB_CLASS_TYPE_ATTR()
{
    static const QString s("type");
    return s;
}


QString MimeTypesReaderMetKeys::MAGIC_PRIORITY_ATTR()
{
    static const QString s("priority");
    return s;
}


QString MimeTypesReaderMetKeys::MATCH_TAG()
{
    static const QString s("match");
    return s;
}


QString MimeTypesReaderMetKeys::MATCH_OFFSET_ATTR()
{
    static const QString s("offset");
    return s;
}


QString MimeTypesReaderMetKeys::MATCH_TYPE_ATTR()
{
    static const QString s("type");
    return s;
}


QString MimeTypesReaderMetKeys::MATCH_VALUE_ATTR()
{
    static const QString s("value");
    return s;
}


QString MimeTypesReaderMetKeys::MATCH_MASK_ATTR()
{
    static const QString s("mask");
    return s;
}


QString MimeTypesReaderMetKeys::NS_URI_ATTR()
{
    static const QString s("namespaceURI");
    return s;
}


QString MimeTypesReaderMetKeys::LOCAL_NAME_ATTR()
{
    static const QString s("localName");
    return s;
}


QString MimeTypesReaderMetKeys::TIKA_LINK_TAG()
{
    static const QString s("tika:link");
    return s;
}


QString MimeTypesReaderMetKeys::TIKA_UTI_TAG()
{
    static const QString s("tika:uti");
    return s;
}


}       // namespace mime

}       // namespace qtika
