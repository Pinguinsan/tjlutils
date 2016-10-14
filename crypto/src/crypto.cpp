#include "crypto.h"

Crypto::Crypto()
{

}

std::string Crypto::getSHA256Hash(const std::string &stringToHash)
{
    return Crypto::getHash(HashType::SHA256, stringToHash);
}

std::string Crypto::getSHA512Hash(const std::string &stringToHash)
{
    return Crypto::getHash(HashType::SHA512, stringToHash);
}

std::string Crypto::getHash(HashType hashType, const std::string &stringToHash)
{
    //TODO: Implement
    if (hashType == HashType::SHA256) {
        return "";
    } else if (hashType == HashType::SHA512) {
        unsigned char *out = static_cast<unsigned char *>( malloc(HASH_BYTES) );
        if (!out) {
            std::cout << "Error allocating memory for output buffer from Crypto::getSHA512Hash";
            return "";
        }
        unsigned char *in = static_cast<unsigned char *>( malloc(sizeof(stringToHash.c_str()) ) );
        if (!in) {
            std::cout << "Error allocating memory for input buffer from Crpto::getHash512";
        }
        memcpy(in, stringToHash.c_str(), sizeof(stringToHash.c_str()));
        //strcpy( static_cast<char *>(in), stringToHash.c_str() );
        int returnVal{crypto_hash_sha512(out, in, static_cast<unsigned long long>(stringToHash.length()))};
        (void) returnVal;
        char *temp{(char *) malloc(sizeof(out))};
        memcpy(temp, out, sizeof(out));
        const char *otherTemp{temp};
        return static_cast<std::string>(otherTemp);
    } else {
        throw std::runtime_error("Unknown hash type requested");
    }
}

Crypto::~Crypto()
{

}
