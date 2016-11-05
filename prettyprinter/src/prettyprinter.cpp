/***********************************************************************
*    prettyprinter.cpp:                                                *
*    PrettyPrinter class, for printing colored text to a POSIX terminal*
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a PrettyPrinter class       *
*    It is used to print colored text to a POSIX terminal              *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "prettyprinter.h"

const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE{"\033["};
const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL{"m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_FOREGROUND_COLOR{"\033[39m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_BACKGROUND_COLOR{"\033[49m"};
const std::vector<ForegroundColor> PrettyPrinter::s_FOREGROUND_TERMINAL_COLORS_CONTAINER{ForegroundColor::FG_BLACK,
                                                                                        ForegroundColor::FG_RED,
                                                                                        ForegroundColor::FG_GREEN, 
                                                                                        ForegroundColor::FG_YELLOW, 
                                                                                        ForegroundColor::FG_BLUE, 
                                                                                        ForegroundColor::FG_MAGENTA, 
                                                                                        ForegroundColor::FG_CYAN, 
                                                                                        ForegroundColor::FG_LIGHT_GRAY, 
                                                                                        ForegroundColor::FG_DEFAULT,
                                                                                        ForegroundColor::FG_DARK_GRAY, 
                                                                                        ForegroundColor::FG_LIGHT_RED, 
                                                                                        ForegroundColor::FG_LIGHT_GREEN, 
                                                                                        ForegroundColor::FG_LIGHT_YELLOW, 
                                                                                        ForegroundColor::FG_LIGHT_BLUE, 
                                                                                        ForegroundColor::FG_LIGHT_MAGENTA, 
                                                                                        ForegroundColor::FG_LIGHT_CYAN, 
                                                                                        ForegroundColor::FG_WHITE};

const std::vector<BackgroundColor> PrettyPrinter::s_BACKGROUND_TERMINAL_COLORS_CONTAINER{BackgroundColor::BG_RED, 
                                                                                        BackgroundColor::BG_GREEN, 
                                                                                        BackgroundColor::BG_BLUE, 
                                                                                        BackgroundColor::BG_DEFAULT};

PrettyPrinter::PrettyPrinter() :
    m_foregroundColor{ForegroundColor::FG_DEFAULT},
    m_backgroundColor{BackgroundColor::BG_DEFAULT},
    m_outputStream{&std::cout}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor) :
    m_foregroundColor{foregroundColor},
    m_backgroundColor{BackgroundColor::BG_DEFAULT},
    m_outputStream{&std::cout}
{

}

PrettyPrinter::PrettyPrinter(BackgroundColor backgroundColor) :
    m_foregroundColor{ForegroundColor::FG_DEFAULT},
    m_backgroundColor{backgroundColor},
    m_outputStream{&std::cout}
{

}

PrettyPrinter::PrettyPrinter(std::ostream *outputStream) :
    m_foregroundColor{ForegroundColor::FG_DEFAULT},
    m_backgroundColor{BackgroundColor::BG_DEFAULT},
    m_outputStream{outputStream}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, std::ostream *outputStream) :
    m_foregroundColor{foregroundColor},
    m_backgroundColor{BackgroundColor::BG_DEFAULT},
    m_outputStream{outputStream}
{

}

PrettyPrinter::PrettyPrinter(BackgroundColor backgroundColor, std::ostream *outputStream) :
    m_foregroundColor{ForegroundColor::FG_DEFAULT},
    m_backgroundColor{backgroundColor},
    m_outputStream{outputStream}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, BackgroundColor backgroundColor, std::ostream *outputStream) :
    m_foregroundColor{foregroundColor},
    m_backgroundColor{backgroundColor},
    m_outputStream{outputStream}
{

}

void PrettyPrinter::setForegroundColor(ForegroundColor foregroundColor)
{
    this->m_foregroundColor = foregroundColor;
}

void PrettyPrinter::setBackgroundColor(BackgroundColor backgroundColor)
{
    this->m_backgroundColor = backgroundColor;
}


void PrettyPrinter::setOutputStream(std::ostream *outputStream)
{
    this->m_outputStream = outputStream;
}

ForegroundColor PrettyPrinter::foregroundColor() const
{
    return this->m_foregroundColor;
}

BackgroundColor PrettyPrinter::backgroundColor() const
{
    return this->m_backgroundColor;
}

std::ostream *PrettyPrinter::outputStream()
{
    return this->m_outputStream;
}

ForegroundColor PrettyPrinter::randomForegroundColor()
{
    using namespace MathUtilities;
    return PrettyPrinter::s_FOREGROUND_TERMINAL_COLORS_CONTAINER.at(MathUtilities::randomBetween(0, PrettyPrinter::s_FOREGROUND_TERMINAL_COLORS_CONTAINER.size()-1));
}

BackgroundColor PrettyPrinter::randomBackgroundColor()
{
    using namespace MathUtilities;
    return PrettyPrinter::s_BACKGROUND_TERMINAL_COLORS_CONTAINER.at(MathUtilities::randomBetween(0, PrettyPrinter::s_BACKGROUND_TERMINAL_COLORS_CONTAINER.size()-1));
}

void PrettyPrinter::resetStreamToDefault(std::ostream *oStream)
{
    *oStream << PrettyPrinter::DEFAULT_TERMINAL_BACKGROUND_COLOR << PrettyPrinter::DEFAULT_TERMINAL_FOREGROUND_COLOR;
}

void PrettyPrinter::resetBackgroundColor()
{
    this->m_backgroundColor = BackgroundColor::BG_DEFAULT;
}

void PrettyPrinter::resetForegroundColor()
{
    this->m_foregroundColor = ForegroundColor::FG_DEFAULT;
}