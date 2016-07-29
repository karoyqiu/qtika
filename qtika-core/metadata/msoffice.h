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
 * A collection of Microsoft Office and Open Document property names.
 *
 * This is being replaced with cleaner, better defined properties in
 *  {@link Office}.
 */
class QTIKACORESHARED_EXPORT MSOffice
{
protected:
    MSOffice();

public:
    virtual ~MSOffice() Q_DECL_EQ_DEFAULT;

public:
    /** How long has been spent editing the document? */
    const QString EDIT_TIME;

    /**
     * For user defined metadata entries in the document,
     *  what prefix should be attached to the key names.
     * eg <meta:user-defined meta:name="Info1">Text1</meta:user-defined> becomes custom:Info1=Text1
     */
    const QString USER_DEFINED_METADATA_NAME_PREFIX;
};


}       // namespace metadata

}       // namespace qtika
