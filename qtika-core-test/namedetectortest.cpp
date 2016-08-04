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
#include "namedetectortest.h"

#include <detect/namedetector.h>
#include <metadata/metadata.h>

using qtika::metadata::Metadata;
using qtika::mime::MediaType;

namespace qtika {

namespace mime {

char *toString(const MediaType &type)
{
    using QTest::toString;
    return toString(type.toString());
}

}

}

#define assertDetect(type, name) {                          \
    Metadata meta;                                          \
    meta.set(Metadata::RESOURCE_NAME_KEY(), name);                 \
    QCOMPARE(detector_->detect(Q_NULLPTR, meta), type); }


void NameDetectorTest::initTestCase()
{
    using namespace qtika::detect;

    NameDetector::PatternMap patterns;
    patterns.insert(QRegularExpression("^.*\\.txt$", QRegularExpression::CaseInsensitiveOption),
                    Detector::MediaType::TEXT_PLAIN());
    patterns.insert(QRegularExpression("^README$"), Detector::MediaType::TEXT_PLAIN());

    detector_ = new NameDetector(patterns);
}


void NameDetectorTest::cleanupTestCase()
{
    delete detector_;
}


void NameDetectorTest::testDetect()
{
    assertDetect(MediaType::TEXT_PLAIN(), "text.txt");
    assertDetect(MediaType::TEXT_PLAIN(), "text.txt ");    // trailing space
    assertDetect(MediaType::TEXT_PLAIN(), "text.txt\n");   // trailing newline
    assertDetect(MediaType::TEXT_PLAIN(), "text.txt?a=b"); // URL query
    assertDetect(MediaType::TEXT_PLAIN(), "text.txt#abc"); // URL fragment
    assertDetect(MediaType::TEXT_PLAIN(), "text%2Etxt");   // URL encoded
    assertDetect(MediaType::TEXT_PLAIN(), "text.TXT");     // case insensitive
    assertDetect(MediaType::OCTET_STREAM(), "text.txt.gz");

    assertDetect(MediaType::TEXT_PLAIN(), "README");
    assertDetect(MediaType::TEXT_PLAIN(), " README ");     // space around
    assertDetect(MediaType::TEXT_PLAIN(), "\tREADME\n");   // other whitespace
    assertDetect(MediaType::TEXT_PLAIN(), "/a/README");    // leading path
    assertDetect(MediaType::TEXT_PLAIN(), "\\b\\README");  // windows path
    assertDetect(MediaType::OCTET_STREAM(), "ReadMe");     // case sensitive
    assertDetect(MediaType::OCTET_STREAM(), "README.NOW");

    // tough one
    assertDetect(
                MediaType::TEXT_PLAIN(),
                " See http://www.example.com:1234/README.txt?a=b#c \n");
    assertDetect(MediaType::TEXT_PLAIN(), "See README.txt"); // even this!
    assertDetect(MediaType::OCTET_STREAM(), "See README");   // but not this

    // test also the zero input cases
    assertDetect(MediaType::OCTET_STREAM(), "");
    assertDetect(MediaType::OCTET_STREAM(), QString());
    QCOMPARE(detector_->detect(Q_NULLPTR, Metadata()), MediaType::OCTET_STREAM());
}
