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

#include <QDateTime>


namespace qtika {

namespace utils {

namespace DateUtils {


/**
 * Returns a ISO 8601 representation of the given date. This method
 * is thread safe and non-blocking.
 *
 * @see <a href="https://issues.apache.org/jira/browse/TIKA-495">TIKA-495</a>
 * @param date given date
 * @return ISO 8601 date string, including timezone details
 */
QTIKACORESHARED_EXPORT QString formatDate(const QDateTime &date);


/**
 * Returns a ISO 8601 representation of the given date, which is
 *  in an unknown timezone. This method is thread safe and non-blocking.
 *
 * @see <a href="https://issues.apache.org/jira/browse/TIKA-495">TIKA-495</a>
 * @param date given date
 * @return ISO 8601 date string, without timezone details
 */
QTIKACORESHARED_EXPORT QString formatDateUnknownTimezone(const QDateTime &date);


}       // namespace DateUtils

}       // namespace metadata

}       // namespace qtika
