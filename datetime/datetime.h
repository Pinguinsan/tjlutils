/***********************************************************************
*    datetime.h:                                                       *
*    Namespace DateTime, for time/date stamps                          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a DateTime namespace          *
*    It is used to get time/date stamps in varying formats, as well as *
*    individual time/date components (current minute, etc)             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_DATETIME_H
#define TJLUTILS_DATETIME_H

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
#include <sstream>
#include <algorithm>
#include <ctime>
#include "generalutilities.h"

namespace DateTime
{
    std::string currentYear();
    std::string currentMonth();
    std::string currentMonthName();
    std::string monthName(int monthIndex);
    int monthIndex(const std::string &monthName);
    std::string currentDay();
    std::string currentHour12();
    std::string currentHour24();
    std::string currentMinute();
    std::string currentSecond();
    std::string timeStamp12();
    std::string timeStamp24();
    std::string dateStampMDY();
    std::string dateStampDMY();
    std::string getOverallStampDMY12();
    std::string getOverallStampDMY24();
    std::string getOverallStampMDY12();
    std::string getOverallStampMDY24();
}

#endif //TJLUTILS_DATETIME_H
