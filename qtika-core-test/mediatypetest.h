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

#include <QObject>


class MediaTypeTest : public QObject
{
    Q_OBJECT

private slots:
    void testBasics();
    void testLowerCase();
    void testTrim();
    void testQuote();

    /**
     * @since TIKA-121
     */
    void testParseWithParams();

    /**
     * Per http://tools.ietf.org/html/rfc2045#section-5.1, charset can be in quotes
     */
    void testParseWithParamsAndQuotedCharset();

    /**
     * @since TIKA-121
     */
    void testParseNoParams();

    /**
     * @since TIKA-121
     */
    void testParseNoParamsWithSemi();

    /**
     * TIKA-349
     */
    void testOddParameters();
};
