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

namespace detect {


class TextDetectorData;


/**
 * Content type detection of plain text documents. This detector looks at the
 * beginning of the document input stream and considers the document to be
 * a text document if no ASCII (ISO-Latin-1, UTF-8, etc.) control bytes are
 * found. As a special case some control bytes (up to 2% of all characters)
 * are also allowed in a text document if it also contains no or just a few
 * (less than 10%) characters above the 7-bit ASCII range.
 * <p>
 * Note that text documents with a character encoding like UTF-16 are better
 * detected with {@link MagicDetector} and an appropriate magic byte pattern.
 *
 * @since Apache Tika 0.3
 */
class QTIKACORESHARED_EXPORT TextDetector : public Detector
{
public:
    /**
     * Constructs a {@link TextDetector} which will look at a given number of
     * bytes from the beginning of the document.
     */
    explicit TextDetector(int bytesToTest = DEFAULT_NUMBER_OF_BYTES_TO_TEST);
    TextDetector(const TextDetector &);
    TextDetector &operator=(const TextDetector &);
    virtual ~TextDetector();

    /**
     * Looks at the beginning of the document input stream to determine
     * whether the document is text or not.
     *
     * @param input document input stream, or <code>null</code>
     * @param meta ignored
     * @return "text/plain" if the input stream suggest a text document,
     *         "application/octet-stream" otherwise
     */
    virtual MediaType detect(QIODevice *input, const Metadata &meta) Q_DECL_OVERRIDE;

public:
    /**
     * The number of bytes from the beginning of the document stream
     * to test for control bytes.
     */
    static const int DEFAULT_NUMBER_OF_BYTES_TO_TEST;

private:
    QSharedDataPointer<TextDetectorData> data;
};


}       // namespace detect

}       // namespace qtika
