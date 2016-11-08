#ifndef ARDUINOPC_SHORTWATCHDOG_H
#define ARDUINOPC_SHORTWATCHDOG_H

#include "failsafe.h"
#include "gpio.h"

/* 
 * Standard C++ headers
 * Special thanks to maniacbug for the
 * nice port of the c++ stdlib 
 * https://github.com/maniacbug/StandardCplusplus
 */
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <set>
#include <vector>

enum ShortWatchdogHeaderPosition { FIRST_PIN, FIRST_STATE, SECOND_PIN, SECOND_STATE };

class ShortWatchdog
{
public:
    ShortWatchdog(const std::vector<std::pair<GPIO*, bool>> &shorts);
    ShortWatchdog(const std::vector<std::pair<GPIO*, bool>> &shorts, const std::set<FailSafe> &failSafes);
    std::vector<std::pair<GPIO*, bool>> shorts() const;
    void reset();
    void executeFailSafe();
    void checkForShorts();
    void addFailSafe(const FailSafe &failSafe);
    void removeFailSafe(const FailSafe &failSafe);
    bool secondState() const;
    bool shortEvent() const;
    std::set<FailSafe> failSafes() const;

    friend bool operator==(const ShortWatchdog &lhs, const ShortWatchdog &rhs)
    {
        if (lhs.m_shorts.size() != rhs.m_shorts.size()) {
            return false;
        }
        for (unsigned int i = 0; i < lhs.m_shorts.size(); i++) {
            if (lhs.m_shorts.at(i).first->pinNumber() != rhs.m_shorts.at(i).first->pinNumber()) {
                return false;
            }
            if (lhs.m_shorts.at(i).second != rhs.m_shorts.at(i).second) {
                return false;
            }
        }
    }

    friend bool operator<(const ShortWatchdog &lhs, const ShortWatchdog &rhs)
    {  
        return lhs.m_shorts.size() < rhs.m_shorts.size();
    }

private:
    std::vector<std::pair<GPIO*, bool>> m_shorts;
    bool m_shortEvent;
    std::set<FailSafe> m_failSafes;
};

#endif //ARDUINOPC_SHORTWATCHDOG_H