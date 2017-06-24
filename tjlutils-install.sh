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
    #suRemoveFile "$ui/tjlutils.h"
    #suRemoveFile "$ui/systecommand.h"
    #suRemoveFile "$ui/generalutilities.h"
    #suRemoveFile "$ui/fileutilities.h"
    #suRemoveFile "$ui/mathutilities.h"
    #suRemoveFile "$ui/datetime.h"
    #suRemoveFile "$ui/templateobjects.h"
    #suRemoveFile "$ui/serialport.h"
    #suRemoveFile "$ui/eventtimer.h"
    #suRemoveFile "$ui/prettyprinter.h"
    #suRemoveFile "$ui/udpduplex.h"
    #suRemoveFile "$ui/ibytestream.h"
    #suRemoveFile "$ui/stringformat.h"
    echo "All cleaned up"
}

ui="/usr/include"
ul="/usr/lib"
olt="/opt/LibraryBuilds/tjlutils"
sourceDir="/opt/GitHub/tjlutils"
buildDir="/opt/LibraryBuilds/tjlutils"
ub="/usr/bin"

function displayHelp() {
    echo "Usage: tjlutils-install.sh [--install/--uninstall]"
}

cygwinCheck=$(uname -a | grep -i 'cygwin')

if [[ -z "$cygwinCheck" ]]; then
   if [[ "$EUID" != "0" ]]; then
      SUDO=sudo
   fi
else
    SUDO=""
fi

function showSuccess() {
    echo "success"
}

function showFailure() {
    echo "failure"
    cleanUp
}

function suRemoveFile() {
    echo -n "Removing \"$1\"..."
    rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function suLinkFile() {
    echo -n "Linking \"$1\" to \"$2\"..."
    ln -s -f "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function suRemoveFile() {
    echo -n "Removing \"$1\"..."
    $SUDO rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function suLinkFile() {
    echo -n "Linking \"$1\" to \"$2\"..."
    $SUDO ln -s -f "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}


function copyFile() {
    echo -n "Copying \"$1\" to \"$2\"..."
    cp -R "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function appendStringToFile() {
    echo -n "Putting string \"$1\" into file $2..."
    echo "$1" >> "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function changeDirectory() {
    echo -n "Entering directory \"$1\"..."
    cd "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function createDirectory() {
    echo -n "Creating directory \"$1\"..."
    mkdir "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function runGitClone() {
    echo -n "Cloning \"$1\" using git..."
    git clone "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else 
        showSuccess
        return 0
    fi
}

function runCmake() {
    echo -n "Running cmake from source directory \"$1\"..."
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

function bailout() {
    rm -rf "$buildDir"
}

function generateDesktopFile() {
    copyFile "$utilityDir/$skeletonDesktopFileName" "$buildDir/$desktopFileName" || { echo "Failed to generate desktop file, exiting"; exit 1; }
    copyFile "$iconPath" "$buildDir/" || { echo "Failed to generate desktop file, exiting"; exit 1; }
    appendStringToFile "Exec=$buildDir$programName" "$buildDir/$desktopFileName"
    appendStringToFile "Icon=$buildDir$iconName" "$buildDir/$desktopFileName"
}

function retrieveDependancy() {
    createDirectory "$buildDir/lib" || { echo "Failed to make dependancy lib directory, bailing out"; exit 1; }
    changeDirectory "$buildDir/lib"  || { echo "Failed to enter dependancy lib directory, bailing out"; exit 1; }
    runGitClone "$dependancySource" || { echo "Failed to retrieve dependancy source, bailing out"; exit 1; }
}

function buildDependancy() {
    sudo ./$depenancyInstallCommand || { echo "Failed to build dependancy, bailing out"; exit 1; }
}

if [[ "$1" == "-u" || "$1" == "--u" || "$1" == "-uninstall" || "$1" == "--uninstall" || "$1" == "uninstall" ]]; then
    
    suRemoveDirectory "$buildDir/" || { echo "Could not remove directory, bailing out"; exit 1;}
    suRemoveFile "$ui/systecommand.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/generalutilities.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/fileutilities.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/mathutilities.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/datetime.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/templateobjects.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/serialport.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/eventtimer.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/tcpclient.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/tcpduplex.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/tcpserver.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/udpduplex.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/ibytestream.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/stringformat.h" || { echo "Could not remove file, bailing out"; exit 1;}
    suRemoveFile "$ui/bitset.h" || { echo "Could not remove file, bailing out"; exit 1;}

    if [[ -z "$cygwinCheck" ]]; then
        suRemoveFile "$ul/libtjlutils.so" || { echo "Could not remove file, bailing out"; exit 1;}
        suRemoveFile "$ub/EnumerateSerial.exe" || { echo "Could not remove file, bailing out"; exit 1;}
    else
        suRemoveFile "$ul/cygtjlutils.dll" || { echo "Could not remove file, bailing out"; exit 1;}
    fi

    suRemoveFile "$ul/libtjlutils.a" || { echo "Could not remove file, bailing out"; exit 1;}

    echo "tjlutils uninstalled successfully"

elif [[ "$1" == "-h" || "$1" == "--h" || "$1" == "-help" || "$1" == "--help" || "$1" == help ]]; then

    displayHelp
    exit 0

else
    trap bailout INT QUIT TERM
    if [[ $# -gt 0 ]]; then
        #if [[ "$1" == "--uninstall" ]]; then
        #    echo "Success"
        #    exit 0
        #fi
        buildDir="$1"
        if ! [[ -d "$buildDir" ]]; then
            createDirectory "$buildDir" || { echo "Unable to create build directory \"$buildDir\", exiting"; exit 1; }
        fi
    else
        #buildDir="$filePath/$buildDir"
        buildDir="$olt"
        if ! [[ -d "$buildDir" ]]; then
            createDirectory "$buildDir" || { echo "Unable to make build directory \"$buildDir\", exiting"; exit 1; }
        fi
    fi

    suLinkFile "$sourceDir/systemcommand/systemcommand.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/generalutilities/generalutilities.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/fileutilities/fileutilities.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/mathutilities/mathutilities.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/datetime/datetime.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/serialport/serialport.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/prettyprinter/prettyprinter.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/templateobjects/templateobjects.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/udpserver/udpserver.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/udpclient/udpclient.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/udpduplex/udpduplex.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tcpserver/tcpserver.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tcpclient/tcpclient.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tcpduplex/tcpduplex.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/stringformat/stringformat.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tstream/tstream.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tstream/tscriptexecutor.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/tstream/tscriptreader.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/eventtimer/eventtimer.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}
    suLinkFile "$sourceDir/bitset/bitset.h" "$ui/" || { echo "Could not link file, bailing out"; exit 1;}    
    
    changeDirectory "$buildDir" || { echo "Could not enter build directory, bailing out"; exit 1;}
    runCmake "$sourceDir" || { echo "Cmake failed, bailing out"; exit 1;}
    runMake || { echo "Make failed, bailing out"; exit 1;}

    if [[ -z "$cygwinCheck" ]]; then
        suLinkFile "$buildDir/libtjlutils.so" "$ul/" || { echo "Could not link file, bailing out"; exit 1;}
 
        suLinkFile "$sourceDir/serialport/cs-helper/EnumerateSerial.exe" "$ub/" || { echo "Could not link file, bailing out"; exit 1;}
    else
        suLinkFile "$buildDir/cygtjlutils.dll" "$ul/" || { echo "Could not link file, bailing out"; exit 1;}
    fi

    suLinkFile "$buildDir/libtjlutils.a" "$ul/" || { echo "Could not link file, bailing out"; exit 1;}

    echo "tjlutils installed successfully"
fi
