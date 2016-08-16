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

#include <QtTest>
#include <QCoreApplication>

#include "mediatypetest.h"
#include "metadatatest.h"
#include "namedetectortest.h"
#include "magicdetectortest.h"
#include "textdetectortest.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QTEST_SET_MAIN_SOURCE_PATH

    int result = 0;

    MediaTypeTest mediaTypeTest;
    result += QTest::qExec(&mediaTypeTest);

    MetadataTest metadataTest;
    result += QTest::qExec(&metadataTest);

    NameDetectorTest nameDetectorTest;
    result += QTest::qExec(&nameDetectorTest);

    MagicDetectorTest magicDetectorTest;
    result += QTest::qExec(&magicDetectorTest);

    TextDetectorTest textDetectorTest;
    result += QTest::qExec(&textDetectorTest);

    return result;
}
