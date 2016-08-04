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
#include "message.h"


namespace qtika {

namespace metadata {


QString Message::MESSAGE_RECIPIENT_ADDRESS()
{
    return QS("Message-Recipient-Address");
}


QString Message::MESSAGE_FROM()
{
    return QS("Message-From");
}


QString Message::MESSAGE_TO()
{
    return QS("Message-To");
}


QString Message::MESSAGE_CC()
{
    return QS("Message-Cc");
}


QString Message::MESSAGE_BCC()
{
    return QS("Message-Bcc");
}


}       // namespace metadata

}       // namespace qtika
