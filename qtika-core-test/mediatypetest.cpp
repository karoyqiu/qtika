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
#include "mediatypetest.h"

#include <QTest>
#include <mime/mediatype.h>

#define QS(x)   QStringLiteral(x)

using qtika::mime::MediaType;


void MediaTypeTest::testBasics()
{
    QCOMPARE(MediaType("application", "octet-stream").toString(), QS("application/octet-stream"));
    QCOMPARE(MediaType("text", "plain").toString(), QS("text/plain"));

    MediaType::ParameterMap params;
    QCOMPARE(MediaType("text", "plain", params).toString(), QS("text/plain"));

    params.insert("charset", "UTF-8");
    QCOMPARE(MediaType("text", "plain", params).toString(), QS("text/plain; charset=UTF-8"));

    params.insert("x-eol-style", "crlf");
    QCOMPARE(MediaType("text", "plain", params).toString(),
             QS("text/plain; charset=UTF-8; x-eol-style=crlf"));
}


void MediaTypeTest::testLowerCase()
{
    QCOMPARE(MediaType("TEXT", "PLAIN").toString(), QS("text/plain"));
    QCOMPARE(MediaType("Text", "Plain").toString(), QS("text/plain"));

    MediaType::ParameterMap params;
    QCOMPARE(MediaType("text", "PLAIN", params).toString(), QS("text/plain"));

    params.insert("CHARSET", "UTF-8");
    QCOMPARE(MediaType("TEXT", "plain", params).toString(), QS("text/plain; charset=UTF-8"));

    params.insert("X-Eol-Style", "crlf");
    QCOMPARE(MediaType("TeXt", "PlAiN", params).toString(),
             QS("text/plain; charset=UTF-8; x-eol-style=crlf"));
}


void MediaTypeTest::testTrim()
{
    QCOMPARE(MediaType(" text ", " plain ").toString(), QS("text/plain"));
    QCOMPARE(MediaType("\ttext", "plain\t").toString(), QS("text/plain"));

    MediaType::ParameterMap params;
    QCOMPARE(MediaType("text\r\n", "\tplain", params).toString(), QS("text/plain"));

    params.insert(" charset", "UTF-8");
    QCOMPARE(MediaType("\n\ntext", "plain \r", params).toString(), QS("text/plain; charset=UTF-8"));

    params.insert("\r\n\tx-eol-style  \t", "crlf");
    QCOMPARE(MediaType("    text", "\tplain ", params).toString(),
             QS("text/plain; charset=UTF-8; x-eol-style=crlf"));
}


void MediaTypeTest::testQuote()
{
    MediaType::ParameterMap params;
    params.insert("a", " value with spaces ");
    params.insert("b", "text/plain");
    params.insert("c", "()<>@,;:\\\"/[]?=");
    QCOMPARE(MediaType("text", "plain", params).toString(),
             QS("text/plain; a=\" value with spaces \"; b=\"text\\/plain\""
                "; c=\"\\(\\)\\<\\>\\@\\,\\;\\:\\\\\\\"\\/\\[\\]\\?\\=\""));
}


void MediaTypeTest::testParseWithParams()
{
    QString mimeStringWithParams("text/html;charset=UTF-8;foo=bar;foo2=bar2");
    MediaType type = MediaType::parse(mimeStringWithParams);
    QVERIFY(!type.isNull());
    QVERIFY(!type.parameters().isEmpty());
    QCOMPARE(type.parameters().size(), 3);
    QVERIFY(type.parameters().contains("charset"));
    QVERIFY(type.parameters().contains("foo"));
    QVERIFY(type.parameters().contains("foo2"));
}


void MediaTypeTest::testParseWithParamsAndQuotedCharset()
{
    // Typical case, with a quoted charset
    QString mimeStringWithParams("text/html;charset=\"UTF-8\"");
    MediaType type = MediaType::parse(mimeStringWithParams);
    QVERIFY(!type.isNull());
    QCOMPARE(type.parameters(), (MediaType::ParameterMap { { "charset", "UTF-8" } }));

    // Complex case, with various different quoted and un-quoted forms
    mimeStringWithParams = "text/html;charset=\'UTF-8\';test=\"true\";unquoted=here";
    type = MediaType::parse(mimeStringWithParams);
    QVERIFY(!type.isNull());
    QCOMPARE(type.parameters().size(), 3);
    QCOMPARE(type.parameters().value("charset"), QS("UTF-8"));
    QCOMPARE(type.parameters().value("test"), QS("true"));
    QCOMPARE(type.parameters().value("unquoted"), QS("here"));
}


void MediaTypeTest::testParseNoParams()
{
    QString mimeStringNoParams("text/html");
    MediaType type = MediaType::parse(mimeStringNoParams);
    QVERIFY(!type.isNull());
    QVERIFY(type.parameters().isEmpty());
}


void MediaTypeTest::testParseNoParamsWithSemi()
{
    QString mimeStringNoParams("text/html;");
    MediaType type = MediaType::parse(mimeStringNoParams);
    QVERIFY(!type.isNull());
    QVERIFY(type.parameters().isEmpty());
}


void MediaTypeTest::testOddParameters()
{
    QCOMPARE(MediaType::parse("text/html;; charset=UTF-8").toString(),
             QS("text/html; charset=UTF-8"));
    QCOMPARE(MediaType::parse("text/html;; charset=\"UTF-8\"").toString(),
             QS("text/html; charset=UTF-8"));
    QCOMPARE(MediaType::parse("text/html;; charset=\"UTF-8").toString(),
             QS("text/html; charset=UTF-8"));
}
