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
 * A collection of Creative Commons properties names.
 *
 * @see <a href="http://www.creativecommons.org/">creativecommons.org</a>
 */
class QTIKACORESHARED_EXPORT CreativeCommons abstract
{
public:
    static const QString LICENSE_URL;

    static const QString LICENSE_LOCATION;

    static const QString WORK_TYPE;
};


}       // namespace metadata

}       // namespace qtika
