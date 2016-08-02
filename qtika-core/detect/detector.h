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
#include "mime/mediatype.h"

class QIODevice;


namespace qtika {

namespace metadata { class Metadata; }

namespace detect {


/**
 * Content type detector. Implementations of this interface use various
 * heuristics to detect the content type of a document based on given
 * input metadata or the first few bytes of the document stream.
 *
 * @since Apache Tika 0.3
 */
class QTIKACORESHARED_EXPORT Detector
{
public:
    typedef mime::MediaType MediaType;
    typedef metadata::Metadata Metadata;

    virtual ~Detector() Q_DECL_EQ_DEFAULT;

    /**
     * Detects the content type of the given input document. Returns
     * <code>application/octet-stream</code> if the type of the document
     * can not be detected.
     * <p>
     * If the document input stream is not available, then the first
     * argument may be <code>null</code>. Otherwise the detector may
     * read bytes from the start of the stream to help in type detection.
     * The given stream is guaranteed to support the
     * {@link InputStream#markSupported() mark feature} and the detector
     * is expected to {@link InputStream#mark(int) mark} the stream before
     * reading any bytes from it, and to {@link InputStream#reset() reset}
     * the stream before returning. The stream must not be closed by the
     * detector.
     * <p>
     * The given input metadata is only read, not modified, by the detector.
     *
     * @param input document input stream, or <code>null</code>
     * @param meta input metadata for the document
     * @return detected media type, or <code>application/octet-stream</code>
     * @throws IOException if the document input stream could not be read
     */
    virtual MediaType detect(QIODevice *input, const Metadata &meta) = 0;
};


}       // namespace detect

}       // namespace qtika
