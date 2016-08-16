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
#include "textdetector.h"

#include <qiodevicetransactionrollback.h>

#include "mime/mediatype.h"
#include "textstatistics.h"


namespace qtika {

namespace detect {


class TextDetectorData : public QSharedData
{
public:
    /**
     * Lookup table for all the ASCII/ISO-Latin/UTF-8/etc. control bytes
     * in the range below 0x20 (the space character). If an entry in this
     * table is <code>true</code> then that byte is very unlikely to occur
     * in a plain text document.
     * <p>
     * The contents of this lookup table are based on the following definition
     * from section 4 of the "Content-Type Processing Model" Internet-draft
     * (<a href="http://webblaze.cs.berkeley.edu/2009/mime-sniff/mime-sniff.txt"
     * >draft-abarth-mime-sniff-01</a>).
     * <pre>
     * +-------------------------+
     * | Binary data byte ranges |
     * +-------------------------+
     * | 0x00 -- 0x08            |
     * | 0x0B                    |
     * | 0x0E -- 0x1A            |
     * | 0x1C -- 0x1F            |
     * +-------------------------+
     * </pre>
     *
     * @see <a href="https://issues.apache.org/jira/browse/TIKA-154">TIKA-154</a>
     */
    static const bool IS_CONTROL_BYTE[0x20];

    int bytesToTest;
};


const bool TextDetectorData::IS_CONTROL_BYTE[0x20] = {
    true, true, true, true, true, true, true, true,
    true, false, false, true, false, false, true, true,     // 0x09 - tabulator, 0x0a - new line, 0x0c - new page, 0x0d - carriage return
    true, true, true, true, true, true, true, true,
    true, true, true, false, true, true, true, true         // 0x1b - escape
};

const int TextDetector::DEFAULT_NUMBER_OF_BYTES_TO_TEST = 512;


TextDetector::TextDetector(int bytesToTest)
    : data(new TextDetectorData)
{
    data->bytesToTest = bytesToTest;
}


TextDetector::TextDetector(const TextDetector &rhs)
    : data(rhs.data)
{
}


TextDetector &TextDetector::operator=(const TextDetector &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


TextDetector::~TextDetector()
{
}


Detector::MediaType TextDetector::detect(QIODevice *input, const Metadata &meta) const
{
    Q_UNUSED(meta)

    if (input == Q_NULLPTR)
    {
        return MediaType::OCTET_STREAM();
    }

    qtika::internal::QIODeviceTransactionRollback rollback(input);

    TextStatistics stats;
    char buffer[1024] = { 0 };
    qint64 n = 0;
    qint64 m = input->read(buffer, qMin<qint64>(data->bytesToTest, sizeof(buffer)));

    while (m > 0 && n < data->bytesToTest)
    {
        stats.addData(QByteArray::fromRawData(buffer, m), 0, m);
        n += m;
        m = input->read(buffer, qMin<qint64>(data->bytesToTest - n, sizeof(buffer)));
    }

    if (stats.isMostlyAscii() || stats.looksLikeUTF8())
    {
        return MediaType::TEXT_PLAIN();
    }

    return MediaType::OCTET_STREAM();
}


}       // namespace detect

}       // namespace qtika
