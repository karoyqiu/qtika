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
#include "mimetypes.h"

#include <QBuffer>
#include <QMutex>
#include <QUrl>

#include <qiodevicetransactionrollback.h>

#include "detect/textdetector.h"
#include "detect/xmlrootextractor.h"
#include "magic.h"
#include "mediatyperegistry.h"
#include "metadata/metadata.h"
#include "patterns.h"


namespace qtika {

namespace mime {


class MimeTypesData : public QSharedData
{
public:
    /**
     * Use the MimeType hint to try to clarify or specialise the current
     *  possible types list.
     * If the hint is a specialised form, use that instead
     * If there are multiple possible types, use the hint to select one
     */
    QList<MimeType> applyHint(const QList<MimeType> &possibleTypes, const MimeType &hint) const;

public:
    /**
     * Name of the {@link #rootMimeType root} type, application/octet-stream.
     */
    static const QString OCTET_STREAM;

    /**
     * Name of the {@link #textMimeType text} type, text/plain.
     */
    static const QString PLAIN_TEXT;

    /**
     * Name of the {@link #xmlMimeType xml} type, application/xml.
     */
    static const QString XML;

    /**
     * Root type, application/octet-stream.
     */
    MimeType rootMimeType;
    QList<MimeType> rootMimeTypes;

    /**
     * Text type, text/plain.
     */
    MimeType textMimeType;

    /*
     * xml type, application/xml
     */
    MimeType xmlMimeType;

    /**
     * Registered media types and their aliases.
     */
    MediaTypeRegistry registry;

    /** All the registered MimeTypes indexed on their canonical names */
    QHash<MediaType, MimeType> types;

    /** The patterns matcher */
    Patterns patterns;

    /** Sorted list of all registered magics */
    QList<Magic> magics;

    /** Sorted list of all registered rootXML */
    QList<MimeType> xmls;

    //QMutex *mutex;
};


const QString MimeTypesData::OCTET_STREAM("application/octet-stream");
const QString MimeTypesData::PLAIN_TEXT("text/plain");
const QString MimeTypesData::XML("application/xml");


QList<MimeType> MimeTypesData::applyHint(const QList<MimeType> &possibleTypes, const MimeType &hint) const
{
    QList<MimeType> result;

    if (possibleTypes.isEmpty())
    {
        result.append(hint);
        return result;
    }

    for (const MimeType &type : possibleTypes)
    {
        if (hint == type || registry.isSpecializationOf(hint.type(), type.type()))
        {
            // Use just this type
            result.append(hint);
            return result;
        }
    }

    // Hint didn't help, sorry
    return possibleTypes;
}


MimeTypes::MimeTypes()
    : data(new MimeTypesData)
{
    data->rootMimeType = MediaType::OCTET_STREAM();
    data->textMimeType = MediaType::TEXT_PLAIN();
    data->xmlMimeType = MediaType::APPLICATION_XML();

    data->rootMimeTypes.append(data->rootMimeType);

    add(data->rootMimeType);
    add(data->textMimeType);
    add(data->xmlMimeType);
}


MimeTypes::MimeTypes(const MimeTypes &rhs)
    : data(rhs.data)
{
}


MimeTypes &MimeTypes::operator=(const MimeTypes &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


MimeTypes::~MimeTypes()
{
}


void MimeTypes::init()
{
    for (const MimeType &type : data->types.values())
    {
        data->magics.append(type.magics());

        if (type.hasRootXML())
        {
            data->xmls.append(type);
        }
    }
}


QList<MimeType> MimeTypes::getMimeType(const QByteArray &d)
{
    if (d.isNull())
    {
        throw std::invalid_argument("Data is missing.");
    }

    if (d.isEmpty())
    {
        // See https://issues.apache.org/jira/browse/TIKA-483
        return data->rootMimeTypes;
    }

    // Then, check for magic bytes
    QList<MimeType> result;
    int currentPriority = -1;

    for (const Magic &magic : data->magics)
    {
        if (currentPriority > 0 && currentPriority > magic.priority())
        {
            break;
        }

        if (magic.eval(d))
        {
            result.append(magic.type());
            currentPriority = magic.priority();
        }
    }

    if (!result.isEmpty())
    {
        for (int i = 0; i < result.size(); i++)
        {
            MimeType matched = result.at(i);

            // When detecting generic XML (or possibly XHTML),
            // extract the root element and match it against known types
            if (matched.name() == "application/xml" || matched.name() == "text/html")
            {
                auto rootElement = qtika::detect::XmlRootExtractor::extractRootElement(d);

                if (!rootElement.isNull())
                {
                    for (const MimeType &type : data->xmls)
                    {
                        if (type.matchesXML(rootElement.namespaceURI, rootElement.localName))
                        {
                            result[i] = type;
                            break;
                        }
                    }
                }
                else if (matched.name() == "application/xml")
                {
                    // Downgrade from application/xml to text/plain since
                    // the document seems not to be well-formed.
                    result[i] = data->textMimeType;
                }
            }
        }

        return result;
    }

    // Finally, assume plain text if no control bytes are found
    try
    {
        qtika::detect::TextDetector detector(minLength());
        QBuffer buffer;
        buffer.setData(d);
        buffer.open(QIODevice::ReadOnly);
        MimeType type = forName(detector.detect(&buffer, Metadata()).toString());
        result.append(type);
        return result;
    }
    catch (const std::exception &)
    {
        return data->rootMimeTypes;
    }
}


MimeType MimeTypes::getMimeType(const QString &name) const
{
    MimeType type = data->patterns.matches(name);

    if (!type.isNull())
    {
        return type;
    }

    type = data->patterns.matches(name.toLower());

    if (!type.isNull())
    {
        return type;
    }

    return data->rootMimeType;
}


MimeType MimeTypes::getMimeType(const QFileInfo &file) const
{
    // UNDONE: return forName(new Tika(this).detect(file));
    throw std::exception("Not implemented.");
}


MimeType MimeTypes::getRegisteredMimeType(const QString &name) const
{
    MediaType type = MediaType::parse(name);

    if (type.isNull())
    {
        // TODO: throw MimeTypeException
        std::string msg("Invalid media type name: ");
        msg += name.toStdString();
        throw std::invalid_argument(msg);
    }

    MediaType normalized = data->registry.normalize(type);
    MimeType candidate = data->types.value(normalized);

    if (!candidate.isNull())
    {
        return candidate;
    }

    if (normalized.hasParameters())
    {
        return data->types.value(normalized.baseType());
    }

    return MediaType();
}


void MimeTypes::setSuperType(const MimeType &type, const detect::Detector::MediaType &parent)
{
    data->registry.addSuperType(type.type(), parent);
}


void MimeTypes::addAlias(const MimeType &type, const detect::Detector::MediaType &alias)
{
    data->registry.addAlias(type.type(), alias);
}


void MimeTypes::addPattern(const MimeType &type, const QString &pattern, bool isRegex)
{
    data->patterns.add(pattern, isRegex, type);
}


MediaTypeRegistry MimeTypes::mediaTypeRegistry() const
{
    return data->registry;
}


void MimeTypes::add(const MimeType &type)
{
    data->registry.addType(type.type());
    data->types.insert(type.type(), type);

    // Update the magics index...
    if (type.hasMagic())
    {
        data->magics.append(type.magics());
    }

    // Update the xml (xmlRoot) index...
    if (type.hasRootXML())
    {
        data->xmls.append(type);
    }
}


int MimeTypes::minLength()
{
    // This needs to be reasonably large to be able to correctly detect
    // things like XML root elements after initial comment and DTDs
    return 64 * 1024;
}


MimeType MimeTypes::forName(const QString &name)
{
    MediaType type = MediaType::parse(name);

    if (type.isNull())
    {
        // TODO: throw MimeTypeException
        std::string msg("Invalid media type name: ");
        msg += name.toStdString();
        throw std::runtime_error(msg);
    }

    MediaType normalized = data->registry.normalize(type);
    MimeType mime = data->types.value(normalized);

    if (mime.isNull())
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);

        // Double check it didn't already get added while
        //  we were waiting for the lock
        mime = data->types.value(normalized);

        if (mime.isNull())
        {
            mime = normalized;
            add(mime);
            data->types.insert(type, mime);
        }
    }

    return mime;
}


QByteArray MimeTypes::readMagicHeader(QIODevice *stream)
{
    if (stream == Q_NULLPTR)
    {
        throw std::invalid_argument("InputStream is missing.");
    }

    QByteArray bytes(minLength(), 0);
    int totalRead = 0;
    int lastRead = stream->read(bytes.data(), bytes.length());

    while (lastRead > 0)
    {
        totalRead += lastRead;

        if (totalRead == bytes.length())
        {
            return bytes;
        }

        lastRead = stream->read(bytes.data() + totalRead, bytes.length() - totalRead);
    }

    bytes.resize(totalRead);
    return bytes;
}


MediaType MimeTypes::detect(QIODevice *input, const Metadata &meta) const
{
    QList<MimeType> possibleTypes;

    // Get type based on magic prefix
    if (input != Q_NULLPTR)
    {
        internal::QIODeviceTransactionRollback rollback(input);
        QByteArray prefix = readMagicHeader(input);
        possibleTypes = const_cast<MimeTypes *>(this)->getMimeType(prefix);
    }

    // Get type based on resourceName hint (if available)
    QString resourceName = meta.get(Metadata::RESOURCE_NAME_KEY());

    if (!resourceName.isEmpty())
    {
        QString name;

        // Deal with a URI or a path name in as the resource name
        QUrl url = QUrl::fromUserInput(resourceName);

        if (url.isValid())
        {
            QString path = url.path();

            if (!path.isEmpty())
            {
                int slash = path.lastIndexOf('/');

                if (slash + 1 < path.length())
                {
                    name = path.mid(slash + 1);
                }
            }
        }
        else
        {
            name = resourceName;
        }

        if (!name.isEmpty())
        {
            MimeType hint = getMimeType(name);

            // If we have some types based on mime magic, try to specialise
            //  and/or select the type based on that
            // Otherwise, use the type identified from the name
            possibleTypes = data->applyHint(possibleTypes, hint);
        }
    }

    // Get type based on metadata hint (if available)
    QString typeName = meta.get(Metadata::CONTENT_TYPE());

    if (!typeName.isEmpty())
    {
        MimeType hint = const_cast<MimeTypes *>(this)->forName(typeName);
        possibleTypes = data->applyHint(possibleTypes, hint);
    }

    if (possibleTypes.isEmpty())
    {
        // Report that we don't know what it is
        return MediaType::OCTET_STREAM();
    }

    return possibleTypes.first().type();
}


}       // namespace mime

}       // namespace qtika
