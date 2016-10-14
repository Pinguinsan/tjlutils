/***********************************************************************
*    eventtimer.h:                                                     *
*    Class EventTimer, for timing events and getting calculated times  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an EventTimer class           *
*    EventTimer is used to time events that need to be able to pause   *
*    The class also allows access to calculated out hours/minutes/etc, *
*    suitable for a running game timer (eg 00:01.295)                  *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_EVENTTIMER_H
#define TJLUTILS_EVENTTIMER_H

#include <chrono>
#include <string>
#include "generalutilities.h"
#include <mathutilities.h>

class EventTimer
{
public:
    EventTimer();

    void start();
    void pause();
    void unpause();
    void update();

    long long int totalTimeMicroseconds() const;
    long long int totalTimeMilliseconds() const;
    long long int totalTimeSeconds() const;
    long long int totalTimeMinutes() const;
    long long int totalTimeHours() const;
    long long int hours() const;
    long long int minutes() const;
    long long int seconds() const;
    long long int milliseconds() const;
    bool isPaused() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    std::chrono::milliseconds m_rawTime;
    long long int m_totalTime;
    long long int m_hours;
    long long int m_minutes;
    long long int m_seconds;
    long long int m_milliseconds;
    bool m_isPaused;
};

#endif //TJLUTILS_EVENTTIMER_H
