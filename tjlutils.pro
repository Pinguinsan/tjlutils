#-------------------------------------------------
#
# Project created by QtCreator 2016-08-05T09:39:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = tjlutils
TEMPLATE = lib

DEFINES += TJLUTILS_LIBRARY

CONFIG += static_and_shared build_all c++14

INCLUDEPATH += systemcommand/ \
               templateobjects/ \
               mathutilities/ \
               stringformat/ \
               fileutilities/ \
               generalutilities/ \
               serialport/ \
               eventtimer/ \
               datetime/ \
               prettyprinter/ \
               udpduplex/ \
               ibytestream/

SOURCES += systemcommand/systemcommand.cpp \
           generalutilities/generalutilities.cpp \
           fileutilities/fileutilities.cpp \
           mathutilities/mathutilities.cpp \
           datetime/datetime.cpp \
           serialport/serialport.cpp \
           udpduplex/udpduplex.cpp \
           prettyprinter/prettyprinter.cpp \
           ibytestream/ibytestream.cpp \

HEADERS += systemcommand/systemcommand.h \
           mathutilities/mathutilities.h \
           fileutilities/fileutilities.h \
           generalutilities/generalutilities.h \
           datetime/datetime.h \
           serialport/serialport.h \
           eventtimer/eventtimer.h \
           prettyprinter/prettyprinter \
           udpduplex/udpduplex.h \
           templateobjects/templateobjects.h \
           bitset/bitset.h \
           stringformat/stringformat.h \
           ibytestream/ibytestream.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    #POST_LINK_SCRIPT = C:/Cygwin64/opt/GitHub/tjlutils/copy-headers-windows.sh
    #CYGWIN_DIR = C:/Cygwin64/bin/mintty.exe
    #QMAKE_POST_LINK += $$quote(cmd $$quote($${CYGWIN_DIR} $$quote($${POST_LINK_SCRIPT})))
}
