#include <iostream>
#include "eventtimer.h"
#include "stringformat.h"

void delaySomeTime()
{
    //Pretend we're doing some calculation
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main()
{
    EventTimer<std::chrono::steady_clock> *steadyClockEventTimer{new EventTimer<std::chrono::steady_clock>()};
    steadyClockEventTimer->start();
    std::cout << stringFormat("Began event timer with std::chrono::steady_clock") << std::endl;
    delaySomeTime();
    std::cout << stringFormat("After calculation, steadyClockEventTimer->toString() = %s", steadyClockEventTimer->toString().c_str()) << std::endl << std::endl;
    delete steadyClockEventTimer;

    EventTimer<std::chrono::system_clock> *systemClockEventTimer = new EventTimer<std::chrono::system_clock>();
    systemClockEventTimer->start();
    std::cout << stringFormat("Began event timer with std::chrono::system_clock") << std::endl;
    delaySomeTime();
    std::cout << stringFormat("After calculation, systemClockEventTimer->toString() = %s", systemClockEventTimer->toString().c_str()) << std::endl << std::endl;
    delete systemClockEventTimer;

    EventTimer<std::chrono::high_resolution_clock>* highResolutionClockEventTimer = new EventTimer<std::chrono::high_resolution_clock>();
    highResolutionClockEventTimer->start();
    std::cout << stringFormat("Began event timer with std::chrono::high_resolution_clock") << std::endl;
    delaySomeTime();
    std::cout << stringFormat("After calculation, highResolutionClockEventTimer->toString() = %s", highResolutionClockEventTimer->toString().c_str()) << std::endl << std::endl;
    delete highResolutionClockEventTimer;

    return 0;
}
