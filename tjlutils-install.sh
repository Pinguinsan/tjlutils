#!/bin/bash

##########################################
# tjlutils-install.sh
#
# Intended to be a placeholder until 
# I learn how to add a sudo make 
# install/uninstall to CMake
#
##########################################

function displayHelp() {
    echo "Usage: tjlutils-install.sh [--install/--uninstall]"
}

cygwinCheck=$(uname -a | grep -i 'cygwin')

if [[ -z "$cygwinCheck" ]]; then
   if [[ "$EUID" != "0" ]]; then
      echo "This script must be run as root, as it involves deleting and symlinking to the /usr/include and /usr/lib/ directories"
      exit 1
   fi
fi

ui="/usr/include"
ul="/usr/lib"
ogt="/opt/GitHub/tjlutils"
olt="/opt/LibraryBuilds/tjlutils"
ub="/usr/bin"

if [[ "$1" == "-u" || "$1" == "--u" || "$1" == "-uninstall" || "$1" == "--uninstall" || "$1" == "uninstall" ]]; then
    
    rm -rf "$olt"
    rm -f "$ui/tjlutils.h"
    rm -f "$ui/systecommand.h"
    rm -f "$ui/generalutilities.h"
    rm -f "$ui/fileutilities.h"
    rm -f "$ui/mathutilities.h"
    rm -f "$ui/datetime.h"
    rm -f "$ui/pythoncrypto.h"
    rm -f "$ui/crypto.h"
    rm -f "$ui/hash.h"
    rm -f "$ui/templateobjects.h"
    rm -f "$ui/serialport.h"
    rm -f "$ui/eventtimer.h"
    rm -f "$ui/prettyprinter.h"

    if [[ -z "$cygwinCheck" ]]; then
        rm -f "$ul/libtjlutils.so"
        rm -f "$ub/EnumerateSerial.exe"
    else
        rm -f "$ul/cygtjlutils.dll"
    fi

    rm -f "$ul/libtjlutils.a"

elif [[ "$1" == "-h" || "$1" == "--h" || "$1" == "-help" || "$1" == "--help" || "$1" == help ]]; then

    displayHelp
    exit 0

else
    
    rm -rf "$olt"
    mkdir "$olt"
    cd "$olt" || exit 1
    cmake "$ogt" || exit 1
    make || exit 1

    rm -f "$ui/systemcommand.h"
    ln -s "$ogt/systemcommand/include/systemcommand.h" "$ui/"
    
    rm -f "$ui/generalutilities.h"
    ln -s "$ogt/generalutilities/include/generalutilities.h" "$ui/"

    rm -f "$ui/fileutilities.h"
    ln -s "$ogt/fileutilities/include/fileutilities.h" "$ui/"
    
    rm -f "$ui/mathutilities.h"
    ln -s "$ogt/mathutilities/include/mathutilities.h" "$ui/"
    
    rm -f "$ui/datetime.h"
    ln -s "$ogt/datetime/include/datetime.h" "$ui/"
    
    rm -f "$ui/pythoncrypto.h"
    ln -s "$ogt/pythoncrypto/include/pythoncrypto.h" "$ui/"
   
    rm -f "$ui/crypto.h"
    rm -f "$ui/hash.h"
    ln -s "$ogt/crypto/include/crypto.h" "$ui/"
    ln -s "$ogt/crypto/include/hash.h" "$ui/"
    
    rm -f "$ui/serialport.h"
    ln -s "$ogt/serialport/include/serialport.h" "$ui/"

    rm -f "$ui/eventtimer.h"
    ln -s "$ogt/eventtimer/include/eventtimer.h" "$ui/"

    rm -f "$ui/prettyprinter.h"
    ln -s "$ogt/prettyprinter/include/prettyprinter.h" "$ui/"

    rm -f "$ui/templateobjects.h"
    ln -s "$ogt/templateobjects/templateobjects.h" "$ui/"

    rm -f "$ui/tjlutils.h"
    ln -s "$ogt/common/tjlutils.h" "$ui/"


    if [[ -z "$cygwinCheck" ]]; then
        rm -f "$ul/libtjlutils.so"
        ln -s "$olt/libtjlutils.so" "$ul/"

        rm -f "$ub/EnumerateSerial.exe"
        ln -s "$ogt/serialport/cs-helper/EnumerateSerial.exe" "$ub/"
    else
        rm -f "$ul/cygtjlutils.dll"
        ln -s "$olt/cygtjlutils.dll" "$ul/"
    fi

    rm -f "$ul/libtjlutils.a"
    ln -s "$olt/libtjlutils.a" "$ul/"
fi
