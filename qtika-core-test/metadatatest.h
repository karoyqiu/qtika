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

#include <QObject>


class MetadataTest : public QObject
{
    Q_OBJECT

private slots:
    /** Test for the <code>add(String, String)</code> method. */
    void testAdd();

    /** Test for the <code>set(String, String)</code> method. */
    void testSet();

    /** Test for <code>setAll(Properties)</code> method. */
    //void testSetProperties();

    /** Test for <code>get(String)</code> method. */
    void testGet();

    /** Test for <code>isMultiValued()</code> method. */
    void testIsMultiValued();

    /** Test for <code>names</code> method. */
    void testNames();

    /** Test for <code>remove(String)</code> method. */
    void testRemove();

    /** Test for <code>equals(Object)</code> method. */
    void testEquals();

    /**
     * Tests for getting and setting integer
     *  based properties
     */
    void testGetSetInt();

    /**
     * Tests for getting and setting date
     *  based properties
     */
    void testGetSetDate();

    /**
     * Some documents, like jpegs, might have date in unspecified time zone
     * which should be handled like strings but verified to have parseable ISO 8601 format
     */
    void testGetSetDateUnspecifiedTimezone();

    /**
     * Defines a composite property, then checks that when set as the
     *  composite the value can be retrieved with the property or the aliases
     */
    void testCompositeProperty();
};
