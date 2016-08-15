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

#include <QSharedDataPointer>

#include "mimetype.h"


namespace qtika {

namespace mime {


class MediaTypeRegistry;
class PatternsData;


/**
 * Defines a MimeType pattern.
 */
class Patterns
{
public:
    Patterns();
    explicit Patterns(MediaTypeRegistry *registry);
    Patterns(const Patterns &);
    Patterns &operator=(const Patterns &);
    ~Patterns();

    void add(const QString &pattern, const MimeType &type);

    void add(const QString &pattern, bool isRegex, const MimeType &type);

    /**
     * Find the MimeType corresponding to a resource name.
     *
     * It applies the recommendations detailed in FreeDesktop Shared MIME-info
     * Database for guessing MimeType from a resource name: It first tries a
     * case-sensitive match, then try again with the resource name converted to
     * lower-case if that fails. If several patterns match then the longest
     * pattern is used. In particular, files with multiple extensions (such as
     * Data.tar.gz) match the longest sequence of extensions (eg '*.tar.gz' in
     * preference to '*.gz'). Literal patterns (eg, 'Makefile') are matched
     * before all others. Patterns beginning with `*.' and containing no other
     * special characters (`*?[') are matched before other wildcarded patterns
     * (since this covers the majority of the patterns).
     */
    MimeType matches(const QString &name) const;

private:
    QSharedDataPointer<PatternsData> data;
};


}       // namespace mime

}       // namespace qtika
