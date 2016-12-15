#!/bin/bash

##########################################
# tjlutils-install.sh
#
# Intended to be a placeholder until 
# I learn how to add a sudo make 
# install/uninstall to CMake
#
##########################################

function cleanUp() {
    removeFile "$ui/tjlutils.h"
    removeFile "$ui/systecommand.h"
    removeFile "$ui/generalutilities.h"
    removeFile "$ui/fileutilities.h"
    removeFile "$ui/mathutilities.h"
    removeFile "$ui/datetime.h"
    removeFile "$ui/pythoncrypto.h"
    removeFile "$ui/crypto.h"
    removeFile "$ui/hash.h"
    removeFile "$ui/templateobjects.h"
    removeFile "$ui/serialport.h"
    removeFile "$ui/eventtimer.h"
    removeFile "$ui/prettyprinter.h"
    removeFile "$ui/arduino.h"
    removeFile "$ui/udpserver.h"
    removeFile "$ui/udpclient.h"
}

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

function showSuccess() {
    echo "success"
}

function showFailure() {
    echo "failure"
    cleanUp
}

function removeFile() {
    echo -n "Removing $1..."
    rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function linkFile() {
    echo -n "Linking $1 to $2..."
    ln -s -f "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function changeDirectory() {
    echo -n "Entering directory to $1..."
    cd "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function runCmake() {
    echo -n "Running cmake from source directory $1..."
    cmake "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function runMake() {
    echo -n "Running make..."
    make
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

if [[ "$1" == "-u" || "$1" == "--u" || "$1" == "-uninstall" || "$1" == "--uninstall" || "$1" == "uninstall" ]]; then
    
    removeFile "$olt"
    removeFile "$ui/tjlutils.h"
    removeFile "$ui/systecommand.h"
    removeFile "$ui/generalutilities.h"
    removeFile "$ui/fileutilities.h"
    removeFile "$ui/mathutilities.h"
    removeFile "$ui/datetime.h"
    removeFile "$ui/pythoncrypto.h"
    removeFile "$ui/crypto.h"
    removeFile "$ui/hash.h"
    removeFile "$ui/templateobjects.h"
    removeFile "$ui/serialport.h"
    removeFile "$ui/eventtimer.h"
    removeFile "$ui/prettyprinter.h"
    removeFile "$ui/arduino.h"
    removeFile "$ui/udpserver.h"
    removeFile "$ui/udpclient.h"

    if [[ -z "$cygwinCheck" ]]; then
        removeFile "$ul/libtjlutils.so"
        removeFile "$ub/EnumerateSerial.exe"
    else
        removeFile "$ul/cygtjlutils.dll"
    fi

    removeFile "$ul/libtjlutils.a"

    echo "tjlutils uninstalled successfully"

elif [[ "$1" == "-h" || "$1" == "--h" || "$1" == "-help" || "$1" == "--help" || "$1" == help ]]; then

    displayHelp
    exit 0

else

    linkFile "$ogt/systemcommand/include/systemcommand.h" "$ui/"
    linkFile "$ogt/generalutilities/include/generalutilities.h" "$ui/"
    linkFile "$ogt/fileutilities/include/fileutilities.h" "$ui/"
    linkFile "$ogt/mathutilities/include/mathutilities.h" "$ui/"
    linkFile "$ogt/datetime/include/datetime.h" "$ui/"
    linkFile "$ogt/pythoncrypto/include/pythoncrypto.h" "$ui/"
    linkFile "$ogt/crypto/include/crypto.h" "$ui/"
    linkFile "$ogt/crypto/include/hash.h" "$ui/"
    linkFile "$ogt/serialport/include/serialport.h" "$ui/"
    linkFile "$ogt/eventtimer/include/eventtimer.h" "$ui/"
    linkFile "$ogt/prettyprinter/include/prettyprinter.h" "$ui/"
    linkFile "$ogt/templateobjects/templateobjects.h" "$ui/"
    linkFile "$ogt/arduino/include/arduino.h" "$ui/"
    linkFile "$ogt/udpserver/include/udpserver.h" "$ui/"
    linkFile "$ogt/udpclient/include/udpclient.h" "$ui/"
    linkFile "$ogt/udpduplex/include/udpduplex.h" "$ui/"
    linkFile "$ogt/common/tjlutils.h" "$ui/"
    
    changeDirectory "$olt" || exit 1
    runCmake "$ogt" || exit 1
    runMake || exit 1

    if [[ -z "$cygwinCheck" ]]; then
        linkFile "$olt/libtjlutils.so" "$ul/"

        linkFile "$ogt/serialport/cs-helper/EnumerateSerial.exe" "$ub/"
    else
        linkFile "$olt/cygtjlutils.dll" "$ul/"
    fi

    linkFile "$olt/libtjlutils.a" "$ul/"

    echo "tjlutils installed successfully"
fi
