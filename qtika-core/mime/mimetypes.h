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
#include "mediatyperegistry.h"

#include <QSharedDataPointer>

class QFileInfo;


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
     * Called after all configured types have been loaded.
     * Initializes the magics and xmls sets.
     */
    void init();

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
    QList<MimeType> getMimeType(const QByteArray &d);

    /**
     * Find the Mime Content Type of a document from its name.
     * Returns application/octet-stream if no better match is found.
     *
     * @deprecated Use {@link Tika#detect(String)} instead
     * @param name of the document to analyze.
     * @return the Mime Content Type of the specified document name
     */
    Q_DECL_DEPRECATED MimeType getMimeType(const QString &name) const;

    /**
     * Find the Mime Content Type of a document stored in the given file.
     * Returns application/octet-stream if no better match is found.
     *
     * @deprecated Use {@link Tika#detect(File)} instead
     * @param file file to analyze
     * @return the Mime Content Type of the specified document
     * @throws MimeTypeException if the type can't be detected
     * @throws IOException if the file can't be read
     */
    Q_DECL_DEPRECATED MimeType getMimeType(const QFileInfo &file) const;

    /**
     * Returns the registered, normalised media type with the given name (or alias).
     *
     * <p>Unlike {@link #forName(String)}, this function will <em>not<em> create a
     * new MimeType and register it. Instead, <code>null</code> will be returned if
     * there is no definition available for the given name.
     *
     * <p>Also, unlike {@link #forName(String)}, this function may return a
     * mime type that has fewer parameters than were included in the supplied name.
     * If the registered mime type has parameters (e.g.
     * <code>application/dita+xml;format=map</code>), then those will be maintained.
     * However, if the supplied name has paramenters that the <em>registered</em> mime
     * type does not (e.g. <code>application/xml; charset=UTF-8</code> as a name,
     * compared to just <code>application/xml</code> for the type in the registry),
     * then those parameters will not be included in the returned type.
     *
     * @param name media type name (case-insensitive)
     * @return the registered media type with the given name or alias, or null if not found
     * @throws MimeTypeException if the given media type name is invalid
     */
    MimeType getRegisteredMimeType(const QString &name) const;

    void setSuperType(const MimeType &type, const MediaType &parent);

    /**
     * Adds an alias for the given media type. This method should only
     * be called from {@link MimeType#addAlias(String)}.
     *
     * @param type media type
     * @param alias media type alias (normalized to lower case)
     */
    void addAlias(const MimeType &type, const MediaType &alias);

    /**
     * Adds a file name pattern for the given media type. The caller can specify
     * whether the pattern being added <b>is</b> or <b>is not</b> a JDK standard
     * regular expression via the <code>isRegex</code> parameter. If the value
     * is set to true, then a JDK standard regex is assumed, otherwise the
     * freedesktop glob type is assumed.
     *
     * @param type
     *            media type
     * @param pattern
     *            file name pattern
     * @param isRegex
     *            set to true if JDK std regexs are desired, otherwise set to
     *            false.
     * @throws MimeTypeException
     *             if the pattern conflicts with existing ones.
     *
     */
    void addPattern(const MimeType &type, const QString &pattern, bool isRegex = false);

    MediaTypeRegistry mediaTypeRegistry() const;

    /**
     * Add the specified mime-type in the repository.
     *
     * @param type
     *            is the mime-type to add.
     */
    void add(const MimeType &type);

    /**
     * Return the minimum length of data to provide to analyzing methods based
     * on the document's content in order to check all the known MimeTypes.
     *
     * @return the minimum length of data to provide.
     * @see #getMimeType(byte[])
     * @see #getMimeType(String, byte[])
     */
    Q_DECL_CONSTEXPR static int minLength();

    /**
     * Returns the registered media type with the given name (or alias).
     * The named media type is automatically registered (and returned) if
     * it doesn't already exist.
     *
     * @param name media type name (case-insensitive)
     * @return the registered media type with the given name or alias
     * @throws MimeTypeException if the given media type name is invalid
     */
    MimeType forName(const QString &name);

    /**
     * Reads the first {@link #getMinLength()} bytes from the given stream.
     * If the stream is shorter, then the entire content of the stream is
     * returned.
     * <p>
     * The given stream is never {@link InputStream#close() closed},
     * {@link InputStream#mark(int) marked}, or
     * {@link InputStream#reset() reset} by this method.
     *
     * @param stream stream to be read
     * @return first {@link #getMinLength()} (or fewer) bytes of the stream
     * @throws IOException if the stream can not be read
     */
    static QByteArray readMagicHeader(QIODevice *stream);

    /**
     * Automatically detects the MIME type of a document based on magic
     * markers in the stream prefix and any given metadata hints.
     * <p>
     * The given stream is expected to support marks, so that this method
     * can reset the stream to the position it was in before this method
     * was called.
     *
     * @param input document stream, or <code>null</code>
     * @param metadata metadata hints
     * @return MIME type of the document
     * @throws IOException if the document stream could not be read
     */
    virtual MediaType detect(QIODevice *input, const Metadata &meta) const Q_DECL_OVERRIDE;

public:
    static MimeTypes defaultMimeTypes();

private:
    QSharedDataPointer<MimeTypesData> data;
};


}       // namespace mime

}       // namespace qtika
