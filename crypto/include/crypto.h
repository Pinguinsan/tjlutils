/***********************************************************************
*    crypto.h:                                                         *
*    User defined class, Crypto, for using hashing tools               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations for a Crypto class               *
*    The class is made up entirely of static methods and constants     *
*    It is used to enable expose some Python hashing tools, allowing   *
*    one to pass strings to a function and receive a hash back,        *
*    such as sha256, enabling cryptographically strong security        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_CRYPTO_H
#define TJLUTILS_CRYPTO_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include "hash.h"

#define HASH_BYTES 64

enum class HashType {
    SHA256,
    SHA512
};

class Crypto
{
public:
    Crypto();
    ~Crypto();
    std::string getSHA256Hash(const std::string &stringToHash);
    std::string getSHA512Hash(const std::string &stringToHash);
private:
    std::string getHash(HashType hashType, const std::string &stringToHash);
};

#endif //TJLUTILS_CRYPTO_H
