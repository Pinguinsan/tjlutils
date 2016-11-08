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
               #arduino/include/ \
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
           prettyprinter/src/prettyprinter.cpp
           #arduino/src/arduino.cpp \
           #arduino/src/arduinouno.cpp \
           #arduino/src/arduinonano.cpp \
           #arduino/src/arduinomega.cpp \
           #arduino/src/arduinofactory.cpp \
           #arduino/src/canmessage.cpp \
           #arduino/src/candatapacket.cpp

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
           #arduino/include/arduino.h \
           #arduino/include/arduinouno.h \
           #arduino/include/arduinonano.h \
           #arduino/include/arduinomega.h \
           #arduino/include/arduinofactory.h \
           #arduino/include/canmessage.h \
           #arduino/include/candatapacket.h \
           templateobjects/templateobjects.h \
           common/tjlutils.h            

unix {
    target.path = /usr/lib
    INSTALLS += target
}
