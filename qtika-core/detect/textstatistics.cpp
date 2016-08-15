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
#include "textstatistics.h"


namespace qtika {

namespace detect {


class TextStatisticsData : public QSharedData
{
public:
    int count(int b) const;
    int count(int from, int to) const;
    int countSafeControl() const;

public:
    int counts[256];
    int total;
};


int TextStatisticsData::count(int b) const
{
    return counts[b & 0xff];
}


int TextStatisticsData::count(int from, int to) const
{
    Q_ASSERT(from >= 0 && to <= sizeof(counts));

    int count = 0;

    for (int i = from; i < to; i++)
    {
        count += counts[i];
    }

    return count;
}


int TextStatisticsData::countSafeControl() const
{
    return count('\t') + count('\n') + count('\r') + count(0x0c) + count(0x1b);
}


TextStatistics::TextStatistics()
    : data(new TextStatisticsData)
{
    memset(data->counts, 0, sizeof(data->counts));
    data->total = 0;
}


TextStatistics::TextStatistics(const TextStatistics &rhs)
    : data(rhs.data)
{
}


TextStatistics &TextStatistics::operator=(const TextStatistics &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


TextStatistics::~TextStatistics()
{
}


void TextStatistics::addData(const QByteArray &buffer, int offset, int length)
{
    for (int i = 0; i < length; i++)
    {
        data->counts[buffer.at(offset + i) & 0xff]++;
        data->total++;
    }
}


bool TextStatistics::isMostlyAscii() const
{
    int control = data->count(0, 0x20);
    int ascii = data->count(0x20, 128);
    int safe = data->countSafeControl();

    return data->total > 0
        && (control - safe) * 100 < data->total * 2
            && (ascii + safe) * 100 > data->total * 90;
}


bool TextStatistics::looksLikeUTF8() const
{
    int control = data->count(0, 0x20);
    int utf8 = data->count(0x20, 0x80);
    int safe = data->countSafeControl();

    int expectedContinuation = 0;
    int leading[] = { data->count(0xc0, 0xe0), data->count(0xe0, 0xf0), data->count(0xf0, 0xf8) };

    for (int i = 0; i < 3; i++)
    {
        utf8 += leading[i];
        expectedContinuation += (i + 1) * leading[i];
    }

    int continuation = data->count(0x80, 0xc0);

    return utf8 > 0
            && continuation <= expectedContinuation
            && continuation >= expectedContinuation - 3
            && data->count(0xf80, 0x100) == 0
            && (control - safe) * 100 < utf8 * 2;
}


int TextStatistics::count() const
{
    return data->total;
}


int TextStatistics::count(int b) const
{
    return data->count(b);
}


int TextStatistics::countControl() const
{
    return data->count(0, 0x20) - data->countSafeControl();
}


int TextStatistics::countSafeAscii() const
{
    return data->count(0x20, 128) + data->countSafeControl();
}


int TextStatistics::countEightBit() const
{
    return data->count(128, 256);
}



}       // namespace detect

}       // namespace qtika
