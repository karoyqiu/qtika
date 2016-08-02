# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

QT       -= gui

TARGET = qtika-core
TEMPLATE = lib
CONFIG += warn_on c++11

DEFINES += QTIKACORE_LIBRARY
VERSION = 1.13

SOURCES += \
    mime/mediatype.cpp \
    metadata/creativecommons.cpp \
    metadata/property.cpp \
    metadata/propertytypeexception.cpp \
    metadata/geographic.cpp \
    metadata/httpheaders.cpp \
    metadata/message.cpp \
    metadata/msoffice.cpp \
    metadata/climateforcast.cpp \
    metadata/tiff.cpp \
    metadata/tikametadatakeys.cpp \
    metadata/tikamimekeys.cpp \
    metadata/metadata.cpp

HEADERS += \
    qtika-core-global.h \
    mime/mediatype.h \
    stable.h \
    metadata/creativecommons.h \
    metadata/property.h \
    metadata/propertytypeexception.h \
    metadata/private/propertydata.h \
    metadata/geographic.h \
    metadata/httpheaders.h \
    metadata/message.h \
    metadata/msoffice.h \
    metadata/climateforcast.h \
    metadata/tiff.h \
    metadata/tikametadatakeys.h \
    metadata/tikamimekeys.h \
    metadata/metadata.h

#PRECOMPILED_HEADER = qtika-core-global.h

win32: QMAKE_CXXFLAGS_WARN_ON = /W4

unix {
    target.path = /usr/lib
    INSTALLS += target
}
