/***********************************************************************
*    datetime.cpp:                                                     *
*    Namespace DateTime, for time/date stamps                          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a DateTime namespace        *
*    It is used to get time/date stamps in varying formats, as well as *
*    individual time/date components (current minute, etc)             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "datetime.h"

namespace DateTime
{
    std::string currentYear()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        return GeneralUtilities::toString(now->tm_year + 1900);
    }

    std::string currentMonth()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        return GeneralUtilities::toString(now->tm_mon + 1);
    }

    std::string currentMonthName()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int currentMonth{(now->tm_mon) + 1};
        switch (currentMonth) {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
        default: throw std::logic_error("Invalid argument " + GeneralUtilities::tQuoted(currentMonth) + " retrieved from localtime()");
        }
    }

    std::string monthName(int monthIndex)
    {
        switch (monthIndex) {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
            default: throw std::invalid_argument("Invalid argument " + GeneralUtilities::tQuoted(monthIndex) + " passed to monthName");
        }
    }

    int monthIndex(const std::string &monthName)
    {
        std::string copyString{monthName};
        std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
        if (copyString == "january") {
            return 0;
        } else if (copyString == "february") {
            return 1;
        } else if (copyString == "march") {
            return 2;
        } else if (copyString == "april") {
            return 3;
        } else if (copyString == "may") {
            return 4;
        } else if (copyString == "june") {
            return 5;
        } else if (copyString == "july") {
            return 6;
        } else if (copyString == "august") {
            return 7;
        } else if (copyString == "september") {
            return 8;
        } else if (copyString == "october") {
            return 9;
        } else if (copyString == "november") { 
            return 10;
        } else if (copyString == "december") {
            return 11;
        } else {
            throw std::invalid_argument("Invalid argument " + GeneralUtilities::tQuoted(copyString) + " passed to monthIndex");
        }
    }

    std::string currentDay()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int currentDay{now->tm_mday};
        std::string returnDay{GeneralUtilities::toString(currentDay)};
        if (currentDay < 10) {
            returnDay.insert(returnDay.begin(), '0');
        }
        return returnDay;
    }

    std::string currentHour12()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        std::string AMPM{""};
        int currentHour{now->tm_hour};
        if (currentHour > 24) {
            currentHour -= 12;
        }
        std::string returnHour{GeneralUtilities::toString(currentHour)};
        if (currentHour < 10) {
            returnHour.insert(returnHour.begin(), '0');
        }
        return returnHour;
    }

    std::string currentHour24() 
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int currentHour{now->tm_hour};
        std::string returnHour{GeneralUtilities::toString(currentHour)};
        if (currentHour < 10) {
            returnHour.insert(returnHour.begin(), '0');
        }
        return returnHour;
    }

    std::string currentMinute()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int currentMinute{now->tm_min};
        std::string returnMinute{GeneralUtilities::toString(currentMinute)};
        if (currentMinute < 10) {
            returnMinute.insert(returnMinute.begin(), '0');
        }
        return returnMinute;
    }

    std::string currentSecond()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int currentSecond{now->tm_sec};
        std::string returnSecond{GeneralUtilities::toString(currentSecond)};
        if (currentSecond < 10) {
            returnSecond.insert(returnSecond.begin(), '0');
        }
        return returnSecond;
    }

    std::string timeStamp12()
    {
        time_t t{time(0)};
        struct tm *now{localtime(&t)};
        int tempHour{now->tm_hour};
        if (tempHour > 12) {
            tempHour += 100;
        }
        std::string AMPM{""};
        if (tempHour >= 100) {
            AMPM = "PM";
            tempHour -= 112;
        } else {
            AMPM = "AM";
        }
        std::string returnHour{GeneralUtilities::toString(tempHour)};
        if (tempHour < 10) {
            returnHour.insert(returnHour.begin(), '0');
        }
        return (returnHour + ':' + currentMinute() + ':' + currentSecond() + AMPM);
    }

    std::string timeStamp24()
    {
        return (currentHour24() + ':' + currentMinute() + ':' + currentDay());
    }

    std::string dateStampMDY()
    {
        return (currentMonth() + '-' + currentDay() + '-' + currentYear());
    }

    std::string dateStampDMY()
    {
        return (currentDay() + '-' + currentMonth() + '-' + currentYear());
    }

    std::string getOverallStampDMY12()
    {
        return (dateStampDMY() + ' ' + timeStamp12());
    }

    std::string getOverallStampDMY24()
    {
        return (dateStampDMY() + ' ' + timeStamp24());
    }

    std::string getOverallStampMDY12()
    {
        return (dateStampMDY() + ' ' + timeStamp12());
    }

    std::string getOverallStampMDY24()
    {
        return (dateStampMDY() + ' ' + timeStamp24());
    }
}
