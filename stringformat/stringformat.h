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

template<typename ... Args>
std::string stringFormat(const std::string& format, Args ... args)
{
    size_t size{std::snprintf(nullptr, 0, format.c_str(), args ...) + 1};
    std::unique_ptr<char[]> stringBuffer{new char[size]}; 
    snprintf(stringBuffer.get(), size, format.c_str(), args ...);
    return std::string{stringBuffer.get(), stringBuffer.get() + size - 1}; 
}
