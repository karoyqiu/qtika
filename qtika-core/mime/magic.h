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

#include "clause.h"

#include <QSharedDataPointer>


namespace qtika {

namespace mime {


class MagicData;
class MimeType;


/**
 * Defines a magic for a MimeType. A magic is made of one or several
 * MagicClause.
 */
class Magic : public Clause
{
public:
    Magic(const MimeType &type, int priority, ClausePtr clause);
    Magic(const Magic &);
    Magic &operator=(const Magic &);
    virtual ~Magic();

    MimeType type() const;

    int priority() const;

    virtual bool eval(const QByteArray &data) const Q_DECL_OVERRIDE;

    virtual int size() const Q_DECL_OVERRIDE;

    virtual QString toString() const Q_DECL_OVERRIDE;

    bool operator<(const Magic &rhs) const;

private:
    QSharedDataPointer<MagicData> data;
};


bool operator==(const Magic &lhs, const Magic &rhs);

uint qHash(const Magic &m, uint seed);


}       // namespace mime

}       // namespace qtika
