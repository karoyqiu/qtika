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
#include "property.h"

#include <type_traits>

#include <QException>
#include <QHash>
#include <QMutex>
#include <QSet>

#include "propertydata.h"
#include "propertytypeexception.h"


namespace qtika {

namespace metadata {

namespace {

template<typename T, typename... Args>
static Property throwException(Args&& ...args)
{
#ifndef QT_NO_EXCEPTIONS
    static_assert(std::is_base_of<std::exception, T>::value, "T must be derived from std::exception.");
    throw T(args...);
#else
    static Property dummy;
    qCritical() << msg;
    return dummy;
#endif
}

}


QHash<QString, Property> PropertyData::properties;
QMutex PropertyData::mutex;

PropertyData::PropertyData()
    : internal(false)
    , propertyType(Property::PropertyType::INVALID)
    , valueType(Property::ValueType::INVALID)
{
}


Property::Property()
    : data(new PropertyData)
{
}

Property::Property(const QString &name, bool internal, PropertyType propertyType,
                   ValueType valueType, const QStringList &choices, const Property &primaryProperty,
                   const QList<Property> &secondaryExtractProperties)
    : Property()
{
    data->name = name;
    data->internal = internal;
    data->propertyType = propertyType;
    data->valueType = valueType;
    data->choices = choices.toSet();

    if (primaryProperty.isNull())
    {
        data->primaryProperty = *this;
        QMutexLocker locker(&PropertyData::mutex);
        PropertyData::properties.insert(name, *this);
    }
    else
    {
        data->primaryProperty = primaryProperty;
        data->secondaryExtractProperties = secondaryExtractProperties;
    }
}


Property::Property(const QString &name, bool internal, Property::ValueType valueType,
                   const QStringList &choices)
    : Property(name, internal, PropertyType::SIMPLE, valueType, choices)
{
}


Property::Property(const Property &rhs)
    : data(rhs.data)
{
}


Property &Property::operator=(const Property &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


bool Property::isNull() const
{
    return !data->name.isEmpty();
}


QString Property::name() const
{
    return data->name;
}


bool Property::isInternal() const
{
    return data->internal;
}


bool Property::isExternal() const
{
    return !data->internal;
}


bool Property::isMultiValuePermitted() const
{
    switch (data->propertyType)
    {
    case PropertyType::BAG:
    case PropertyType::SEQ:
    case PropertyType::ALT:
        return true;

    case PropertyType::COMPOSITE:
        // Base it on the primary property's behaviour
        return data->primaryProperty.isMultiValuePermitted();

    default:
        return false;
    }
}


Property::PropertyType Property::propertyType() const
{
    return data->propertyType;
}


Property::ValueType Property::valueType() const
{
    return data->valueType;
}


QSet<QString> Property::choices() const
{
    return data->choices;
}


Property Property::primaryProperty() const
{
    return data->primaryProperty;
}


QList<Property> Property::secondaryExtractProperties() const
{
    return data->secondaryExtractProperties;
}


Property::PropertyType Property::propertyType(const QString &key)
{
    return PropertyData::properties.value(key).propertyType();
}


Property Property::get(const QString &key)
{
    return PropertyData::properties.value(key);
}


QSet<Property> Property::properties(const QString &prefix)
{
    QSet<Property> set;
    QString p = prefix + ":";

    QMutexLocker locker(&PropertyData::mutex);

    for (auto iter = PropertyData::properties.begin(); iter != PropertyData::properties.end(); ++iter)
    {
        if (iter.key().startsWith(p))
        {
            set.insert(iter.value());
        }
    }

    return set;
}


Property Property::internalBoolean(const QString &name)
{
    return Property(name, true, ValueType::BOOLEAN);
}


Property Property::internalClosedChoice(const QString &name, const QStringList &choices)
{
    return Property(name, true, ValueType::CLOSED_CHOICE, choices);
}


Property Property::internalDate(const QString &name)
{
    return Property(name, true, ValueType::DATE);
}


Property Property::internalInteger(const QString &name)
{
    return Property(name, true, ValueType::INTEGER);
}


Property Property::internalIntegerSequence(const QString &name)
{
    return Property(name, true, PropertyType::SEQ, ValueType::INTEGER);
}


Property Property::internalRational(const QString &name)
{
    return Property(name, true, ValueType::RATIONAL);
}


Property Property::internalOpenChoice(const QString &name, const QStringList &choices)
{
    return Property(name, true, ValueType::OPEN_CHOICE, choices);
}


Property Property::internalReal(const QString &name)
{
    return Property(name, true, ValueType::REAL);
}


Property Property::internalText(const QString &name)
{
    return Property(name, true, ValueType::TEXT);
}


Property Property::internalTextBag(const QString &name)
{
    return Property(name, true, PropertyType::BAG, ValueType::TEXT);
}


Property Property::internalURI(const QString &name)
{
    return Property(name, true, ValueType::URI);
}


Property Property::externalBoolean(const QString &name)
{
    return Property(name, false, ValueType::BOOLEAN);
}


Property Property::externalClosedChoice(const QString &name, const QStringList &choices)
{
    return Property(name, false, ValueType::CLOSED_CHOICE, choices);
}


Property Property::externalDate(const QString &name)
{
    return Property(name, false, ValueType::DATE);
}


Property Property::externalInteger(const QString &name)
{
    return Property(name, false, ValueType::INTEGER);
}


Property Property::externalRational(const QString &name)
{
    return Property(name, false, ValueType::RATIONAL);
}


Property Property::externalOpenChoice(const QString &name, const QStringList &choices)
{
    return Property(name, false, ValueType::OPEN_CHOICE, choices);
}


Property Property::externalReal(const QString &name)
{
    return Property(name, false, ValueType::REAL);
}


Property Property::externalText(const QString &name)
{
    return Property(name, false, ValueType::TEXT);
}


Property Property::externalTextBag(const QString &name)
{
    return Property(name, false, PropertyType::BAG, ValueType::TEXT);
}


Property Property::composite(const Property &primaryProperty,
                             const QList<Property> &secondaryExtractProperties)
{
    if (primaryProperty.isNull())
    {
        return throwException<std::invalid_argument>("Primary property must not be null.");
    }

    if (primaryProperty.propertyType() == PropertyType::COMPOSITE)
    {
        return throwException<PropertyTypeException>(primaryProperty.propertyType());
    }

    if (!secondaryExtractProperties.isEmpty())
    {
        for (const Property &sec : secondaryExtractProperties)
        {
            if (sec.propertyType() == PropertyType::COMPOSITE)
            {
                return throwException<PropertyTypeException>(sec.propertyType());
            }
        }
    }

    QStringList choices = primaryProperty.choices().toList();
    return Property(primaryProperty.name(), primaryProperty.isInternal(), PropertyType::COMPOSITE,
                    ValueType::PROPERTY, choices, primaryProperty, secondaryExtractProperties);
}


bool operator==(const Property &lhs, const Property &rhs)
{
    return lhs.name() == rhs.name();
}


bool operator<(const Property &lhs, const Property &rhs)
{
    return lhs.name() < rhs.name();
}


uint qHash(const Property &p, uint seed)
{
    return qHash(p.name(), seed);
}


QString toString(Property::PropertyType type)
{
    switch (type)
    {
    case Property::PropertyType::SIMPLE:
        return QS("SIMPLE");
    case Property::PropertyType::STRUCTURE:
        return QS("STRUCTURE");
    case Property::PropertyType::BAG:
        return QS("BAG");
    case Property::PropertyType::SEQ:
        return QS("SEQ");
    case Property::PropertyType::ALT:
        return QS("ALT");
    case Property::PropertyType::COMPOSITE:
        return QS("COMPOSITE");
    default:
        Q_UNREACHABLE();
    }
}


QString toString(Property::ValueType type)
{
    switch (type)
    {
    case Property::ValueType::BOOLEAN:
        return QS("BOOLEAN");
    case Property::ValueType::OPEN_CHOICE:
        return QS("OPEN_CHOICE");
    case Property::ValueType::CLOSED_CHOICE:
        return QS("CLOSED_CHOICE");
    case Property::ValueType::DATE:
        return QS("DATE");
    case Property::ValueType::INTEGER:
        return QS("INTEGER");
    case Property::ValueType::LOCALE:
        return QS("LOCALE");
    case Property::ValueType::MIME_TYPE:
        return QS("MIME_TYPE");
    case Property::ValueType::PROPER_NAME:
        return QS("PROPER_NAME");
    case Property::ValueType::RATIONAL:
        return QS("RATIONAL");
    case Property::ValueType::REAL:
        return QS("REAL");
    case Property::ValueType::TEXT:
        return QS("TEXT");
    case Property::ValueType::URI:
        return QS("URI");
    case Property::ValueType::URL:
        return QS("URL");
    case Property::ValueType::XPATH:
        return QS("XPATH");
    case Property::ValueType::PROPERTY:
        return QS("PROPERTY");
    default:
        Q_UNREACHABLE();
    }
}


}       // namespace metadata

}       // namespace qtika
