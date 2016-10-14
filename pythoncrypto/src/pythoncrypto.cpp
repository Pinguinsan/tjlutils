#include "pythoncrypto.h"

const std::string PythonCrypto::SYSTEM_PYTHON_COMMAND{"python "};
const std::string PythonCrypto::PYTHON_CRYPTO_FILE_NAME{"crypto.py"};
const std::string PythonCrypto::SHA256_SWITCH{"--sha256 "};
const std::string PythonCrypto::SHA512_SWITCH{"--sha512 "};
const std::string PythonCrypto::PYTHON_INSTALLED_QUERY_STRING{"python --help"};
#ifdef _WIN32
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
    _filename{PYTHON_CRYPTO_FILE_NAME},
    _rng{MathUtilities::randomlySeededMersenneTwister()}
{
    if (!pythonInstalled()) {
        throw std::runtime_error("Python is not installed in the path");
    }
}

std::string PythonCrypto::getHash(const std::string &hashSwitch, const std::string &stringToHash)
{
    this->_filename = GeneralUtilities::toString(this->_rng()) + PYTHON_CRYPTO_FILE_NAME;
    generatePythonCryptoScript(this->_filename);
    SystemCommand systemCommand{SYSTEM_PYTHON_COMMAND + this->_filename + " " + hashSwitch  + GeneralUtilities::tQuoted(stringToHash)};
    std::string hashedString = systemCommand.executeAndWaitForOutputAsString();
    int tempRemove = std::remove(this->_filename.c_str());
    if (tempRemove) {
        std::cout << "WARNING: Failed to remove " << GeneralUtilities::tQuoted(this->_filename) << ", this file can be removed manually" << std::endl;
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
