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

#include "detect/textdetector.h"
#include "detect/xmlrootextractor.h"
#include "magic.h"
#include "mediatyperegistry.h"
#include "patterns.h"


namespace qtika {

namespace mime {


class MimeTypesData : public QSharedData
{
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
};


const QString MimeTypesData::OCTET_STREAM("application/octet-stream");
const QString MimeTypesData::PLAIN_TEXT("text/plain");
const QString MimeTypesData::XML("application/xml");


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


QList<MimeType> MimeTypes::getMimeType(const QByteArray &d) const
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
        // UNDONE: TextDetector detector = new TextDetector(getMinLength());
        //qtika::detect::TextDetector detector(min)
    }
    catch (const std::exception &)
    {
        return data->rootMimeTypes;
    }
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


}       // namespace mime

}       // namespace qtika
