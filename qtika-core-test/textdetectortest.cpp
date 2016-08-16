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
#include "textdetectortest.h"

#include <QBuffer>

#include <mime/mediatype.h>
#include <metadata/metadata.h>

using qtika::mime::MediaType;
using qtika::metadata::Metadata;

#define assertNotText(data)     do { \
    QByteArray bytes = data;    \
    QBuffer buffer(&bytes); \
    buffer.open(QIODevice::ReadOnly);   \
    QCOMPARE(detector_.detect(&buffer, Metadata()), MediaType::OCTET_STREAM()); \
    } while (false)

#define assertText(data) do {                      \
    QByteArray bytes = data;                                       \
    QBuffer buffer(&bytes);                                     \
    buffer.open(QIODevice::ReadOnly);                           \
    QCOMPARE(detector_.detect(&buffer, Metadata()), MediaType::TEXT_PLAIN());              \
    for (int i = 0; i < bytes.length(); i++) {                  \
        char ch = 0;                                            \
        QVERIFY(buffer.getChar(&ch));                           \
        QCOMPARE(ch, bytes.at(i));                              \
    }                                                           \
    QVERIFY(buffer.atEnd()); } while (false)


void TextDetectorTest::testDetectNull()
{
    QCOMPARE(detector_.detect(Q_NULLPTR, Metadata()), MediaType::OCTET_STREAM());
}


void TextDetectorTest::testDetectEmpty()
{
    assertNotText(QByteArray());
}


void TextDetectorTest::testDetectText()
{
    assertText("Hello, World!");
    assertText(" \t\r\n");

    char d1[] = { -1, -2, -3, 0x09, 0x0A, 0x0C, 0x0D, 0x1B };
    assertNotText(QByteArray::fromRawData(d1, sizeof(d1)));

    char d2[] = { 0 };
    assertNotText(QByteArray::fromRawData(d2, sizeof(d2)));

    char d3[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    assertNotText(QByteArray::fromRawData(d3, sizeof(d3)));

    QByteArray data(512, '.');
    assertText(data);

    memset(data.data() + 100, 0x1f, 10);
    assertText(data);           // almost text

    memset(data.data() + 100, 0x1f, 11);
    assertNotText(data);        // no longer almost text, too many control chars

    memset(data.data(), 0x1f, data.length());
    assertNotText(data);

    data.resize(513);
    memset(data.data(), '.', data.length());
    data[0] = 0x1f;
    assertText(data);
    memset(data.data() + 100, 0x83, 50);
    assertText(data); // almost text
    memset(data.data() + 100, 0x83, 100);
    assertNotText(data); // no longer almost text, too many non-ASCII
    memset(data.data(), 0x1f, data.length());
    assertNotText(data);
}
