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

template<typename ... Args>
std::string PStringFormat(const char *format, Args ... args)
{
    ssize_t size = std::snprintf(nullptr, 0, format, args ...) + 1;
    std::unique_ptr<char[]> stringBuffer{new char[size]}; 
    snprintf(stringBuffer.get(), size, format, args ...);
    return std::string{stringBuffer.get(), stringBuffer.get() + size - 1}; 
}

std::string TStringFormat(const char *formatting)
{
    return std::string{formatting};
}

template <typename First, typename ... Args>
std::string TStringFormat(const char *formatting, First first, Args ... args)
{
    static const std::regex targetRegex{"\\{[0-9]+\\}"};
    std::smatch match;
    std::string returnString{formatting};
    std::string copyString{returnString};

    //Found position, length
    std::vector<std::pair<size_t, size_t>> foundPositions{};
    while(std::regex_search(copyString, match, targetRegex)) {
        size_t foundPosition{match.position() + (returnString.length() - copyString.length())};
        foundPositions.emplace_back(foundPosition, match.str().length());
        copyString = match.suffix();
    }
    if (foundPositions.empty()) {
        throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
    }
    //<{value}, position>
    std::vector<std::pair<int, size_t>> foundValues{};
    try {
        for (const auto &it : foundPositions) {
            //Find value in curly brackets, and convert to integer (
            foundValues.emplace_back(std::stoi(returnString.substr(it.first + 1, it.second - 2)), it.first);
        }
    } catch (std::exception &e) {
        throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
    }
    //Sort them
    std::sort(foundValues.begin(), foundValues.end(), [](const std::pair<int, size_t> &lhs, const std::pair<int, size_t> &rhs) { return lhs.first < rhs.first; });
    size_t sizeOfToken{toStdString(foundValues.at(0).first).length() + 2};
    returnString = returnString.substr(0, foundValues.at(0).second)
                   + toStdString(first)
                   + returnString.substr(foundValues.at(0).second + sizeOfToken);
    return TStringFormat(returnString.c_str(), args...);
}


