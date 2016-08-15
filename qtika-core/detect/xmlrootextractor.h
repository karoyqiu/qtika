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

#include <QString>

class QIODevice;


namespace qtika {

namespace detect {


struct QName
{
    QString namespaceURI;
    QString localName;

    bool isNull() const { return namespaceURI.isNull() && localName.isNull(); }
};


/**
 * Utility class that uses a {@link javax.xml.parsers.SAXParser} to determine
 * the namespace URI and local name of the root element of an XML file.
 *
 * @since Apache Tika 0.4
 */
class XmlRootExtractor
{
public:
    /**
     * @since Apache Tika 0.9
     */
    static QName extractRootElement(QIODevice *stream);

    static QName extractRootElement(const QByteArray &data);
};


}       // namespace detect

}       // namespace qtika
