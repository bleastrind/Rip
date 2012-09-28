TEMPLATE = app
TARGET = RipEmurator
QT += core \
    gui \
    network
HEADERS += rawsender.h \
    winpcapsender.h \
    ripcore.h \
    rippackage.h \
    rippackagelistener.h \
    ripinfosmodel.h \
    ripemurator.h
SOURCES += rawsender.cpp \
    winpcapsender.c \
    ripcore.cpp \
    rippackagelistener.cpp \
    ripinfosmodel.cpp \
    main.cpp \
    ripemurator.cpp
FORMS += ripemurator.ui
INCLUDEPATH += "F:\resourse\MyWorks\Internet\WpdPack_4_1_1\WpdPack\Include"
LIBS += -L"F:\resourse\MyWorks\Internet\WpdPack_4_1_1\WpdPack\Lib" \
    -l \
    wpcap -l ws2_32\

RESOURCES += 