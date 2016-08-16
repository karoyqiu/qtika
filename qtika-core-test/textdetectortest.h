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
#include <detect/textdetector.h>


class TextDetectorTest : public QObject
{
    Q_OBJECT

private slots:
    void testDetectNull();

    /**
     * Test for type detection of empty documents.
     *
     * @see <a href="https://issues.apache.org/jira/browse/TIKA-483">TIKA-483</a>
     */
    void testDetectEmpty();

    void testDetectText();

private:
    const qtika::detect::TextDetector detector_;
};
