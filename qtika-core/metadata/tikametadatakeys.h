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

#include <QString>


namespace qtika {

namespace metadata {


/**
 * Contains keys to properties in Metadata instances.
 */
class QTIKACORESHARED_EXPORT TikaMetadataKeys
{
protected:
    TikaMetadataKeys() Q_DECL_EQ_DEFAULT;

public:
    virtual ~TikaMetadataKeys() Q_DECL_EQ_DEFAULT;

    static QString RESOURCE_NAME_KEY();
    static QString PROTECTED();
    static QString EMBEDDED_RELATIONSHIP_ID();
    static QString EMBEDDED_STORAGE_CLASS_ID();
    static QString EMBEDDED_RESOURCE_TYPE();
};


}       // namespace metadata

}       // namespace qtika
