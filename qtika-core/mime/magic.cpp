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
#include "magic.h"

#include "mimetype.h"


namespace qtika {

namespace mime {


class MagicData : public QSharedData
{
public:
    MagicData();

    MimeType type;
    int priority;
    Clause *clause;
    QString string;
};


MagicData::MagicData()
    : priority(0)
    , clause(Q_NULLPTR)
{
}


Magic::Magic(const MimeType &type, int priority, Clause *clause)
    : data(new MagicData)
{
    data->type = type;
    data->priority = priority;
    data->clause = clause;
    data->string = QString("[%1/%2]").arg(priority).arg(clause->toString());
}


Magic::Magic(const Magic &rhs)
    : data(rhs.data)
{
}


Magic &Magic::operator=(const Magic &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


Magic::~Magic()
{
}


MimeType Magic::type() const
{
    return data->type;
}


int Magic::priority() const
{
    return data->priority;
}


bool Magic::eval(const QByteArray &d) const
{
    return data->clause->eval(d);
}


int Magic::size() const
{
    return data->clause->size();
}


QString Magic::toString() const
{
    return data->string;
}


bool Magic::operator<(const Magic &rhs) const
{
    int diff = rhs.data->priority - data->priority;

    if (diff == 0)
    {
        diff = rhs.size() - size();
    }

    if (diff == 0)
    {
        if (rhs.type() < type())
        {
            diff = -1;
        }
        else if (rhs.type() != type())
        {
            diff = 1;
        }
    }

    if (diff == 0)
    {
        diff = rhs.toString().compare(toString());
    }

    return (diff < 0);
}


bool operator==(const Magic &lhs, const Magic &rhs)
{
    return lhs.toString() == rhs.toString();
}


uint qHash(const Magic &m, uint seed)
{
    return qHash(m.type(), seed) ^ qHash(m.toString(), seed);
}


}       // namespace mime

}       // namespace qtika
