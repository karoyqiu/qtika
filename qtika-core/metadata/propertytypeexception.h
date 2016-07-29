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

#include <stdexcept>
#include <QCoreApplication>

#include "property.h"


namespace qtika {

namespace metadata {


/**
 * XMP property definition violation exception. This is thrown when
 * you try to set a {@link Property} value with an incorrect type,
 * such as storing an Integer when the property is of type Date.
 *
 * @since Apache Tika 0.8
 */
class QTIKACORESHARED_EXPORT PropertyTypeException Q_DECL_FINAL
    : public std::invalid_argument
{
    Q_DECLARE_TR_FUNCTIONS(PropertyTypeException)

public:
    typedef Property::PropertyType PropertyType;
    typedef Property::ValueType ValueType;

    using std::invalid_argument::invalid_argument;

    PropertyTypeException(PropertyType expected, PropertyType actual);
    PropertyTypeException(ValueType expected, ValueType actual);
    explicit PropertyTypeException(PropertyType unsupported);
};


}       // namespace metadata

}       // namespace qtika
