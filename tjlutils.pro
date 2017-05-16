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
               tstream/

SOURCES += systemcommand/systemcommand.cpp \
           pythoncrypto/pythoncrypto.cpp \
           generalutilities/generalutilities.cpp \
           fileutilities/fileutilities.cpp \
           mathutilities/mathutilities.cpp \
           datetime/datetime.cpp \
           serialport/serialport.cpp \
           udpserver/udpserver.cpp \
           udpclient/udpclient.cpp \
           udpduplex/udpduplex.cpp \
           prettyprinter/prettyprinter.cpp \
           tstream/tstream.cpp \
           tstream/tscriptreader.cpp \
           tstream/tscriptexecutor.cpp

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
           bitset/bitset.h \
           tstream/tstream.h \
           tstream/tscriptreader.h \
           tstream/tscriptexecutor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    #POST_LINK_SCRIPT = C:/Cygwin64/opt/GitHub/tjlutils/copy-headers-windows.sh
    #CYGWIN_DIR = C:/Cygwin64/bin/mintty.exe
    #QMAKE_POST_LINK += $$quote(cmd $$quote($${CYGWIN_DIR} $$quote($${POST_LINK_SCRIPT})))
}
