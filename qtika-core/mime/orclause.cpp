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
#include "orclause.h"

#include "mimetype.h"


namespace qtika {

namespace mime {


class OrClauseData : public QSharedData
{
public:
    QList<ClausePtr> clauses;
};


OrClause::OrClause(const QList<ClausePtr> clauses)
    : data_(new OrClauseData)
{
    data_->clauses = clauses;
}


OrClause::OrClause(const OrClause &rhs)
    : data_(rhs.data_)
{
}


OrClause &OrClause::operator=(const OrClause &rhs)
{
    if (this != &rhs)
    {
        data_.operator=(rhs.data_);
    }

    return *this;
}


OrClause::~OrClause()
{
}


bool OrClause::eval(const QByteArray &data) const
{
    for (auto c : data_->clauses)
    {
        if (c->eval(data))
        {
            return true;
        }
    }

    return false;
}


int OrClause::size() const
{
    int size = 0;

    for (auto c : data_->clauses)
    {
        size = qMax(size, c->size());
    }

    return size;
}


QString OrClause::toString() const
{
    QStringList s;

    for (auto c : data_->clauses)
    {
        s.append(c->toString());
    }

    return QString("or { %1 }").arg(s.join(", "));
}


}       // namespace mime

}       // namespace qtika
