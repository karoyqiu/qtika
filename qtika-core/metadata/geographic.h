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
#include "property.h"


namespace qtika {

namespace metadata {


/**
 * Geographic schema. This is a collection of
 * {@link Property property definition} constants for geographic
 * information, as defined in the W3C Geo Vocabularies.
 *
 * @since Apache Tika 0.8
 * @see <a href="http://www.w3.org/2003/01/geo/"
 *        >W3C Basic Geo Vocabulary</a>
 */
class QTIKACORESHARED_EXPORT Geographic
{
protected:
    Geographic() Q_DECL_EQ_DEFAULT;

public:
    virtual ~Geographic() Q_DECL_EQ_DEFAULT;

public:
    /**
     * The WGS84 Latitude of the Point
     */
    static Property LATITUDE();

    /**
     * The WGS84 Longitude of the Point
     */
    static Property LONGITUDE();

    /**
     * The WGS84 Altitude of the Point
     */
    static Property ALTITUDE();
};


}       // namespace metadata

}       // namespace qtika
