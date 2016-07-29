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
 * Met keys from NCAR CCSM files in the <a
 * href="http://cf-pcmdi.llnl.gov/">Climate Forecast Convention</a>.
 */
class QTIKACORESHARED_EXPORT ClimateForcast
{
protected:
    ClimateForcast();

public:
    virtual ~ClimateForcast() Q_DECL_EQ_DEFAULT;

public:
    const QString PROGRAM_ID;
    const QString COMMAND_LINE;
    const QString HISTORY;
    const QString TABLE_ID;
    const QString INSTITUTION;
    const QString SOURCE;
    const QString CONTACT;
    const QString PROJECT_ID;
    const QString CONVENTIONS;
    const QString REFERENCES;
    const QString ACKNOWLEDGEMENT;
    const QString REALIZATION;
    const QString EXPERIMENT_ID;
    const QString COMMENT;
    const QString MODEL_NAME_ENGLISH;
};


}       // namespace metadata

}       // namespace qtika
