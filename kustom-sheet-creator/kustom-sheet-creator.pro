SOURCES += \
    main.cpp \
    main_window.cpp \
    data.cpp \
    generator.cpp \
    simulator.cpp \
    damage_calculator.cpp \
    odt_writer.cpp \
    dialog.cpp \
    character.cpp \
    xml_reader.cpp \
    data_type.cpp

HEADERS += \
    main_window.h \
    data.h \
    generator.h \
    simulator.h \
    damage_calculator.h \
    odt_writer.h \
    dialog.h \
    character.h \
    xml_reader.h \
    data_type.h

OTHER_FILES += \
    kustom-sheet-creator.pri \
    kustomsheetcreator.rc

RC_FILE= kustomsheetcreator.rc
QT += core gui widgets
