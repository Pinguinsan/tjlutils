/***********************************************************************
*    stringformat.h:                                                   *
*    Template function for using printf formatting for std::string     *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the template declaration for format function(s)   *
*    It can be used to enable a printf style of formatting for standard*
*    library style strings (std::string), improving one-line string    *
*    generation for use in exceptions, writing to files, etc           *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/
#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <regex>
#include <algorithm>
#include <utility>


template <typename T>
std::string toStdString(const T &rhs)
{
    std::stringstream stringStream{};
    stringStream << rhs;
    return stringStream.str();
}

/*snprintf style*/
template<typename ... Args>
std::string PStringFormat(const char *format, Args ... args)
{
    ssize_t size = std::snprintf(nullptr, 0, format, args ...) + 1;
    std::unique_ptr<char[]> stringBuffer{new char[size]};
    snprintf(stringBuffer.get(), size, format, args ...);
    return std::string{stringBuffer.get(), stringBuffer.get() + size - 1};
}

/*Base case to break recursion*/
std::string TStringFormat(const char *formatting)
{
    return std::string{formatting};
}

/*C# style String.Format()*/
template <typename First, typename ... Args>
std::string TStringFormat(const char *formatting, First first, Args ... args)
{
    static const std::regex targetRegex{"\\{[0-9]+\\}"};
    std::smatch match;
    std::string returnString{formatting};
    std::string copyString{returnString};

    /*[Absolute position, length]*/
    std::vector<std::pair<size_t, size_t>> foundPositions{};

    int smallestValue{-1};
    size_t smallestValuePosition{0};
    int smallestValueLength{0};

    /*Iterate through string, finding position and lengths of all matches {x}*/
    while(std::regex_search(copyString, match, targetRegex)) {
        size_t foundPosition{match.position() + (returnString.length() - copyString.length())};
        int regexMatchNumericValue{0};
        try {
            regexMatchNumericValue = std::stoi(returnString.substr(foundPosition + 1, (foundPosition + match.str().length())));
            if (regexMatchNumericValue < 0) {
                throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
            }
            if ((regexMatchNumericValue < smallestValue) || (smallestValue == -1)) {
                smallestValue = regexMatchNumericValue;
                smallestValuePosition = foundPosition;
                smallestValueLength = match.str().length();
            }
        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }

        copyString = match.suffix();
    }
    if (smallestValue == -1) {
        throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
    }
    size_t sizeOfToken{toStdString(smallestValue).length() + 2};
    returnString = returnString.substr(0, smallestValuePosition)
                   + toStdString(first)
                   + returnString.substr(smallestValuePosition + sizeOfToken);
    return TStringFormat(returnString.c_str(), args...);
}

#endif //STRINGFORMAT_H