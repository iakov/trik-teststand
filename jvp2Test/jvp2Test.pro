# Copyright 2014 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. */

TARGET = $$qtLibraryTarget(jvp2Test)
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../common

CONFIG(debug, debug | release) {
	CONFIGURATION = debug
} else {
	CONFIGURATION = release
}

DESTDIR=../build/$$CONFIGURATION

OBJECTS_DIR = .build/$$CONFIGURATION/obj
MOC_DIR = .build/$$CONFIGURATION/moc

LIBS += -L../build/$$CONFIGURATION \
	-ltrikTestCommon

DEFINES += JVP2TEST_LIBRARY

SOURCES += jvp2Test.cpp

HEADERS += jvp2Test.h
