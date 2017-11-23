QT += core widgets
#CONFIG += precompile_header ordered
CONFIG += c++11
PRECOMPILED_HEADER = $$PWD/common/precompile.h
INCLUDEPATH += $$PWD/common $$[QT_INSTALL_HEADERS]/../trikRuntime
CONFIG(debug, debug | release) {
	CONFIGURATION = debug
} else {
	CONFIGURATION = release
}

QMAKE_CXXFLAGS += -Wall -Wextra

DESTDIR=$$PWD/build/$$CONFIGURATION

OBJECTS_DIR = .build/$$CONFIGURATION/obj
MOC_DIR = .build/$$CONFIGURATION/moc

LIBS += -ltrikTestCommon -L$$DESTDIR -ltrikControl
LIBS -= -l$$TARGET
