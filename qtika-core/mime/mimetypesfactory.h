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

#include "mimetypes.h"


class QDomDocument;


namespace qtika {

namespace mime {


/**
 * Creates instances of MimeTypes.
 */
class MimeTypesFactory
{
public:
    /**
     * Creates an empty instance; same as calling new MimeTypes().
     *
     * @return an empty instance
     */
    static MimeTypes create();

    /**
     * Creates and returns a MimeTypes instance from the specified document.
     * @throws MimeTypeException if the type configuration is invalid
     */
    static MimeTypes create(const QDomDocument &doc);

    /**
     * Creates and returns a MimeTypes instance from the specified input stream.
     * Does not close the input stream(s).
     * @throws IOException if the stream can not be read
     * @throws MimeTypeException if the type configuration is invalid
     */
    static MimeTypes create(const QList<QIODevice *> &streams);
    static MimeTypes create(QIODevice *stream);

    /**
     * Creates and returns a MimeTypes instance from the resource
     * at the location specified by the URL.  Opens and closes the
     * InputStream from the URL.
     * If multiple URLs are supplied, then they are loaded in turn.
     *
     * @throws IOException if the URL can not be accessed
     * @throws MimeTypeException if the type configuration is invalid
     */
    static MimeTypes create(const QList<QUrl> &urls);
    static MimeTypes create(const QUrl &url);

    /**
     * Creates and returns a MimeTypes instance from the specified file path,
     * as interpreted by the class loader in getResource().
     *
     * @throws IOException if the file can not be accessed
     * @throws MimeTypeException if the type configuration is invalid
     */
    static MimeTypes create(const QString &filePath);

#if 0
    /**
     * Creates and returns a MimeTypes instance. The core mimetypes
     *  will be loaded from the specified file path, and any custom
     *  override mimetypes found will loaded afterwards.
     * The file paths will be interpreted by the default class loader in
     *  getResource().
     *
     * @param coreFilePath The main MimeTypes file to load
     * @param extensionFilePath The name of extension MimeType files to load afterwards
     *
     * @throws IOException if the file can not be accessed
     * @throws MimeTypeException if the type configuration is invalid
     */
    static MimeTypes create(const QString &coreFilePath, const QString &extensionFilePath);
#endif
};


}       // namespace mime

}       // namespace qtika
