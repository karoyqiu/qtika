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
#include "patterns.h"

#include <map>
#include <limits>
#include <QRegularExpression>

#include "mediatype.h"
#include "mediatyperegistry.h"
#include "mimetypeexception.h"


namespace qtika {

namespace mime {

namespace {

struct Shorter
{
    bool operator()(const QRegularExpression &a, const QRegularExpression &b) const
    {
        return a.pattern().length() < b.pattern().length();
    }
};

}


class PatternsData : public QSharedData
{
public:
    PatternsData();

    void addName(const QString &name, const MimeType &type);

    void addExtension(const QString &extension, const MimeType &type);

    void addGlob(const QString &glob, const MimeType &type);

    static QString compile(const QString &glob);

public:
    MediaTypeRegistry *registry;

    /**
     * Index of exact name patterns.
     */
    QHash<QString, MimeType> names;

    /**
     * Index of extension patterns of the form "*extension".
     */
    QHash<QString, MimeType> extensions;

    int minExtensionLength;

    int maxExtensionLength;

    /**
     * Index of generic glob patterns, sorted by length.
     */
    std::map<QRegularExpression, MimeType, Shorter> globs;
};


PatternsData::PatternsData()
    : registry(Q_NULLPTR)
    , minExtensionLength(std::numeric_limits<int>::max())
    , maxExtensionLength(0)
{
}


void PatternsData::addName(const QString &name, const MimeType &type)
{
    MimeType previous = names.value(name);

    if (previous.isNull() || registry->isSpecializationOf(previous.type(), type.type()))
    {
        names.insert(name, type);
    }
    else if (previous == type || registry->isSpecializationOf(type.type(), previous.type()))
    {
        // do nothing
    }
    else
    {
        QString msg = "Conflicting name pattern: " % name;
        throw MimeTypeException(msg.toStdString().c_str());
    }
}


void PatternsData::addExtension(const QString &extension, const MimeType &type)
{
    MimeType previous = extensions.value(extension);

    if (previous.isNull() || registry->isSpecializationOf(previous.type(), type.type()))
    {
        extensions.insert(extension, type);
        minExtensionLength = qMin(minExtensionLength, extension.length());
        maxExtensionLength = qMax(maxExtensionLength, extension.length());
    }
    else if (previous == type || registry->isSpecializationOf(type.type(), previous.type()))
    {
        // do nothing
    }
    else
    {
        QString msg = "Conflicting extension pattern: " % extension;
        throw MimeTypeException(msg.toStdString().c_str());
    }
}


void PatternsData::addGlob(const QString &glob, const MimeType &type)
{
    QRegularExpression regexp(glob);
    auto iter = globs.find(regexp);
    MimeType previous;

    if (iter != globs.end())
    {
        previous = iter->second;
    }

    if (previous.isNull() || registry->isSpecializationOf(previous.type(), type.type()))
    {
        globs.emplace(regexp, type);
    }
    else if (previous == type || registry->isSpecializationOf(type.type(), previous.type()))
    {
        // do nothing
    }
    else
    {
        QString msg = "Conflicting glob pattern: " % glob;
        throw MimeTypeException(msg.toStdString().c_str());
    }
}


QString PatternsData::compile(const QString &glob)
{
    static const QString specials("\\[]^.-$+(){}|");

    QString pattern("^");

    for (int i = 0; i < glob.length(); i++)
    {
        QChar ch = glob.at(i);

        if (ch == '?')
        {
            pattern.append('.');
        }
        else if (ch == '*')
        {
            pattern.append(".*");
        }
        else if (specials.contains(ch))
        {
            pattern.append('\\');
            pattern.append(ch);
        }
        else
        {
            pattern.append(ch);
        }
    }

    pattern.append('$');
    return pattern;
}


Patterns::Patterns()
    : data(new PatternsData)
{
}


Patterns::Patterns(MediaTypeRegistry *registry)
    : Patterns()
{
    data->registry = registry;
}


Patterns::Patterns(const Patterns &rhs)
    : data(rhs.data)
{
}


Patterns &Patterns::operator=(const Patterns &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


Patterns::~Patterns()
{
}


void Patterns::add(const QString &pattern, const MimeType &type)
{
    add(pattern, false, type);
}


void Patterns::add(const QString &pattern, bool isRegex, const MimeType &type)
{
    if (pattern.isEmpty() || type.isNull())
    {
        throw std::invalid_argument("Pattern and/or mime type is missing.");
    }

    if (isRegex)
    {
        // in this case, we don't need to build a regex pattern
        // it's already there for us, so just add the pattern as is
        data->addGlob(pattern, type);
    }
    else
    {
        if (pattern.indexOf('*') == -1 && pattern.indexOf('?') == -1
                && pattern.indexOf('[') == -1)
        {
            data->addName(pattern, type);
        }
        else if (pattern.startsWith('*') && pattern.indexOf('*', 1) == -1
                 && pattern.indexOf('?') == -1 && pattern.indexOf('[') == -1)
        {
            QString extension = pattern.mid(1);
            data->addExtension(extension, type);
            //type.addExtension(extension);   // ???
        }
        else
        {
            data->addGlob(PatternsData::compile(pattern), type);
        }
    }
}


MimeType Patterns::matches(const QString &name) const
{
    if (name.isEmpty())
    {
        throw std::invalid_argument("Name is missing.");
    }

    // First, try exact match of the provided resource name
    if (data->names.contains(name))
    {
        return data->names.value(name);
    }

    // Then try "extension" (*.xxx) matching
    int maxLength = qMin(data->maxExtensionLength, name.length());

    for (int n = maxLength; n >= data->minExtensionLength; n--)
    {
        QString extension = name.mid(name.length() - n);

        if (data->extensions.contains(extension))
        {
            return data->extensions.value(extension);
        }
    }

    // And finally, try complex regexp matching
    for (auto iter = data->globs.begin(); iter != data->globs.end(); ++iter)
    {
        auto match = iter->first.match(name);

        if (match.hasMatch())
        {
            return iter->second;
        }
    }

    return MimeType();
}


}       // namespace mime

}       // namespace qtika
