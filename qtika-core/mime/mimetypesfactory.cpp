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
#include "mimetypesfactory.h"

#include <QDomDocument>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "mimetypesreader.h"


namespace qtika {

namespace mime {


MimeTypes MimeTypesFactory::create()
{
    return MimeTypes();
}


MimeTypes MimeTypesFactory::create(const QDomDocument &doc)
{
    MimeTypes mimeTypes;
    MimeTypesReader reader(mimeTypes);
    reader.read(doc);
    mimeTypes.init();
    return mimeTypes;
}


MimeTypes MimeTypesFactory::create(const QList<QIODevice *> &streams)
{
    MimeTypes mimeTypes;
    MimeTypesReader reader(mimeTypes);

    for (QIODevice *s : streams)
    {
        reader.read(s);
    }

    mimeTypes.init();
    return mimeTypes;
}


MimeTypes MimeTypesFactory::create(QIODevice *stream)
{
    return create(QList<QIODevice *> { stream });
}


MimeTypes MimeTypesFactory::create(const QList<QUrl> &urls)
{
    QNetworkAccessManager manager;
    QList<QNetworkReply *> replies;
    QList<QIODevice *> streams;

    for (const QUrl &url : urls)
    {
        replies.append(manager.get(QNetworkRequest(url)));
    }

    for (QNetworkReply *reply : replies)
    {
        if (!reply->isFinished())
        {
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
        }

        streams.append(reply);
    }

    return create(streams);
}


MimeTypes MimeTypesFactory::create(const QUrl &url)
{
    return create(QList<QUrl>{ url });
}


MimeTypes MimeTypesFactory::create(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return MimeTypes();
    }

    return create(&file);
}


MimeTypes MimeTypesFactory::create(const QString &coreFilePath, const QString &extensionFilePath)
{
    QList<QIODevice *> files;

    QFile core(coreFilePath);

    if (core.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        files.append(&core);
    }

    QFile extension(extensionFilePath);

    if (extension.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        files.append(&extension);
    }

    return create(files);
}


}       // namespace mime

}       // namespace qtika
