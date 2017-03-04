#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <stringformat.h>

int main()
{
    std::random_device randomDevice{};
    std::mt19937 randomEngine{randomDevice()};
    std::uniform_int_distribution<> randomGenerator{1, 1000};

    int randomNumber{randomGenerator(randomEngine)};
    std::string hellow{"stringlol"};
    size_t stringLength{strlen(hellow.c_str())};
    char myChar{'t'};

    std::cout << stringFormat("randomNumber = %i, hellow = \"%s\", stringLength = %zu, myChar = \"%c\"", randomNumber, hellow.c_str(), stringLength, myChar) << std::endl;  
    return 0;
}
