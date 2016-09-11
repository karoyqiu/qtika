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
#include "xmlrootextractor.h"

#include <QBuffer>
#include <QXmlDefaultHandler>
#include <QXmlInputSource>
#include <QXmlSimpleReader>


namespace qtika {

namespace detect {

namespace {

class ExtractorHandler : public QXmlDefaultHandler
{
public:
    QName rootElement() const { return rootElement_; }

    virtual bool startElement(const QString &namespaceURI, const QString &localName,
                              const QString &qName, const QXmlAttributes &atts) Q_DECL_OVERRIDE;

private:
    QName rootElement_;
};


bool ExtractorHandler::startElement(const QString &namespaceURI, const QString &localName,
                                    const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(qName)
    Q_UNUSED(atts)

    rootElement_.namespaceURI = namespaceURI;
    rootElement_.localName = localName;
    return false;
}

}


QName XmlRootExtractor::extractRootElement(QIODevice *stream)
{
    QXmlSimpleReader reader;
    reader.setFeature(QS("http://xml.org/sax/features/namespaces"), true);

    ExtractorHandler handler;
    reader.setContentHandler(&handler);

    QXmlInputSource source(stream);
    reader.parse(source);

    return handler.rootElement();
}


QName XmlRootExtractor::extractRootElement(const QByteArray &data)
{
    QByteArray dup = data;
    QBuffer buffer(&dup);
    buffer.open(QIODevice::ReadOnly);
    return extractRootElement(&buffer);
}


}       // namespace detect

}       // namespace qtika
