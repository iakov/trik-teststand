QT += core widgets
#CONFIG += precompile_header ordered
PRECOMPILED_HEADER = $$PWD/common/precompile.h
INCLUDEPATH += $$PWD/common $$[QT_INSTALL_HEADERS]/../trikRuntime
CONFIG(debug, debug | release) {
	CONFIGURATION = debug
} else {
	CONFIGURATION = release
}

DESTDIR=$$PWD/build/$$CONFIGURATION

OBJECTS_DIR = .build/$$CONFIGURATION/obj
MOC_DIR = .build/$$CONFIGURATION/moc


