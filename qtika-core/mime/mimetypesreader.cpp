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
#include "mimetypesreader.h"

#include <QEnableSharedFromThis>
#include <QUrl>

#include "magic.h"
#include "mimetype.h"
#include "mimetypes.h"
#include "clause.h"
#include "orclause.h"
#include "andclause.h"
#include "magicmatch.h"


namespace qtika {

namespace mime {

namespace {

class ClauseRecord : public QEnableSharedFromThis<ClauseRecord>
{
public:
    explicit ClauseRecord(ClausePtr clause);

    QList<ClausePtr> clauses() const { return subclauses_; }

    void stop();

private:
    QSharedPointer<ClauseRecord> parent_;
    ClausePtr clause_;
    QList<ClausePtr> subclauses_;
};


ClauseRecord::ClauseRecord(ClausePtr clause)
    : clause_(clause)
{
    parent_ = sharedFromThis();
}


void ClauseRecord::stop()
{
    if (!subclauses_.isEmpty())
    {
        ClausePtr subclause;

        if (subclauses_.size() == 1)
        {
            subclause = subclauses_.at(0);
        }
        else
        {
            subclause.reset(new OrClause(subclauses_));
        }

        clause_.reset(new AndClause({ clause_, subclause }));
    }

    parent_->subclauses_.append(clause_);
    // TODO: current = current.parent;
}

}


class MimeTypesReaderData : public QSharedData
{
public:
    MimeTypes types;

    /** Current type */
    MimeType type;

    int priority;

    QString characters;

    QSharedPointer<ClauseRecord> current;
};


MimeTypesReader::MimeTypesReader(const MimeTypes &types)
    : data_(new MimeTypesReaderData)
{
    data_->types = types;
    data_->priority = 0;
}


MimeTypesReader::MimeTypesReader(const MimeTypesReader &rhs)
    : data_(rhs.data_)
{
}


MimeTypesReader &MimeTypesReader::operator=(const MimeTypesReader &rhs)
{
    if (this != &rhs)
    {
        data_.operator=(rhs.data_);
    }

    return *this;
}


MimeTypesReader::~MimeTypesReader()
{
}


void MimeTypesReader::read(QIODevice *stream)
{
}


bool MimeTypesReader::resolveEntity(const QString &publicId, const QString &systemId, QXmlInputSource *&ret)
{
    Q_UNUSED(publicId)
    Q_UNUSED(systemId)
    ret = new QXmlInputSource();
    return true;
}


bool MimeTypesReader::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    if (data_->type.isNull())
    {
        if (qName == MIME_TYPE_TAG())
        {
            QString name = atts.value(MIME_TYPE_TYPE_ATTR());
            data_->type = data_->types.forName(name);
        }
    }
    else if (qName == ALIAS_TAG())
    {
        QString alias = atts.value(ALIAS_TYPE_ATTR());
        data_->types.addAlias(data_->type, MediaType::parse(alias));
    }
    else if (qName == SUB_CLASS_OF_TAG())
    {
        QString parent = atts.value(SUB_CLASS_TYPE_ATTR());
        data_->types.setSuperType(data_->type, MediaType::parse(parent));
    }
    else if (qName == ACRONYM_TAG() || qName == COMMENT_TAG() || qName == TIKA_LINK_TAG()
             || qName == TIKA_UTI_TAG())
    {
        data_->characters.clear();
    }
    else if (qName == GLOB_TAG())
    {
        QString pattern = atts.value(PATTERN_ATTR());
        QString isRegex = atts.value(ISREGEX_ATTR());

        if (!pattern.isEmpty())
        {
            data_->types.addPattern(data_->type, pattern,
                                    isRegex.compare(QLatin1String("true"), Qt::CaseInsensitive));
        }
    }
    else if (qName == ROOT_XML_TAG())
    {
        QString ns = atts.value(NS_URI_ATTR());
        QString local = atts.value(LOCAL_NAME_ATTR());
        data_->type.addRootXML(ns, local);
    }
    else if (qName == MATCH_TAG())
    {
        QString kind = atts.value(MATCH_TYPE_ATTR());
        QString offset = atts.value(MATCH_OFFSET_ATTR());
        QString value = atts.value(MATCH_VALUE_ATTR());
        QString mask = atts.value(MATCH_MASK_ATTR());

        if (kind.isEmpty())
        {
            kind = "string";
        }

        data_->current.reset(new ClauseRecord(QSharedPointer<MagicMatch>::create(data_->type.type(), kind, offset, value, mask)));
    }
    else if (qName == MAGIC_TAG())
    {
        QString value = atts.value(MAGIC_PRIORITY_ATTR());

        if (value.isEmpty())
        {
            data_->priority = 50;
        }
        else
        {
            data_->priority = value.toInt();
        }

        data_->current.reset();
    }

    return true;
}


bool MimeTypesReader::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if (!data_->type.isNull())
    {
        if (qName == MIME_TYPE_TAG())
        {
            data_->type = MimeType();
        }
        else if (qName == COMMENT_TAG())
        {
            data_->type.setDescription(data_->characters.trimmed());
            data_->characters.clear();
        }
        else if (qName == ACRONYM_TAG())
        {
            data_->type.setAcronym(data_->characters.trimmed());
            data_->characters.clear();
        }
        else if (qName == TIKA_UTI_TAG())
        {
            data_->type.setUniformTypeIdentifier(data_->characters.trimmed());
            data_->characters.clear();
        }
        else if (qName == TIKA_LINK_TAG())
        {
            QUrl url(data_->characters.trimmed());

            if (!url.isValid())
            {
                throw std::invalid_argument("Unable to parse link.");
            }

            data_->type.addLink(url);
            data_->characters.clear();
        }
        else if (qName == MATCH_TAG())
        {
            data_->current->stop();
        }
        else if (qName == MAGIC_TAG())
        {
            for (auto clause : data_->current->clauses())
            {
                data_->type.addMagic(Magic(data_->type, data_->priority, clause));
            }

            data_->current.reset();
        }
    }

    return true;
}


bool MimeTypesReader::characters(const QString &ch)
{
    data_->characters.append(ch);
    return true;
}


}       // namespace mime

}       // namespace qtika
