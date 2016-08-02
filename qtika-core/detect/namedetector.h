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

#include <QRegularExpression>
#include <QSharedDataPointer>


namespace qtika {

namespace detect {


class NameDetectorData;


/**
 * Content type detection based on the resource name. An instance of this
 * class contains a set of regular expression patterns that are matched
 * against the resource name potentially given as a part of the input metadata.
 * <p>
 * If a pattern matches the given name, then the media type associated with
 * that pattern is returned as the likely content type of the input document.
 * Otherwise the returned type is <code>application/octet-stream</code>.
 * <p>
 * See the {@link #detect(InputStream, Metadata)} method for more details
 * of the matching algorithm.
 *
 * @since Apache Tika 0.3
 */
class NameDetector : public Detector
{
public:
    typedef QHash<QRegularExpression, MediaType> PatternMap;

    /**
     * Creates a new content type detector based on the given name patterns.
     * The given pattern map is not copied, so the caller may update the
     * mappings even after this detector instance has been created. However,
     * the map <em>must not be concurrently modified</em> while this instance
     * is used for type detection.
     *
     * @param patterns map from name patterns to corresponding media types
     */
    explicit NameDetector(const PatternMap &patterns);
    NameDetector(const NameDetector &);
    NameDetector &operator=(const NameDetector &);
    virtual ~NameDetector();

    /**
     * Detects the content type of an input document based on the document
     * name given in the input metadata. The RESOURCE_NAME_KEY attribute of
     * the given input metadata is expected to contain the name (normally
     * a file name or a URL) of the input document.
     * <p>
     * If a resource name is given, then it is first processed as follows.
     * <ol>
     *   <li>
     *     Potential URL query (?...) and fragment identifier (#...)
     *     parts are removed from the end of the resource name.
     *   </li>
     *   <li>
     *     Potential leading path elements (up to the last slash or backslash)
     *     are removed from the beginning of the resource name.
     *   </li>
     *   <li>
     *     Potential URL encodings (%nn, in UTF-8) are decoded.
     *   </li>
     *   <li>
     *     Any leading and trailing whitespace is removed.
     *   </li>
     * </ol>
     * <p>
     * The resulting name string (if any) is then matched in sequence against
     * all the configured name patterns. If a match is found, then the (first)
     * matching media type is returned.
     *
     * @param input ignored
     * @param metadata input metadata, possibly with a RESOURCE_NAME_KEY value
     * @return detected media type, or <code>application/octet-stream</code>
     */
    virtual MediaType detect(QIODevice *input, const Metadata &meta) Q_DECL_OVERRIDE;

private:
    QSharedDataPointer<NameDetectorData> data;
};


}       // namespace detect

}       // namespace qtika
