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
#include "creativecommons.h"


namespace qtika {

namespace metadata {


QString CreativeCommons::LICENSE_URL()
{
    return QS("License-Url");
}


QString CreativeCommons::LICENSE_LOCATION()
{
    return QS("License-Location");
}


QString CreativeCommons::WORK_TYPE()
{
    return QS("Work-Type");
}


}       // namespace metadata

}       // namespace qtika
