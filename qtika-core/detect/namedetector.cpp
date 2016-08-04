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
#include "namedetector.h"

#include <QUrl>

#include "metadata/metadata.h"


namespace qtika {

namespace detect {


class NameDetectorData : public QSharedData
{
public:
    /**
     * The regular expression patterns used for type detection.
     */
    NameDetector::PatternMap patterns;
};


NameDetector::NameDetector(const PatternMap &patterns)
    : data(new NameDetectorData)
{
    data->patterns = patterns;
}


NameDetector::NameDetector(const NameDetector &rhs)
    : data(rhs.data)
{
}


NameDetector &NameDetector::operator=(const NameDetector &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


NameDetector::~NameDetector()
{
}


Detector::MediaType NameDetector::detect(QIODevice *input, const Metadata &meta)
{
    Q_UNUSED(input)

    // Look for a resource name in the input metadata
    QString name = meta.get(Metadata::RESOURCE_NAME_KEY());

    if (!name.isEmpty())
    {
        // If the name is a URL, skip the trailing query and fragment parts
        int pos = name.indexOf('?');

        if (pos != -1)
        {
            name = name.left(pos);
        }

        pos = name.indexOf('#');

        if (pos != -1)
        {
            name = name.left(pos);
        }

        // If the name is a URL or a path, skip all but the last component
        pos = name.lastIndexOf('/');

        if (pos != -1)
        {
            name = name.mid(pos + 1);
        }

        pos = name.lastIndexOf('\\');

        if (pos != -1)
        {
            name = name.mid(pos + 1);
        }

        // Decode any potential URL encoding
        pos = name.indexOf('%');

        if (pos != -1)
        {
            name = QUrl::fromPercentEncoding(name.toUtf8());
        }

        // Skip any leading or trailing whitespace
        name = name.trimmed();

        if (!name.isEmpty())
        {
            // Match the name against the registered patterns
            for (auto reg : data->patterns.keys())
            {
                auto match = reg.match(name);

                if (match.hasMatch())
                {
                    return data->patterns.value(reg);
                }
            }
        }
    }

    return MediaType::OCTET_STREAM();
}


}       // namespace detect

}       // namespace qtika
