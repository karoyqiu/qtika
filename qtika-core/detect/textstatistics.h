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
#include "detector.h"

#include <QSharedDataPointer>


namespace qtika {

namespace detect {


class TextStatisticsData;


/**
 * Utility class for computing a histogram of the bytes seen in a stream.
 *
 * @since Apache Tika 1.2
 */
class TextStatistics
{
public:
    TextStatistics();
    TextStatistics(const TextStatistics &);
    TextStatistics &operator=(const TextStatistics &);
    ~TextStatistics();

    void addData(const QByteArray &buffer, int offset, int length);

    /**
     * Checks whether at least one byte was seen and that the bytes that
     * were seen were mostly plain text (i.e. < 2% control, > 90% ASCII range).
     *
     * @see <a href="https://issues.apache.org/jira/browse/TIKA-483">TIKA-483</a>
     * @see <a href="https://issues.apache.org/jira/browse/TIKA-688">TIKA-688</a>
     * @return <code>true</code> if the seen bytes were mostly safe ASCII,
     *         <code>false</code> otherwise
     */
    bool isMostlyAscii() const;

    /**
     * Checks whether the observed byte stream looks like UTF-8 encoded text.
     *
     * @since Apache Tika 1.3
     * @return <code>true</code> if the seen bytes look like UTF-8,
     *         <code>false</code> otherwise
     */
    bool looksLikeUTF8() const;

    /**
     * Returns the total number of bytes seen so far.
     *
     * @return count of all bytes
     */
    int count() const;

    /**
     * Returns the number of occurrences of the given byte.
     *
     * @param b byte
     * @return count of the given byte
     */
    int count(int b) const;

    /**
     * Counts control characters (i.e. < 0x20, excluding tab, CR, LF,
     * page feed and escape).
     * <p>
     * This definition of control characters is based on section 4 of the
     * "Content-Type Processing Model" Internet-draft
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
     * @return count of control characters
     */
    int countControl() const;

    /**
     * Counts "safe" (i.e. seven-bit non-control) ASCII characters.
     *
     * @see #countControl()
     * @return count of safe ASCII characters
     */
    int countSafeAscii() const;

    /**
     * Counts eight bit characters, i.e. bytes with their highest bit set.
     *
     * @return count of eight bit characters
     */
    int countEightBit() const;
private:
    QSharedDataPointer<TextStatisticsData> data;
};


}       // namespace detect

}       // namespace qtika
