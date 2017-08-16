# Copyright 2013 Roman Kurbatov
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET = $$qtLibraryTarget(i2cTest)
include(../plugin.pri)

DEFINES += I2CTEST_LIBRARY
QT += xml

SOURCES += \
	i2cTest.cpp \
	configurer.cpp

HEADERS += \
	i2cTest.h \
	configurer.h

OTHER_FILES += \
	i2cTest.xml

QMAKE_POST_LINK = "cp -f i2cTest.xml $$DESTDIR"
