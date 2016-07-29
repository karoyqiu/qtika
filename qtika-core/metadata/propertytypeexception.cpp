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
#include "propertytypeexception.h"

#include <QCoreApplication>

#include "private/propertydata.h"


namespace qtika {

namespace metadata {


PropertyTypeException::PropertyTypeException(PropertyType expected, PropertyType actual)
    : std::invalid_argument(tr("Expected a property of type %1, but received %2.").arg(toString(expected), toString(actual)).toStdString())
{
}


PropertyTypeException::PropertyTypeException(ValueType expected, ValueType actual)
    : std::invalid_argument(tr("Expected a property with a %1 value, but received a %2.").arg(toString(expected)).arg(toString(actual)).toStdString())
{
}


PropertyTypeException::PropertyTypeException(PropertyType unsupported)
    : std::invalid_argument(unsupported != PropertyType::COMPOSITE
        ? tr("%1 is not supported.").arg(toString(unsupported)).toStdString()
        : tr("Composite Properties must not include other Composite Properties as either Primary or Secondary.").toStdString())
{
}


}       // namespace metadata

}       // namespace qtika
