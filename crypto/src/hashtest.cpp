#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <pythoncrypto.h>
#include <crypto.h>

int main()
{
    std::string str{"Hello, World!"};

    std::unique_ptr<PythonCrypto> pythonCrypto{new PythonCrypto};;
    std::unique_ptr<Crypto> crypto{new Crypto};

    std::cout << "String: " << str << std::endl;
    std::cout << "PythonCrypto hash: " << pythonCrypto->getSHA512Hash(str) << std::endl;
    std::cout << "Crypto hash: " << crypto->getSHA512Hash(str) << std::endl;
    return 0;

}
