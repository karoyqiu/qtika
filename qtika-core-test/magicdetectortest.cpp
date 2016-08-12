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
#include "magicdetectortest.h"

#include <QBuffer>
#include <QTest>

#include <detect/magicdetector.h>
#include <metadata/metadata.h>
#include <mime/mediatype.h>


using qtika::detect::MagicDetector;
using qtika::metadata::Metadata;
using qtika::mime::MediaType;


/**
 * InputStream class that does not read in all available bytes in
 * one go.
 */
class RestrictiveBuffer : public QBuffer
{
public:
    using QBuffer::QBuffer;

protected:
    virtual qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE
    {
        if (maxlen > 10)
        {
            maxlen -= 10;
        }

        return QBuffer::readData(data, maxlen);
    }
};


#define assertDetect(d, type, s) do {                           \
    QByteArray raw = QByteArray::fromRawData(s, sizeof(s));   \
    assertDetectBytes(d, type, raw); } while (false)

#define assertDetectBytes(d, type, b) do {                      \
    QByteArray bytes = b;                                       \
    QBuffer buffer(&bytes);                                     \
    buffer.open(QIODevice::ReadOnly);                           \
    QCOMPARE(d.detect(&buffer, Metadata()), type);              \
    for (int i = 0; i < bytes.length(); i++) {                  \
        char ch = 0;                                            \
        QVERIFY(buffer.getChar(&ch));                           \
        QCOMPARE(ch, bytes.at(i));                              \
    }                                                           \
    QVERIFY(buffer.atEnd()); } while (false)


void MagicDetectorTest::testDetectNull()
{
    MediaType html("text", "html");
    MagicDetector detector(html, "<html");
    QCOMPARE(detector.detect(Q_NULLPTR, Metadata()), MediaType::OCTET_STREAM());
}


void MagicDetectorTest::testDetectSimple()
{
    MediaType html("text", "html");
    MagicDetector detector(html, "<html");

    assertDetect(detector, html, "<html");
    assertDetect(detector, html, "<html><head/><body/></html>");
    assertDetect(detector, MediaType::OCTET_STREAM(), "<HTML");
    assertDetect(detector, MediaType::OCTET_STREAM(), "<?xml?><html");
    assertDetect(detector, MediaType::OCTET_STREAM(), " <html");
    assertDetect(detector, MediaType::OCTET_STREAM(), "");
}


void MagicDetectorTest::testDetectOffsetRange()
{
    MediaType html("text", "html");
    MagicDetector detector(html, "<html", QByteArray(), 0, 64);

    assertDetect(detector, html, "<html");
    assertDetect(detector, html, "<html><head/><body/></html>");
    assertDetect(detector, html, "<?xml?><html/>");
    assertDetect(detector, html, "\n    <html");
    assertDetect(detector, html, "\u0000<html");
    assertDetect(detector, MediaType::OCTET_STREAM(), "<htm");
    assertDetect(detector, MediaType::OCTET_STREAM(), " html");
    assertDetect(detector, MediaType::OCTET_STREAM(), "<HTML");

    assertDetect(detector, html,
                 "0........1.........2.........3.........4.........5.........6"
                 "1234<html");
    assertDetect(detector, MediaType::OCTET_STREAM(),
                 "0........1.........2.........3.........4.........5.........6"
                 "12345<html");
    assertDetect(detector, MediaType::OCTET_STREAM(), "");
}


void MagicDetectorTest::testDetectMask()
{
    MediaType html("text", "html");
    char pattern[] = { '<',  'H',  'T',  'M',  'L' };
    quint8 mask[] = { 0xff, 0xdf, 0xdf, 0xdf, 0xdf };

    MagicDetector detector(html, QByteArray::fromRawData(pattern, sizeof(pattern)),
                           QByteArray::fromRawData((const char *)mask, sizeof(mask)),
                           0, 64);

    assertDetect(detector, html, "<html");
    assertDetect(detector, html, "<HTML><head/><body/></html>");
    assertDetect(detector, html, "<?xml?><HtMl/>");
    assertDetect(detector, html, "\n    <html");
    assertDetect(detector, html, "\u0000<HTML");
    assertDetect(detector, MediaType::OCTET_STREAM(), "<htm");
    assertDetect(detector, MediaType::OCTET_STREAM(), " html");

    assertDetect(detector, html,
                 "0........1.........2.........3.........4.........5.........6"
                 "1234<html");
    assertDetect(detector, MediaType::OCTET_STREAM(),
                 "0........1.........2.........3.........4.........5.........6"
                 "12345<html");

    assertDetect(detector, MediaType::OCTET_STREAM(), "");
}


void MagicDetectorTest::testDetectRegExPDF()
{
    MediaType pdf("application", "pdf");
    MagicDetector detector(pdf, "^(.|[\r\n]){0,144}%PDF-", QByteArray(), true, 0, 0);

    assertDetect(detector, pdf, "%PDF-1.0");
    assertDetect(detector, pdf,
            "0        10        20        30        40        50        6"
            "0        70        80        90        100       110       1"
            "20       130       140"
            "34%PDF-1.0");
    assertDetect(detector, MediaType::OCTET_STREAM(),
            "0        10        20        30        40        50        6"
            "0        70        80        90        100       110       1"
            "20       130       140"
            "345%PDF-1.0");
    assertDetect(detector, MediaType::OCTET_STREAM(), "");
}


void MagicDetectorTest::testDetectRegExGreedy()
{
    QByteArray pattern("^\\x3chtml xmlns=\"http://www\\.w3\\.org/1999/xhtml"
                       "\".*\\x3ctitle\\x3e.*\\x3c/title\\x3e");
    MediaType xhtml("application", "xhtml+xml");
    MagicDetector detector(xhtml, pattern, QByteArray(), true, 0, 8192);

    assertDetect(detector, xhtml,
            "<html xmlns=\"http://www.w3.org/1999/xhtml\">"
            "<head><title>XHTML test document</title></head>");
}


void MagicDetectorTest::testDetectRegExOptions()
{
    QByteArray pattern("^.{0,1024}\\x3c\\!(?:DOCTYPE|doctype) (?:HTML|html) "
                       "(?:PUBLIC|public) \"-//.{1,16}//(?:DTD|dtd) .{0,64}"
                       "(?:HTML|html) 4\\.01");
    const char data[] =
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\""
        "\"http://www.w3.org/TR/html4/strict.dtd\"><HTML>"
        "<HEAD><TITLE>HTML document</TITLE></HEAD>"
        "<BODY><P>Hello world!</BODY></HTML>";

    const char data1[] =
        "<!DOCTYPE html PUBLIC \"-//W3C//dtd html 4.01//EN\""
        "\"http://www.w3.org/TR/html4/strict.dtd\"><HTML>"
        "<HEAD><TITLE>HTML document</TITLE></HEAD>"
        "<BODY><P>Hello world!</BODY></HTML>";

    const char data2[] =
        "<!DoCtYpE hTmL pUbLiC \"-//W3C//dTd HtMl 4.01//EN\""
        "\"http://www.w3.org/TR/html4/strict.dtd\"><HTML>"
        "<HEAD><TITLE>HTML document</TITLE></HEAD>"
        "<BODY><P>Hello world!</BODY></HTML>";

    MediaType html("text", "html");
    MagicDetector detector(html, pattern, QByteArray(), true, 0, 0);

    assertDetect(detector, html, data);
    assertDetect(detector, html, data1);
    assertDetect(detector, MediaType::OCTET_STREAM(), data2);
}


void MagicDetectorTest::testDetectStreamReadProblems()
{
    QByteArray data("abcdefghijklmnopqrstuvwxyz0123456789");
    MediaType testMT("application", "test");
    MagicDetector detector(testMT, data, QByteArray(), false, 0, 0);
    RestrictiveBuffer stream(&data);
    stream.open(QIODevice::ReadOnly);
    QCOMPARE(detector.detect(&stream, Metadata()), testMT);
}


void MagicDetectorTest::testDetectString()
{
    QString data("abcdEFGhijklmnoPQRstuvwxyz0123456789");
    MediaType testMT("application", "test");

    // Check regular String matching
    MagicDetector detector = MagicDetector::parse(testMT, "string", "0:20", "abcd", QString());
    assertDetectBytes(detector, testMT, data.toLatin1());
    detector = MagicDetector::parse(testMT, "string", "0:20", "cdEFGh", QString());
    assertDetectBytes(detector, testMT, data.toLatin1());

    // Check Little Endian and Big Endian utf-16 strings
    detector = MagicDetector::parse(testMT, "unicodeLE", "0:20", "cdEFGh", QString());
    QByteArray utf = QByteArray::fromRawData((const char *)data.utf16(), data.length() * sizeof(ushort));
    assertDetectBytes(detector, testMT, utf);
    //detector = MagicDetector::parse(testMT, "unicodeBE", "0:20", "cdEFGh", QString());
    //assertDetectBytes(detector, testMT, data.toLatin1());

    // Check case ignoring String matching
    detector = MagicDetector::parse(testMT, "stringignorecase", "0:20", "BcDeFgHiJKlm", QString());
    assertDetectBytes(detector, testMT, data.toLatin1());
}
