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
#include "climateforcast.h"


namespace qtika {

namespace metadata {


QString ClimateForcast::PROGRAM_ID()
{
    return QS("prg_ID");
}


QString ClimateForcast::COMMAND_LINE()
{
    return QS("cmd_ln");
}


QString ClimateForcast::HISTORY()
{
    return QS("history");
}


QString ClimateForcast::TABLE_ID()
{
    return QS("table_id");
}


QString ClimateForcast::INSTITUTION()
{
    return QS("institution");
}


QString ClimateForcast::SOURCE()
{
    return QS("source");
}


QString ClimateForcast::CONTACT()
{
    return QS("contact");
}


QString ClimateForcast::PROJECT_ID()
{
    return QS("project_id");
}


QString ClimateForcast::CONVENTIONS()
{
    return QS("Conventions");
}


QString ClimateForcast::REFERENCES()
{
    return QS("references");
}


QString ClimateForcast::ACKNOWLEDGEMENT()
{
    return QS("acknowledgement");
}


QString ClimateForcast::REALIZATION()
{
    return QS("realization");
}


QString ClimateForcast::EXPERIMENT_ID()
{
    return QS("experiment_id");
}

QString ClimateForcast::COMMENT()
{
    return QS("comment");
}


QString ClimateForcast::MODEL_NAME_ENGLISH()
{
    return QS("model_name_english");
}


}       // namespace metadata

}       // namespace qtika
