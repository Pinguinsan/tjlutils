/***********************************************************************
*    pythoncrypto.h:                                                   *
*    Namespace PythonCrypto, for using Python hashing tools            *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a PythonCrypto namespace    *
*    It is used to enable expose some Python hashing tools, allowing   *
*    one to pass strings to a Python function and receive a hash back, *
*    such as sha256, enabling cryptographically strong security        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_PYTHONCRYTPO_H
#define TJLUTILS_PYTHONCRYTPO_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>
#include <cstdlib>
#include "systemcommand.h"
#include "fileutilities.h"
#include "generalutilities.h"
#include "mathutilities.h"

class PythonCrypto
{
public:
    PythonCrypto();
    ~PythonCrypto();
    std::string getSHA256Hash(const std::string &stringToHash);
    std::string getSHA512Hash(const std::string &stringToHash);
    void generatePythonCryptoScript(const std::string &outputFilename);
    bool pythonInstalled();

private:
    std::string m_filename;
    std::mt19937 m_rng;

    std::string getHash(const std::string &hashSwitch, const std::string &stringToHash);
	static const std::string SYSTEM_PYTHON_COMMAND;
	static const std::string PYTHON_CRYPTO_FILE_NAME;
	static const std::string SHA256_SWITCH;
	static const std::string SHA512_SWITCH;
	static const std::string PYTHON_INSTALLED_QUERY_STRING;
	static const std::string PYTHON_NOT_INSTALLED_IDENTIFIER_STRING;
	static const std::vector<std::string> pythonCryptoScriptSource;

};

#endif //TJLUTILS_PYTHONCRYTPO_H

