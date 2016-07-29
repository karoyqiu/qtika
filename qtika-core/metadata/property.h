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

#include <QSet>
#include <QSharedDataPointer>


namespace qtika {

namespace metadata {

namespace p { class PropertyData; }


/**
 * XMP property definition. Each instance of this class defines a single
 * metadata property like "dc:format". In addition to the property name,
 * the {@link ValueType value type} and category (internal or external)
 * of the property are included in the property definition. The available
 * choice values are also stored for open and closed choice value types.
 *
 * @since Apache Tika 0.7
 */
class QTIKACORESHARED_EXPORT Property Q_DECL_FINAL
{
public:
    enum class PropertyType
    {
        INVALID,
        /** A single value */
        SIMPLE,
        STRUCTURE,
        /** An un-ordered array */
        BAG,
        /** An ordered array */
        SEQ,
        /** An ordered array with some sort of criteria */
        ALT,
        /** Multiple child properties */
        COMPOSITE
    };

    enum class ValueType
    {
        INVALID,
        BOOLEAN, OPEN_CHOICE, CLOSED_CHOICE, DATE, INTEGER, LOCALE,
        MIME_TYPE, PROPER_NAME, RATIONAL, REAL, TEXT, URI, URL, XPATH, PROPERTY
    };

    Property();
    Property(const QString &name, bool internal, PropertyType propertyType, ValueType valueType,
             const QStringList &choices = { }, const Property &primaryProperty = Property(),
             const QList<Property> &secondaryExtractProperties = { });
    Property(const QString &name, bool internal, ValueType valueType, const QStringList &choices = { });
    Property(const Property &);
    Property &operator=(const Property &);

    bool isNull() const;

    QString name() const;

    bool isInternal() const;

    bool isExternal() const;

    /**
     * Is the PropertyType one which accepts multiple values?
     */
    bool isMultiValuePermitted() const;

    PropertyType propertyType() const;

    ValueType valueType() const;

    /**
     * Returns the (immutable) set of choices for the values of this property.
     * Only defined for {@link ValueType#OPEN_CHOICE open} and
     * {@link ValueType#CLOSED_CHOICE closed choice} value types.
     *
     * @return available choices, or <code>null</code>
     */
    QSet<QString> choices() const;

    /**
     * Gets the primary property for a composite property
     *
     * @return the primary property
     */
    Property primaryProperty() const;

    QList<Property> secondaryExtractProperties() const;

public:
    /**
     * Get the type of a property
     * @param key name of the property
     * @return the type of the property
     */
    static PropertyType propertyType(const QString &key);

    /**
     * Retrieve the property object that corresponds to the given key
     * @param key the property key or name
     * @return the Property object
     */
    static Property get(const QString &key);

    static QSet<Property> properties(const QString &prefix);

    static Property internalBoolean(const QString &name);

    static Property internalClosedChoice(const QString &name, const QStringList &choices);

    static Property internalDate(const QString &name);

    static Property internalInteger(const QString &name);

    static Property internalIntegerSequence(const QString &name);

    static Property internalRational(const QString &name);

    static Property internalOpenChoice(const QString &name, const QStringList &choices);

    static Property internalReal(const QString &name);

    static Property internalText(const QString &name);

    static Property internalTextBag(const QString &name);

    static Property internalURI(const QString &name);

    static Property externalBoolean(const QString &name);

    static Property externalClosedChoice(const QString &name, const QStringList &choices);

    static Property externalDate(const QString &name);

    static Property externalInteger(const QString &name);

    static Property externalRational(const QString &name);

    static Property externalOpenChoice(const QString &name, const QStringList &choices);

    static Property externalReal(const QString &name);

    static Property externalText(const QString &name);

    static Property externalTextBag(const QString &name);

    /**
     * Constructs a new composite property from the given primary and array of secondary properties.
     * <p>
     * Note that name of the composite property is taken from its primary property,
     * and primary and secondary properties must not be composite properties themselves.
     *
     * @param primaryProperty
     * @param secondaryExtractProperties
     * @return the composite property
     */
    static Property composite(const Property &primaryProperty,
                              const QList<Property> &secondaryExtractProperties);

private:
    QSharedDataPointer<p::PropertyData> data;
};

QTIKACORESHARED_EXPORT bool operator==(const Property &lhs, const Property &rhs);
QTIKACORESHARED_EXPORT bool operator<(const Property &lhs, const Property &rhs);

QTIKACORESHARED_EXPORT uint qHash(const Property &p, uint seed);

QString toString(Property::PropertyType type);
QString toString(Property::ValueType type);


}       // namespace metadata

}       // namespace qtika


// TODO: FIX THE COMPILE ERROR!
// I don't know why it failed to compile...
#include "private/propertydata.h"
