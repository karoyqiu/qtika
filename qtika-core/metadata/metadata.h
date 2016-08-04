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

#include <QDateTime>
#include <QSharedDataPointer>

#include "climateforcast.h"
#include "creativecommons.h"
#include "geographic.h"
#include "httpheaders.h"
#include "message.h"
#include "msoffice.h"
#include "tiff.h"
#include "tikametadatakeys.h"
#include "tikamimekeys.h"


namespace qtika {

namespace metadata {


class MetadataData;

/**
 * A multi-valued metadata container.
 */
class QTIKACORESHARED_EXPORT Metadata
        : public CreativeCommons
        , public Geographic
        , public HttpHeaders
        , public Message
        , public MSOffice
        , public ClimateForcast
        , public TIFF
        , public TikaMetadataKeys
        , public TikaMimeKeys
{
public:
    /**
     * Constructs a new, empty metadata.
     */
    Metadata();
    Metadata(const Metadata &);
    Metadata &operator=(const Metadata &);
    virtual ~Metadata();

    /**
     * Returns true if named value is multivalued.
     *
     * @param property
     *          metadata property
     * @return true is named value is multivalued, false if single value or null
     */
    bool isMultiValued(const Property &property) const;

    /**
     * Returns true if named value is multivalued.
     *
     * @param name
     *          name of metadata
     * @return true is named value is multivalued, false if single value or null
     */
    bool isMultiValued(const QString &name) const;

    /**
     * Returns an array of the names contained in the metadata.
     *
     * @return Metadata names
     */
    QStringList names() const;

    /**
     * Get the value associated to a metadata name. If many values are assiociated
     * to the specified name, then the first one is returned.
     *
     * @param name
     *          of the metadata.
     * @return the value associated to the specified metadata name.
     */
    QString get(const QString &name) const;

    /**
     * Returns the value (if any) of the identified metadata property.
     *
     * @since Apache Tika 0.7
     * @param property property definition
     * @return property value, or <code>null</code> if the property is not set
     */
    QString get(const Property &property) const;

    /**
     * Returns the value of the identified Integer based metadata property.
     *
     * @since Apache Tika 0.8
     * @param property simple integer property definition
     * @return property value as a Integer, or <code>null</code> if the property is not set, or not a valid Integer
     */
    int getInt(const Property &property, int defaultValue = 0) const;

    /**
     * Returns the value of the identified Date based metadata property.
     *
     * @since Apache Tika 0.8
     * @param property simple date property definition
     * @return property value as a Date, or <code>null</code> if the property is not set, or not a valid Date
     */
    QDateTime getDate(const Property &property) const;

    /**
     * Get the values associated to a metadata name.
     *
     * @param name
     *          of the metadata.
     * @return the values associated to a metadata name.
     */
    QStringList getValues(const QString &name) const;

    /**
     * Get the values associated to a metadata name.
     *
     * @param property
     *          of the metadata.
     * @return the values associated to a metadata name.
     */
    QStringList getValues(const Property &property) const;

    /**
     * Add a metadata name/value mapping. Add the specified value to the list of
     * values associated to the specified metadata name.
     *
     * @param name
     *          the metadata name.
     * @param value
     *          the metadata value.
     */
    void add(const QString &name, const QString &value);

    /**
     * Add a metadata property/value mapping. Add the specified value to the list of
     * values associated to the specified metadata property.
     *
     * @param property
     *          the metadata property.
     * @param value
     *          the metadata value.
     */
    void add(const Property &property, const QString &value);

    /**
     * Set metadata name/value. Associate the specified value to the specified
     * metadata name. If some previous values were associated to this name,
     * they are removed. If the given value is <code>null</code>, then the
     * metadata entry is removed.
     *
     * @param name the metadata name.
     * @param value  the metadata value, or <code>null</code>
     */
    void set(const QString &name, const QString &value);

    /**
     * Sets the value of the identified metadata property.
     *
     * @since Apache Tika 0.7
     * @param property property definition
     * @param value    property value
     */
    void set(const Property &property, const QString &value);

    /**
     * Sets the values of the identified metadata property.
     *
     * @since Apache Tika 1.2
     * @param property property definition
     * @param values    property values
     */
    void set(const Property &property, const QStringList &values);

    /**
     * Sets the integer value of the identified metadata property.
     *
     * @since Apache Tika 0.8
     * @param property simple integer property definition
     * @param value    property value
     */
    void set(const Property &property, int value);

    /**
     * Sets the real or rational value of the identified metadata property.
     *
     * @since Apache Tika 0.8
     * @param property simple real or simple rational property definition
     * @param value    property value
     */
    void set(const Property &property, double value);

    /**
     * Sets the date value of the identified metadata property.
     *
     * @since Apache Tika 0.8
     * @param property simple integer property definition
     * @param date     property value
     */
    void set(const Property &property, const QDateTime &date);

    /**
     * Remove a metadata and all its associated values.
     *
     * @param name
     *          metadata name to remove
     */
    void remove(const QString &name);

    /**
     * Returns the number of metadata names in this metadata.
     *
     * @return number of metadata names
     */
    int size() const;

    QString toString() const;

public:
    /**
     * The common delimiter used between the namespace abbreviation and the property name
     */
    static QString NAMESPACE_PREFIX_DELIMITER();

private:
    friend QTIKACORESHARED_EXPORT bool operator==(const Metadata &lhs, const Metadata &rhs);

    QSharedDataPointer<MetadataData> data;
};


QTIKACORESHARED_EXPORT bool operator==(const Metadata &lhs, const Metadata &rhs);
QTIKACORESHARED_EXPORT bool operator!=(const Metadata &lhs, const Metadata &rhs);


}       // namespace metadata

}       // namespace qtika
