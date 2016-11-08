#include "../include/shortwatchdog.h"

ShortWatchdog::ShortWatchdog(const std::vector<std::pair<GPIO*, bool>> &shorts) :
    m_shorts{shorts},
    m_shortEvent{false},
    m_failSafes{}
{
    
}

ShortWatchdog::ShortWatchdog(const std::vector<std::pair<GPIO*, bool>> &shorts, const std::set<FailSafe> &failSafes) :
    m_shorts{shorts},
    m_shortEvent{false},
    m_failSafes{failSafes}
{
    
}

std::vector<std::pair<GPIO*, bool>> ShortWatchdog::shorts() const
{
    return this->m_shorts;
}

std::set<FailSafe> ShortWatchdog::failSafes() const
{
    return this->m_failSafes;
}


bool ShortWatchdog::shortEvent() const
{
    return this->m_shortEvent;
}

void ShortWatchdog::reset()
{
    this->m_shortEvent = false;
}

void ShortWatchdog::executeFailSafe()
{
    for (auto &it : this->m_failSafes) {
        if (it.gpio()->ioType() != IOType::DIGITAL_OUTPUT) {
            it.gpio()->setIOType(IOType::DIGITAL_OUTPUT);
        }
        it.gpio()->g_digitalWrite(it.state());
    }
}

void ShortWatchdog::addFailSafe(const FailSafe &failSafe)
{
    this->m_failSafes.insert(failSafe);
}

void ShortWatchdog::removeFailSafe(const FailSafe &failSafe)
{
    this->m_failSafes.erase(failSafe);
}

void ShortWatchdog::checkForShorts()
{
    bool shortEvent{true};
    for (auto &it : this->m_shorts) {
        if (it.first->g_digitalRead() != it.second) {
            shortEvent = false;
        }
    }
    this->m_shortEvent = shortEvent;
}