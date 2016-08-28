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

QT      -= gui
QT      += xml

TARGET = qtika-core
TEMPLATE = lib
CONFIG += warn_on c++11
CONFIG += skip_target_version_ext

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
    metadata/metadata.cpp \
    metadata/dateformat.cpp \
    utils/dateutils.cpp \
    metadata/dublincore.cpp \
    detect/namedetector.cpp \
    detect/magicdetector.cpp \
    mime/mimetype.cpp \
    mime/magic.cpp \
    mime/mediatyperegistry.cpp \
    mime/mimetypes.cpp \
    mime/patterns.cpp \
    detect/xmlrootextractor.cpp \
    detect/textdetector.cpp \
    detect/textstatistics.cpp \
    mime/mimetypesfactory.cpp \
    mimetypesreadermetkeys.cpp \
    mime/mimetypesreader.cpp \
    mime/orclause.cpp \
    mime/andclause.cpp \
    mime/magicmatch.cpp

HEADERS += \
    qtika-core-global.h \
    stable.h \
    mime/mediatype.h \
    metadata/creativecommons.h \
    metadata/property.h \
    metadata/propertytypeexception.h \
    metadata/geographic.h \
    metadata/httpheaders.h \
    metadata/message.h \
    metadata/msoffice.h \
    metadata/climateforcast.h \
    metadata/tiff.h \
    metadata/tikametadatakeys.h \
    metadata/tikamimekeys.h \
    metadata/metadata.h \
    metadata/dateformat.h \
    utils/dateutils.h \
    metadata/dublincore.h \
    detect/detector.h \
    detect/namedetector.h \
    detect/magicdetector.h \
    mime/mimetype.h \
    mime/clause.h \
    mime/magic.h \
    mime/mediatyperegistry.h \
    mime/mimetypes.h \
    mime/patterns.h \
    exception/tikaexception.h \
    mime/mimetypeexception.h \
    detect/xmlrootextractor.h \
    detect/textdetector.h \
    detect/textstatistics.h \
    mime/mimetypesfactory.h \
    mimetypesreadermetkeys.h \
    mime/mimetypesreader.h \
    mime/orclause.h \
    mime/andclause.h \
    mime/magicmatch.h

#PRECOMPILED_HEADER = qtika-core-global.h

win32: QMAKE_CXXFLAGS_WARN_ON = /W4

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# qtika-internal
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtika-internal/release/ -lqtika-internal
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtika-internal/debug/ -lqtika-internal
else:unix: LIBS += -L$$OUT_PWD/../qtika-internal/ -lqtika-internal

INCLUDEPATH += $$PWD/../qtika-internal
DEPENDPATH += $$PWD/../qtika-internal

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtika-internal/release/libqtika-internal.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtika-internal/debug/libqtika-internal.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtika-internal/release/qtika-internal.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtika-internal/debug/qtika-internal.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qtika-internal/libqtika-internal.a
