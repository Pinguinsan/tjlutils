/***********************************************************************
*    pythoncrypto.cpp:                                                 *
*    Namespace PythonCrypto, for using Python hashing tools            *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a PythonCrypto namespace    *
*    It is used to enable expose some Python hashing tools, allowing   *
*    one to pass strings to a Python function and receive a hash back, *
*    such as sha256, enabling cryptographically strong security        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "pythoncrypto.h"

const std::string PythonCrypto::SYSTEM_PYTHON_COMMAND{"python "};
const std::string PythonCrypto::PYTHON_CRYPTO_FILE_NAME{"crypto.py"};
const std::string PythonCrypto::SHA256_SWITCH{"--sha256 "};
const std::string PythonCrypto::SHA512_SWITCH{"--sha512 "};
const std::string PythonCrypto::PYTHON_INSTALLED_QUERY_STRING{"python --help"};
#if defined(_WIN32)
    const std::string PythonCrypto::PYTHON_NOT_INSTALLED_IDENTIFIER_STRING{"is not recognized"};
#else
    const std::string PythonCrypto::PYTHON_NOT_INSTALLED_IDENTIFIER_STRING{"command not found"};
#endif

const std::vector<std::string> PythonCrypto::pythonCryptoScriptSource{"#!/bin/python",\
                                                                      "import hashlib",\
                                                                       "import sys",\
                                                                       "def sha256Hash(text):",\
                                                                       "    return str(hashlib.sha256(text.encode('utf-8')).hexdigest())",\
                                                                       "def sha512Hash(text):",\
                                                                       "    return str(hashlib.sha512(text.encode('utf-8')).hexdigest())",\
                                                                       "if (sys.argv[1] == \"--sha256\"):\n"\
                                                                       "    print(sha256Hash(sys.argv[2]))",\
                                                                      "elif (sys.argv[1] == \"--sha512\"):\n"\
                                                                       "    print(sha512Hash(sys.argv[2]))"};

PythonCrypto::PythonCrypto() :
    m_filename{PYTHON_CRYPTO_FILE_NAME},
    m_rng{}
{
    if (!pythonInstalled()) {
        throw std::runtime_error("Python is not installed in the path");
    }
}

std::string PythonCrypto::getHash(const std::string &hashSwitch, const std::string &stringToHash)
{
    this->m_filename = GeneralUtilities::toString(this->m_rng()) + PYTHON_CRYPTO_FILE_NAME;
    generatePythonCryptoScript(this->m_filename);
    SystemCommand systemCommand{SYSTEM_PYTHON_COMMAND + this->m_filename + " " + hashSwitch  + GeneralUtilities::tQuoted(stringToHash)};
    std::string hashedString = systemCommand.executeAndWaitForOutputAsString();
    int tempRemove = std::remove(this->m_filename.c_str());
    if (tempRemove) {
        std::cout << "WARNING: Failed to remove " << GeneralUtilities::tQuoted(this->m_filename) << ", this file can be removed manually" << std::endl;
    }
    return hashedString;
}

std::string PythonCrypto::getSHA256Hash(const std::string &stringToHash)
{
    return getHash(SHA256_SWITCH, stringToHash);
}

std::string PythonCrypto::getSHA512Hash(const std::string &stringToHash)
{
    return getHash(SHA512_SWITCH, stringToHash);
}

void PythonCrypto::generatePythonCryptoScript(const std::string &outputFilename)
{
    std::ofstream writeToFile;
    writeToFile.open(outputFilename);
    if (writeToFile.is_open()) {
        for (std::vector<std::string>::const_iterator iter = pythonCryptoScriptSource.begin(); iter != pythonCryptoScriptSource.end(); iter++) {
            writeToFile << *iter << std::endl;
        }
    } else {
        std::cout << "WARNING: Could not open file " << GeneralUtilities::tQuoted(outputFilename) << " to generate python crypto script (this may cause other operations to fail)" << std::endl;
    }
}

bool PythonCrypto::pythonInstalled()
{
    SystemCommand systemCommand{PYTHON_INSTALLED_QUERY_STRING};
    std::vector<std::string> returnVector{systemCommand.executeAndWaitForOutputAsVector()};
    return ((returnVector.size() > 1) && (std::begin(returnVector)->find(PYTHON_NOT_INSTALLED_IDENTIFIER_STRING) == std::string::npos));
}

PythonCrypto::~PythonCrypto()
{

}
