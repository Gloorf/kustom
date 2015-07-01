SOURCES += \
    main.cpp \
    odt_reader.cpp \
    xml_writer.cpp

HEADERS += \
    odt_reader.h \
    xml_writer.h
 QT += xml
QMAKE_CXXFLAGS += -std=c++11
include(../kustom-sheet-creator/kustom-sheet-creator.pri)
