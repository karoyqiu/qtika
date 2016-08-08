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

#include "qtika-core-global.h"
#include "detector.h"

#include <QSharedDataPointer>


namespace qtika {

namespace mime { class MediaType; }

namespace detect {


class MagicDetectorData;


/**
 * Content type detection based on magic bytes, i.e. type-specific patterns
 * near the beginning of the document input stream.
 *
 * Because this works on bytes, not characters, by default any string
 *  matching is done as ISO_8859_1. To use an explicit different
 *  encoding, supply a type other than "string" / "stringignorecase"
 *
 * @since Apache Tika 0.3
 */
class QTIKACORESHARED_EXPORT MagicDetector : public Detector
{
public:
    /**
     * Creates a detector for input documents that meet the specified
     * magic match.
     */
    MagicDetector(const mime::MediaType &type, const QByteArray &pattern, const QByteArray &mask,
                  bool isRegex, bool isStringIgnoreCase, int offsetRangeBegin, int offsetRangeEnd);

    /**
     * Creates a detector for input documents that meet the specified
     * magic match.
     */
    MagicDetector(const mime::MediaType &type, const QByteArray &pattern, const QByteArray &mask,
                  bool isRegex, int offsetRangeBegin, int offsetRangeEnd);

    /**
     * Creates a detector for input documents that have the exact given byte
     * pattern at the given offset of the document stream.
     *
     * @param type matching media type
     * @param pattern magic match pattern
     * @param offset offset of the pattern match
     */
    MagicDetector(const mime::MediaType &type, const QByteArray &pattern, int offset = 0);

    MagicDetector(const MagicDetector &);
    MagicDetector &operator=(const MagicDetector &);
    virtual ~MagicDetector();

    int length() const;

    /**
     * Returns a string representation of the Detection Rule.
     * Should sort nicely by type and details, as we sometimes
     * compare these.
     */
    QString toString() const;

private:
    QSharedDataPointer<MagicDetectorData> data;
};


}       // namespace detect

}       // namespace qtika
