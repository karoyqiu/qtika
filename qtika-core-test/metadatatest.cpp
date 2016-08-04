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
#include "metadatatest.h"

#include <QtTest>
#include <metadata/metadata.h>
#include <metadata/property.h>
#include <metadata/propertytypeexception.h>
#include <utils/dateutils.h>


using qtika::metadata::Metadata;
using qtika::metadata::Property;
using qtika::metadata::PropertyTypeException;

static const QString CONTENTTYPE = "contenttype";


void MetadataTest::testAdd()
{
    QStringList values;
    Metadata meta;

    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 0);

    meta.add(CONTENTTYPE, "value1");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 1);
    QCOMPARE(values.at(0), QS("value1"));

    meta.add(CONTENTTYPE, "value2");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 2);
    QCOMPARE(values.at(0), QS("value1"));
    QCOMPARE(values.at(1), QS("value2"));

    // NOTE : For now, the same value can be added many times.
    // Should it be changed?
    meta.add(CONTENTTYPE, "value1");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 3);
    QCOMPARE(values.at(0), QS("value1"));
    QCOMPARE(values.at(1), QS("value2"));
    QCOMPARE(values.at(2), QS("value1"));

    Property nonMultiValued = Property::internalText("nonMultiValued");
    meta.add(nonMultiValued, "value1");
    QVERIFY_EXCEPTION_THROWN(meta.add(nonMultiValued, "value2"), PropertyTypeException);
}


void MetadataTest::testSet()
{
    QStringList values;
    Metadata meta;

    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 0);

    meta.set(CONTENTTYPE, "value1");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 1);
    QCOMPARE(values.at(0), QS("value1"));

    meta.set(CONTENTTYPE, "value2");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 1);
    QCOMPARE(values.at(0), QS("value2"));

    meta.set(CONTENTTYPE, "new value 1");
    meta.add(CONTENTTYPE, "new value 2");
    values = meta.getValues(CONTENTTYPE);
    QCOMPARE(values.size(), 2);
    QCOMPARE(values.at(0), QS("new value 1"));
    QCOMPARE(values.at(1), QS("new value 2"));
}


void MetadataTest::testGet()
{
    Metadata meta;
    QVERIFY(meta.get("a-name").isNull());
    meta.add("a-name", "value1");
    QCOMPARE(meta.get("a-name"), QS("value1"));
    meta.add("a-name", "value2");
    QCOMPARE(meta.get("a-name"), QS("value1"));
}


void MetadataTest::testIsMultiValued()
{
    Metadata meta;
    QCOMPARE(meta.isMultiValued("key"), false);
    meta.add("key", "value1");
    QCOMPARE(meta.isMultiValued("key"), false);
    meta.add("key", "value2");
    QCOMPARE(meta.isMultiValued("key"), true);
}


void MetadataTest::testNames()
{
    Metadata meta;
    QStringList names = meta.names();
    QCOMPARE(names.size(), 0);

    meta.add("name-one", "value");
    names = meta.names();
    QCOMPARE(names.size(), 1);
    QCOMPARE(names.at(0), QS("name-one"));
    meta.add("name-two", "value");
    names = meta.names();
    QCOMPARE(names.size(), 2);
}


void MetadataTest::testRemove()
{
    Metadata meta;
    meta.remove("name-one");
    QCOMPARE(meta.size(), 0);
    meta.add("name-one", "value-1.1");
    meta.add("name-one", "value-1.2");
    meta.add("name-two", "value-2.2");
    QCOMPARE(meta.size(), 2);
    QCOMPARE(meta.get("name-one").isNull(), false);
    QCOMPARE(meta.get("name-two").isNull(), false);
    meta.remove("name-one");
    QCOMPARE(meta.size(), 1);
    QCOMPARE(meta.get("name-one").isNull(), true);
    QCOMPARE(meta.get("name-two").isNull(), false);
    meta.remove("name-two");
    QCOMPARE(meta.size(), 0);
    QCOMPARE(meta.get("name-one").isNull(), true);
    QCOMPARE(meta.get("name-two").isNull(), true);
}


void MetadataTest::testEquals()
{
    Metadata meta1, meta2;
    QVERIFY(meta1 == meta2);
    meta1.add("name-one", "value-1.1");
    QVERIFY(meta1 != meta2);
    meta2.add("name-one", "value-1.1");
    QVERIFY(meta1 == meta2);
    meta1.add("name-one", "value-1.2");
    QVERIFY(meta1 != meta2);
    meta2.add("name-one", "value-1.2");
    QVERIFY(meta1 == meta2);
    meta1.add("name-two", "value-2.1");
    QVERIFY(meta1 != meta2);
    meta2.add("name-two", "value-2.1");
    QVERIFY(meta1 == meta2);
    meta1.add("name-two", "value-2.2");
    QVERIFY(meta1 != meta2);
    meta2.add("name-two", "value-2.x");
    QVERIFY(meta1 != meta2);
}


void MetadataTest::testGetSetInt()
{
    Metadata meta;

    // Isn't initially set, will get null back
    QVERIFY(meta.get(Metadata::IMAGE_WIDTH()).isNull());
    QCOMPARE(meta.getInt(Metadata::IMAGE_WIDTH()), 0);

    // Can only set as a single valued int
    QVERIFY_EXCEPTION_THROWN(meta.set(Metadata::BITS_PER_SAMPLE(), 1), PropertyTypeException);

    // Can set it and retrieve it
    meta.set(Metadata::IMAGE_WIDTH(), 22);
    QCOMPARE(meta.get(Metadata::IMAGE_WIDTH()), QS("22"));
    QCOMPARE(meta.getInt(Metadata::IMAGE_WIDTH()), 22);

    // If you save a non int value, you get null
    meta.set(Metadata::IMAGE_WIDTH(), "INVALID");
    QCOMPARE(meta.get(Metadata::IMAGE_WIDTH()), QS("INVALID"));
    QCOMPARE(meta.getInt(Metadata::IMAGE_WIDTH()), 0);

    // If you try to retrieve a non simple int value, you get null
    meta.set(Metadata::IMAGE_WIDTH(), 22);
    QCOMPARE(meta.getInt(Metadata::IMAGE_WIDTH()), 22);
    QCOMPARE(meta.getInt(Metadata::BITS_PER_SAMPLE()), 0);
}


void MetadataTest::testGetSetDate()
{
    Metadata meta;
    qint64 hour = 60 * 60 * 1000;

    // Isn't initially set, will get null back
    QVERIFY(meta.get(Metadata::LAST_MODIFIED()).isNull());
    QCOMPARE(meta.getInt(Metadata::LAST_MODIFIED()), 0);

    // Can only set as a single valued date
    QVERIFY_EXCEPTION_THROWN(meta.set(Metadata::BITS_PER_SAMPLE(), QDateTime::fromMSecsSinceEpoch(1000)),
                             PropertyTypeException);
    QVERIFY_EXCEPTION_THROWN(meta.set(Metadata::IMAGE_WIDTH(), QDateTime::fromMSecsSinceEpoch(1000)),
                             PropertyTypeException);

    // Can set it and retrieve it
    QDateTime oneSecond = QDateTime::fromMSecsSinceEpoch(1000, Qt::UTC);
    meta.set(Metadata::LAST_MODIFIED(), oneSecond);
    QCOMPARE(meta.get(Metadata::LAST_MODIFIED()), QS("1970-01-01T00:00:01Z"));
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);

    // If you save a non date value, you get null
    meta.set(Metadata::LAST_MODIFIED(), "INVALID");
    QCOMPARE(meta.get(Metadata::LAST_MODIFIED()), QS("INVALID"));
    QVERIFY(meta.getDate(Metadata::LAST_MODIFIED()).isNull());

    // If you try to retrieve a non simple date value, you get null
    meta.set(Metadata::LAST_MODIFIED(), oneSecond);
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    QCOMPARE(meta.getInt(Metadata::BITS_PER_SAMPLE()), 0);
    QCOMPARE(meta.getInt(Metadata::LAST_MODIFIED()), 0);

    // Our format doesn't include milliseconds
    // This means things get rounded
    meta.set(Metadata::LAST_MODIFIED(), QDateTime::fromMSecsSinceEpoch(1050, Qt::UTC));
    QCOMPARE(meta.get(Metadata::LAST_MODIFIED()), QS("1970-01-01T00:00:01Z"));
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);

    // We can accept a number of different ISO-8601 variants
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01T00:00:01Z");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01 00:00:01Z");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01T01:00:01+01:00");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01 01:00:01+01:00");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01T12:00:01+12:00");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);
    meta.set(Metadata::LAST_MODIFIED(), "1969-12-31T12:00:01-12:00");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()), oneSecond);

    // Dates without times, come in at midday UTC
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01");
    QVERIFY(!meta.getDate(Metadata::LAST_MODIFIED()).isNull());
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()).toMSecsSinceEpoch(), 12 * hour);
    meta.set(Metadata::LAST_MODIFIED(), "1970:01:01");
    QCOMPARE(meta.getDate(Metadata::LAST_MODIFIED()).toMSecsSinceEpoch(), 12 * hour);
}


void MetadataTest::testGetSetDateUnspecifiedTimezone()
{
    Metadata meta;

    // Set explictly without a timezone
    meta.set(Metadata::LAST_MODIFIED(), "1970-01-01T00:00:01");
    QCOMPARE(meta.get(Metadata::LAST_MODIFIED()), QS("1970-01-01T00:00:01"));

    // Now ask DateUtils to format for us without one
    meta.set(Metadata::LAST_MODIFIED(), qtika::utils::DateUtils::formatDateUnknownTimezone(QDateTime::fromMSecsSinceEpoch(1000)));
    QCOMPARE(meta.get(Metadata::LAST_MODIFIED()), QS("1970-01-01T00:00:01"));
}


void MetadataTest::testCompositeProperty()
{
    Metadata meta;
    Property desc = Property::internalText("dc:description");
    Property compositeProperty = Property::composite(desc, {
                                                         Property::internalText("description"),
                                                         Property::internalText("testDescriptionAlt")
                                                     });
    QString message("composite description");
    meta.set(compositeProperty, message);

    // Fetch as the composite
    QCOMPARE(meta.get(compositeProperty), message);
    // Fetch as the primary property on the composite
    QCOMPARE(meta.get(desc), message);
    // Fetch as the aliases
    QCOMPARE(meta.get("description"), message);
    QCOMPARE(meta.get("testDescriptionAlt"), message);
}
