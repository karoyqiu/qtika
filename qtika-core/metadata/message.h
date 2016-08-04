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
 * A collection of Message related property names.
 */
class QTIKACORESHARED_EXPORT Message
{
protected:
    Message() Q_DECL_EQ_DEFAULT;

public:
    virtual ~Message() Q_DECL_EQ_DEFAULT;

    static QString MESSAGE_RECIPIENT_ADDRESS();
    static QString MESSAGE_FROM();
    static QString MESSAGE_TO();
    static QString MESSAGE_CC();
    static QString MESSAGE_BCC();
};


}       // namespace metadata

}       // namespace qtika
