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

#include <QString>


namespace qtika {

namespace mime {


/**
 * Met Keys used by the {@link MimeTypesReader}.
 */
class MimeTypesReaderMetKeys
{
protected:
    MimeTypesReaderMetKeys() Q_DECL_EQ_DEFAULT;

public:
    virtual ~MimeTypesReaderMetKeys() Q_DECL_EQ_DEFAULT;

protected:
    static QString MIME_INFO();

    static QString MIME_TYPE_TAG();

    static QString MIME_TYPE_TYPE_ATTR();

    static QString ACRONYM_TAG();

    static QString COMMENT_TAG();

    static QString GLOB_TAG();

    static QString ISREGEX_ATTR();

    static QString PATTERN_ATTR();

    static QString MAGIC_TAG();

    static QString ALIAS_TAG();

    static QString ALIAS_TYPE_ATTR();

    static QString ROOT_XML_TAG();

    static QString SUB_CLASS_OF_TAG();

    static QString SUB_CLASS_TYPE_ATTR();

    static QString MAGIC_PRIORITY_ATTR();

    static QString MATCH_TAG();

    static QString MATCH_OFFSET_ATTR();

    static QString MATCH_TYPE_ATTR();

    static QString MATCH_VALUE_ATTR();

    static QString MATCH_MASK_ATTR();

    static QString NS_URI_ATTR();

    static QString LOCAL_NAME_ATTR();

    static QString TIKA_LINK_TAG();

    static QString TIKA_UTI_TAG();
};


}       // namespace mime

}       // namespace qtika
