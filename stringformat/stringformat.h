#include <iostream>
#include <memory>
#include <string>
#include <cstdio>

template<typename ... Args>
std::string stringFormat(const std::string& format, Args ... args)
{
    size_t size{std::snprintf(nullptr, 0, format.c_str(), args ...) + 1};
    std::unique_ptr<char[]> stringBuffer{new char[size]}; 
    snprintf(stringBuffer.get(), size, format.c_str(), args ...);
    return std::string{stringBuffer.get(), stringBuffer.get() + size - 1}; 
}
