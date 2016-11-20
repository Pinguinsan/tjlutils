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

INCLUDEPATH += systemcommand/include/ \
               pythoncrypto/include/ \
               crypto/include/ \
               templateobjects/include/ \
               mathutilities/include/ \
               fileutilities/include/ \
               generalutilities/include/ \
               serialport/include/ \
               eventtimer/include/ \
               datetime/include/ \
               prettyprinter/include/ \
               arduino/include/ \
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
           prettyprinter/src/prettyprinter.cpp

HEADERS += systemcommand/include/systemcommand.h \
           pythoncrypto/include/pythoncrypto.h \
           crypto/include/crypto.h \
           crypto/include/hash.h \
           mathutilities/include/mathutilities.h \
           fileutilities/include/fileutilities.h \
           generalutilities/include/generalutilities.h \
           datetime/include/datetime.h \
           serialport/include/serialport.h \
           eventtimer/include/eventtimer.h \
           prettyprinter/include/prettyprinter \
           arduino/include/arduino.h \
           templateobjects/templateobjects.h \
           common/tjlutils.h            

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    POST_LINK_SCRIPT = C:/Cygwin64/opt/GitHub/tjlutils/copy-headers-windows.sh
    CYGWIN_DIR = C:/Cygwin64/bin/mintty.exe
    QMAKE_POST_LINK += $$quote(cmd $$quote($${CYGWIN_DIR} $$quote($${POST_LINK_SCRIPT})))
}
