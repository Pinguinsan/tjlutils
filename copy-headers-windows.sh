#!/bin/bash 

minDir="/cygdrive/c/Qt/5.7/mingw53_32/include/"
secondMinDir="/cygdrive/c/Qt/5.7/mingw53_32/lib/";
libDir="/opt/GitHub/tjlutils"
buildDir="/opt/LibraryBuilds/tjlutils/Debug/debug"
sysDir="/cygdrive/c/Windows/SysWOW64/"
binDir="/cygdrive/c/Users/Public/Public Programs/"

if ! [[ -d "$binDir" ]]; then
    mkdir "$binDir"
fi

rm -f "$minDir/systemcommand.h"
rm -f "$minDir/pythoncrypto.h"
rm -f "$minDir/crypto.h"
rm -f "$minDir/hash.h"
rm -f "$minDir/fileutilities.h"
rm -f "$minDir/mathutilities.h"
rm -f "$minDir/generalutilities.h"
rm -f "$minDir/datetime.h"
rm -f "$minDir/serialport.h"
rm -f "$minDir/eventtimer.h"
rm -f "$minDir/prettyprinter.h"
rm -f "$minDir/arduino.h"
rm -f "$minDir/udpserver.h"
rm -f "$binDir/EnumerateSerial.exe"

cp "$libDir/systemcommand/include/systemcommand.h" "$minDir"
cp "$libDir/pythoncrypto/include/pythoncrypto.h" "$minDir"
cp "$libDir/crypto/include/crypto.h" "$minDir"
cp "$libDir/crypto/include/hash.h" "$minDir"
cp "$libDir/fileutilities/include/fileutilities.h" "$minDir"
cp "$libDir/mathutilities/include/mathutilities.h" "$minDir"
cp "$libDir/generalutilities/include/generalutilities.h" "$minDir"
cp "$libDir/datetime/include/datetime.h" "$minDir"
cp "$libDir/templateobjects/templateobjects.h" "$minDir"
cp "$libDir/serialport/include/serialport.h" "$minDir"
cp "$libDir/eventtimer/include/eventtimer.h" "$minDir"
cp "$libDir/prettyprinter/include/prettyprinter.h" "$minDir"
cp "$libDir/arduino/include/arduino.h" "$minDir"
cp "$libDir/udpserver/include/udpserver.h" "$minDir"
cp "$libDir/common/tjlutils.h" "$minDir"
cp "$libDir/serialport/cs-helper/EnumerateSerial.exe" "$binDir"

cp "$buildDir/tjlutils.dll" "$secondMinDir"
cp "$buildDir/libtjlutils.a" "$secondMinDir"
