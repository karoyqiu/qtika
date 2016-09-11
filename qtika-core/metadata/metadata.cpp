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
#include "metadata.h"

#include <QHash>
#include <QSharedData>
#include <QStringBuilder>

#include "dateformat.h"
#include "propertytypeexception.h"


namespace qtika {

namespace metadata {


class MetadataData : public QSharedData
{
public:
    /**
     * Parses the given date string. This method is synchronized to prevent
     * concurrent access to the thread-unsafe date formats.
     *
     * @see <a href="https://issues.apache.org/jira/browse/TIKA-495">TIKA-495</a>
     * @param date date string
     * @return parsed date, or <code>null</code> if the date can't be parsed
     */
    static QDateTime parseDate(const QString &date);

public:
    /**
     * A map of all metadata attributes.
     */
    QHash<QString, QStringList> metadata;
};


QDateTime MetadataData::parseDate(const QString &date)
{
    /**
     * Some parsers will have the date as a ISO-8601 string
     *  already, and will set that into the Metadata object.
     * So we can return Date objects for these, this is the
     *  list (in preference order) of the various ISO-8601
     *  variants that we try when processing a date based
     *  property.
     */
    static const QList<DateFormat> formats = {
        // yyyy-mm-ddThh...
        DateFormat(QS("yyyy-MM-dd'T'HH:mm:ss'Z'")),         // UTC/Zulu
        DateFormat(QS("yyyy-MM-dd'T'HH:mm:ssZ")),           // With timezone
        DateFormat(QS("yyyy-MM-dd'T'HH:mm:ss")),            // Without timezone
        // yyyy-mm-dd hh...
        DateFormat(QS("yyyy-MM-dd' 'HH:mm:ss'Z'")),         // UTC/Zulu
        DateFormat(QS("yyyy-MM-dd' 'HH:mm:ssZ")),           // With timezone
        DateFormat(QS("yyyy-MM-dd' 'HH:mm:ss")),            // Without timezone
        // Date without time, set to Midday UTC
        DateFormat(QS("yyyy-MM-dd"), Qt::OffsetFromUTC),    // Normal date format
        DateFormat(QS("yyyy:MM:dd"), Qt::OffsetFromUTC)     // Image (IPTC/EXIF) format
    };

    QDateTime dt;

    for (const DateFormat &format : formats)
    {
        dt = format.fromString(date);

        if (dt.isValid())
        {
            break;
        }
    }

    return dt;
}


Metadata::Metadata()
    : data(new MetadataData)
{
}


Metadata::Metadata(const Metadata &rhs)
    : data(rhs.data)
{
}


Metadata &Metadata::operator=(const Metadata &rhs)
{
    if (this != &rhs)
    {
        data.operator=(rhs.data);
    }

    return *this;
}


Metadata::~Metadata()
{
}


bool Metadata::isMultiValued(const Property &property) const
{
    return isMultiValued(property.name());
}


bool Metadata::isMultiValued(const QString &name) const
{
    return data->metadata.contains(name)
            && data->metadata.value(name).size() > 1;
}


QStringList Metadata::names() const
{
    return data->metadata.keys();
}


QString Metadata::get(const QString &name) const
{
    QString value;

    if (data->metadata.contains(name))
    {
        value = data->metadata.value(name).first();
    }

    return value;
}


QString Metadata::get(const Property &property) const
{
    return get(property.name());
}


int Metadata::getInt(const Property &property, int defaultValue) const
{
    if (property.primaryProperty().propertyType() != Property::PropertyType::SIMPLE)
    {
        return defaultValue;
    }

    if (property.primaryProperty().valueType() != Property::ValueType::INTEGER)
    {
        return defaultValue;
    }

    QString s = get(property);

    if (s.isEmpty())
    {
        return defaultValue;
    }

    bool ok = false;
    int value = s.toInt(&ok, 0);

    return ok ? value : defaultValue;
}


QDateTime Metadata::getDate(const Property &property) const
{
    QDateTime value;

    if (property.primaryProperty().propertyType() != Property::PropertyType::SIMPLE)
    {
        return value;
    }

    if (property.primaryProperty().valueType() != Property::ValueType::DATE)
    {
        return value;
    }

    QString s = get(property);

    if (s.isEmpty())
    {
        return value;
    }

    value = MetadataData::parseDate(s);
    return value;
}


QStringList Metadata::getValues(const QString &name) const
{
    return data->metadata.value(name);
}


QStringList Metadata::getValues(const Property &property) const
{
    return getValues(property.name());
}


void Metadata::add(const QString &name, const QString &value)
{
    QStringList values = getValues(name);
    values.append(value);
    data->metadata.insert(name, values);
}


void Metadata::add(const Property &property, const QString &value)
{
    QStringList values = getValues(property);

    if (values.isEmpty())
    {
        set(property, value);
    }
    else if (property.isMultiValuePermitted())
    {
        values.append(value);
        set(property, values);
    }
    else
    {
        QString message = property.name() % QS(" : ") % metadata::toString(property.propertyType());
        throw PropertyTypeException(message.toStdString());
    }
}


void Metadata::set(const QString &name, const QString &value)
{
    if (value.isEmpty())
    {
        data->metadata.remove(name);
    }
    else
    {
        data->metadata.insert(name, { value });
    }
}


void Metadata::set(const Property &property, const QString &value)
{
    if (property.isNull())
    {
        throw std::invalid_argument("Property must not be null.");
    }

    if (property.propertyType() == Property::PropertyType::COMPOSITE)
    {
        set(property.primaryProperty(), value);

        for (const Property &sec : property.secondaryExtractProperties())
        {
            set(sec, value);
        }
    }
    else
    {
        set(property.name(), value);
    }
}


void Metadata::set(const Property &property, const QStringList &values)
{
    if (property.isNull())
    {
        throw std::invalid_argument("Property must not be null.");
    }

    if (property.propertyType() == Property::PropertyType::COMPOSITE)
    {
        set(property.primaryProperty(), values);

        for (const Property &sec : property.secondaryExtractProperties())
        {
            set(sec, values);
        }
    }
    else
    {
        data->metadata.insert(property.name(), values);
    }
}


void Metadata::set(const Property &property, int value)
{
    if (property.primaryProperty().propertyType() != Property::PropertyType::SIMPLE)
    {
        throw PropertyTypeException(Property::PropertyType::SIMPLE,
                                    property.primaryProperty().propertyType());
    }

    if (property.primaryProperty().valueType() != Property::ValueType::INTEGER)
    {
        throw PropertyTypeException(Property::ValueType::INTEGER,
                                    property.primaryProperty().valueType());
    }

    set(property, QString::number(value));
}


void Metadata::set(const Property &property, double value)
{
    if (property.primaryProperty().propertyType() != Property::PropertyType::SIMPLE)
    {
        throw PropertyTypeException(Property::PropertyType::SIMPLE,
                                    property.primaryProperty().propertyType());
    }

    if (property.primaryProperty().valueType() != Property::ValueType::REAL
        && property.primaryProperty().valueType() != Property::ValueType::RATIONAL)
    {
        throw PropertyTypeException(Property::ValueType::REAL,
                                    property.primaryProperty().valueType());
    }

    set(property, QString::number(value));
}


void Metadata::set(const Property &property, const QDateTime &date)
{
    if (property.primaryProperty().propertyType() != Property::PropertyType::SIMPLE)
    {
        throw PropertyTypeException(Property::PropertyType::SIMPLE,
                                    property.primaryProperty().propertyType());
    }

    if (property.primaryProperty().valueType() != Property::ValueType::DATE)
    {
        throw PropertyTypeException(Property::ValueType::DATE,
                                    property.primaryProperty().valueType());
    }

    set(property, date.toString(Qt::ISODate));
}


void Metadata::remove(const QString &name)
{
    data->metadata.remove(name);
}


int Metadata::size() const
{
    return data->metadata.size();
}


QString Metadata::toString() const
{
    QStringList pairs;

    for (auto iter = data->metadata.begin(); iter != data->metadata.end(); ++iter)
    {
        for (const QString &value : iter.value())
        {
            pairs.append(iter.key() % QS("=") % value);
        }
    }

    return pairs.join(QLatin1Char(' '));
}


QString Metadata::NAMESPACE_PREFIX_DELIMITER()
{
    return QS(":");
}


bool operator==(const Metadata &lhs, const Metadata &rhs)
{
    return (lhs.data == rhs.data)
            || (lhs.data->metadata == rhs.data->metadata);
}


bool operator!=(const Metadata &lhs, const Metadata &rhs)
{
    return !(lhs == rhs);
}


}       // namespace metadata

}       // namespace qtika
