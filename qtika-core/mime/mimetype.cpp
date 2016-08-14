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
#include "mimetype.h"

#include <QUrl>

#include "magic.h"
#include "mediatype.h"


namespace qtika {

namespace mime {

namespace {

/**
 * Defines a RootXML description. RootXML is made of a localName and/or a
 * namespaceURI.
 */
class RootXML
{
public:
    RootXML(const MimeType &type, const QString &nsURI, const QString &local);

    bool matches(const QString &nsURI, const QString &local) const;

    MimeType type() const
    {
        return type_;
    }

    QString namespaceURI() const
    {
        return namespaceURI_;
    }

    QString localName() const
    {
        return localName_;
    }

    QString toString() const
    {
        // return type_.tostring ...
        return ", " % namespaceURI_ % ", " % localName_;
    }

private:
    MimeType type_;
    QString namespaceURI_;
    QString localName_;

};


RootXML::RootXML(const MimeType &type, const QString &nsURI, const QString &local)
    : type_(type)
    , namespaceURI_(nsURI)
    , localName_(local)
{
    if (nsURI.isEmpty() && local.isEmpty())
    {
        throw std::invalid_argument("Both namespaceURI and localName cannot be empty.");
    }
}


bool RootXML::matches(const QString &nsURI, const QString &local) const
{
    return namespaceURI_ == nsURI && localName_ == local;
}

}


class MimeTypeData : public QSharedData
{
public:
    MimeTypeData();

    /**
     * The normalized media type name.
     */
    MediaType type;

    /**
     * The MimeType acronym
     */
    QString acronym;

    /**
     * The http://en.wikipedia.org/wiki/Uniform_Type_Identifier
     */
    QString uti;

    /**
     * Documentation Links
     */
    QList<QUrl> links;

    /**
     * Description of this media type.
     */
    QString description;

    /** The magics associated to this Mime-Type */
    QList<Magic> magics;

    /** The root-XML associated to this Mime-Type */
    QList<RootXML> rootXML;

    /** The minimum length of data to provides for magic analyzis */
    int minLength = 0;

    /**
     * All known file extensions of this type, in order of preference
     * (best first).
     */
    QStringList extensions;
};


MimeTypeData::MimeTypeData()
    : minLength(0)
{
}


MimeType::MimeType()
    : data(new MimeTypeData)
{
}


MimeType::MimeType(const MediaType &type)
    : MimeType()
{
    if (type.isNull())
    {
        throw std::invalid_argument("Media type name is missing.");
    }

    data->type = type;
}


MimeType::MimeType(const MimeType &rhs) : data(rhs.data)
{
}


MimeType &MimeType::operator=(const MimeType &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


MimeType::~MimeType()
{
}


bool MimeType::isNull() const
{
    return data->type.isNull();
}


MediaType MimeType::type() const
{
    return data->type;
}


QString MimeType::name() const
{
    return data->type.toString();
}


QString MimeType::description() const
{
    return data->description;
}


void MimeType::setDescription(const QString &desc)
{
    if (desc.isNull())
    {
        throw std::invalid_argument("Description is missing.");
    }

    data->description = desc;
}


QString MimeType::acronym() const
{
    return data->acronym;
}


void MimeType::setAcronym(const QString &a)
{
    if (a.isNull())
    {
        throw std::invalid_argument("Acronym is missing.");
    }

    data->acronym = a;
}


QString MimeType::uniformTypeIdentifier() const
{
    return data->uti;
}


void MimeType::setUniformTypeIdentifier(const QString &uti)
{
    if (uti.isNull())
    {
        throw std::invalid_argument("Uniform Type Identifier is missing.");
    }

    data->uti = uti;
}


QList<QUrl> MimeType::links() const
{
    return data->links;
}


void MimeType::addLink(const QUrl &link)
{
    if (!link.isValid())
    {
        throw std::invalid_argument("Invalid link.");
    }

    data->links.append(link);
}


void MimeType::addRootXML(const QString &namespaceURI, const QString &localName)
{
    data->rootXML.append(RootXML(*this, namespaceURI, localName));
}


bool MimeType::matchesXML(const QString &namespaceURI, const QString &localName) const
{
    for (const RootXML &xml : data->rootXML)
    {
        if (xml.matches(namespaceURI, localName))
        {
            return true;
        }
    }

    return false;
}


bool MimeType::hasRootXML() const
{
    return !data->rootXML.isEmpty();
}


QList<Magic> MimeType::magics() const
{
    return data->magics;
}


void MimeType::addMagic(const Magic &magic)
{
    data->magics.append(magic);
}


int MimeType::minLength() const
{
    return data->minLength;
}


bool MimeType::hasMagic() const
{
    return !data->magics.isEmpty();
}


bool MimeType::matchesMagic(const QByteArray &d) const
{
    for (const Magic &m : data->magics)
    {
        if (m.eval(d))
        {
            return true;
        }
    }

    return false;
}


bool MimeType::operator<(const MimeType &rhs) const
{
    return data->type < rhs.data->type;
}


QString MimeType::toString() const
{
    return data->type.toString();
}


QString MimeType::extension() const
{
    if (data->extensions.isEmpty())
    {
        return QString();
    }

    return data->extensions.first();
}


QStringList MimeType::extensions() const
{
    return data->extensions;
}


void MimeType::addExtension(const QString &ext)
{
    data->extensions.append(ext);
}


bool MimeType::isValid(const QString &name)
{
    if (name.isEmpty())
    {
        throw std::invalid_argument("Name is missing.");
    }

    bool slash = false;

    for (int i = 0; i < name.length(); i++)
    {
        QChar ch = name.at(i);

        if (ch <= ' ' || ch >= 127 || ch == '(' || ch == ')' ||
                ch == '<' || ch == '>' || ch == '@' || ch == ',' ||
                ch == ';' || ch == ':' || ch == '\\' || ch == '"' ||
                ch == '[' || ch == ']' || ch == '?' || ch == '=')
        {
            return false;
        }
        else if (ch == '/')
        {
            if (slash || i == 0 || i + 1 == name.length())
            {
                return false;
            }

            slash = true;
        }
    }

    return slash;
}


bool operator==(const MimeType &lhs, const MimeType &rhs)
{
    return lhs.type() == rhs.type();
}


bool operator!=(const MimeType &lhs, const MimeType &rhs)
{
    return !(lhs == rhs);
}


uint qHash(const MimeType &m, uint seed)
{
    return qHash(m.type(), seed);
}


}       // namespace mime

}       // namespace qtika
