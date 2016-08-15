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
#include "detect/detector.h"
#include "mimetype.h"

#include <QSharedDataPointer>


namespace qtika {

namespace mime {


class MimeType;
class MimeTypesData;


/**
 * This class is a MimeType repository. It gathers a set of MimeTypes and
 * enables to retrieves a content-type from its name, from a file name, or from
 * a magic character sequence.
 * <p>
 * The MIME type detection methods that take an {@link InputStream} as
 * an argument will never reads more than {@link #getMinLength()} bytes
 * from the stream. Also the given stream is never
 * {@link InputStream#close() closed}, {@link InputStream#mark(int) marked},
 * or {@link InputStream#reset() reset} by the methods. Thus a client can
 * use the {@link InputStream#markSupported() mark feature} of the stream
 * (if available) to restore the stream back to the state it was before type
 * detection if it wants to process the stream based on the detected type.
 */
class QTIKACORESHARED_EXPORT MimeTypes Q_DECL_FINAL : public detect::Detector
{
public:
    MimeTypes();
    MimeTypes(const MimeTypes &);
    MimeTypes &operator=(const MimeTypes &);
    virtual ~MimeTypes();

    /**
     * Returns the MIME type that best matches the given first few bytes
     * of a document stream. Returns application/octet-stream if no better
     * match is found.
     * <p>
     * If multiple matches are found, the best (highest priority) matching
     * type is returned. If multiple matches are found with the same priority,
     * then all of these are returned.
     * <p>
     * The given byte array is expected to be at least {@link #getMinLength()}
     * long, or shorter only if the document stream itself is shorter.
     *
     * @param data first few bytes of a document stream
     * @return matching MIME type
     */
    QList<MimeType> getMimeType(const QByteArray &d) const;

    /**
     * Add the specified mime-type in the repository.
     *
     * @param type
     *            is the mime-type to add.
     */
    void add(const MimeType &type);

private:
    QSharedDataPointer<MimeTypesData> data;
};


}       // namespace mime

}       // namespace qtika
