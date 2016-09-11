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
#include "dateformat.h"

#include <QTimeZone>
#include <QRegularExpression>


namespace qtika {

namespace metadata {


DateFormat::DateFormat(const QString &format, Qt::TimeSpec spec)
    : format_(format)
    , spec_(spec)
{
    if (format.endsWith(QLatin1String("Z")))
    {
        format_.chop(1);
        spec_ = Qt::TimeZone;
    }
}


QDateTime DateFormat::fromString(const QString &string) const
{
    QString s = string;
    QString tz;

    if (spec_ == Qt::TimeZone)
    {
        static const QRegularExpression plusMinus(QS("[+-]"));
        int pos = string.lastIndexOf(plusMinus);

        if (pos != -1)
        {
            s = string.left(pos);
            tz = string.mid(pos);
        }
    }

    QDateTime dt = QDateTime::fromString(s, format_);

    if (dt.isValid())
    {
        if (spec_ == Qt::OffsetFromUTC)
        {
            dt.setTimeSpec(Qt::OffsetFromUTC);
            dt.setOffsetFromUtc(-12 * 60 * 60);
        }
        else if (spec_ == Qt::TimeZone)
        {
            QByteArray id("UTC");
            id += tz.toUtf8();
            dt.setTimeSpec(Qt::TimeZone);
            dt.setTimeZone(QTimeZone(id));
        }
        else
        {
            dt.setTimeSpec(spec_);
        }
    }

    return dt;
}


}       // namespace metadata

}       // namespace qtika
