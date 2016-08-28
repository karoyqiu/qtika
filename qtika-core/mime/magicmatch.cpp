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
#include "magicmatch.h"

#include <QBuffer>

#include "detect/magicdetector.h"
#include "mediatype.h"
#include "metadata/metadata.h"


namespace qtika {

namespace mime {


class MagicMatchData : public QSharedData
{
public:
    MagicMatchData(const MediaType &mediaType, const QString &type, const QString &offset,
                   const QString &value, const QString &mask);

    MediaType mediaType;
    QString type;
    QString offset;
    QString value;
    QString mask;
    detect::MagicDetector detector;
};


MagicMatchData::MagicMatchData(const MediaType &mt, const QString &t,
                               const QString &o, const QString &v, const QString &m)
    : mediaType(mt)
    , type(t)
    , offset(o)
    , value(v)
    , mask(m)
    , detector(detect::MagicDetector::parse(mt, t, o, v, m))
{
}


MagicMatch::MagicMatch(const MediaType &mediaType, const QString &type, const QString &offset,
                       const QString &value, const QString &mask)
    : data_(new MagicMatchData(mediaType, type, offset, value, mask))
{
}


MagicMatch::MagicMatch(const MagicMatch &rhs)
    : data_(rhs.data_)
{
}


MagicMatch &MagicMatch::operator=(const MagicMatch &rhs)
{
    if (this != &rhs)
    {
        data_.operator=(rhs.data_);
    }

    return *this;
}


MagicMatch::~MagicMatch()
{
}


detect::MagicDetector MagicMatch::detector() const
{
    return data_->detector;
}


bool MagicMatch::eval(const QByteArray &data) const
{
    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);
    return data_->detector.detect(&buffer, metadata::Metadata()) != MediaType::OCTET_STREAM();
}


int MagicMatch::size() const
{
    return data_->detector.length();
}


QString MagicMatch::toString() const
{
    return QString("%1 %2 %3 %4 %5").arg(data_->mediaType.toString()).arg(data_->type)
            .arg(data_->offset).arg(data_->value).arg(data_->mask);
}


}       // namespace mime

}       // namespace qtika
