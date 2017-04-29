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
               pythoncrypto/ \
               crypto/ \
               templateobjects/ \
               mathutilities/ \
               fileutilities/ \
               generalutilities/ \
               serialport/ \
               eventtimer/ \
               datetime/ \
               prettyprinter/ \
               arduino/ \
               udpserver/ \
               udpclient/ \
               udpduplex/ \
               common/

SOURCES += systemcommand/src/systemcommand.cpp \
           pythoncrypto/src/pythoncrypto.cpp \
           crypto/src/crypto.cpp \
           generalutilities/src/generalutilities.cpp \
           fileutilities/src/fileutilities.cpp \
           mathutilities/src/mathutilities.cpp \
           datetime/src/datetime.cpp \
           serialport/src/serialport.cpp \
           eventtimer/src/eventtimer.cpp \
           arduino/src/arduino.cpp \
           udpserver/src/udpserver.cpp \
           udpclient/src/udpclient.cpp \
           udpduplex/src/udpduplex.cpp \
           prettyprinter/src/prettyprinter.cpp

HEADERS += systemcommand/systemcommand.h \
           pythoncrypto/pythoncrypto.h \
           mathutilities/mathutilities.h \
           fileutilities/fileutilities.h \
           generalutilities/generalutilities.h \
           datetime/datetime.h \
           serialport/serialport.h \
           eventtimer/eventtimer.h \
           prettyprinter/prettyprinter \
           udpserver/udpserver.h \
           udpclient/updclient.h \
           udpduplex/udpduplex.h \
           templateobjects/templateobjects.h \
           bitset/bitset.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    #POST_LINK_SCRIPT = C:/Cygwin64/opt/GitHub/tjlutils/copy-headers-windows.sh
    #CYGWIN_DIR = C:/Cygwin64/bin/mintty.exe
    #QMAKE_POST_LINK += $$quote(cmd $$quote($${CYGWIN_DIR} $$quote($${POST_LINK_SCRIPT})))
}
