/***********************************************************************
*    prettyprinter.h:                                                  *
*    PrettyPrinter class, for printing colored text to a POSIX terminal*
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a PrettyPrinter class         *
*    It is used to print colored text to a POSIX terminal              *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/


#ifndef TJUTILS_PRETTYPRINTER_H
#define TJUTILS_PRETTYPRINTER_H

#include <iostream>
#include <ostream>
#include <vector>
#include <mathutilities.h>

enum ForegroundColor {
    FG_BLACK         = 30,
    FG_RED           = 31,
    FG_GREEN         = 32, 
    FG_YELLOW        = 33, 
    FG_BLUE          = 34, 
    FG_MAGENTA       = 35, 
    FG_CYAN          = 36, 
    FG_LIGHT_GRAY    = 37, 
    FG_DEFAULT       = 39,
    FG_DARK_GRAY     = 90, 
    FG_LIGHT_RED     = 91, 
    FG_LIGHT_GREEN   = 92, 
    FG_LIGHT_YELLOW  = 93, 
    FG_LIGHT_BLUE    = 94, 
    FG_LIGHT_MAGENTA = 95, 
    FG_LIGHT_CYAN    = 96, 
    FG_WHITE         = 97
};

enum BackgroundColor {
    BG_RED           = 41, 
    BG_GREEN         = 42, 
    BG_BLUE          = 44, 
    BG_DEFAULT       = 49
};

class PrettyPrinter 
{
public:
    PrettyPrinter();
    PrettyPrinter(std::ostream *outputStream);
    PrettyPrinter(ForegroundColor ForegroundColor);
    PrettyPrinter(BackgroundColor backgroundColor);
    PrettyPrinter(ForegroundColor ForegroundColor, BackgroundColor backgroundColor);
    PrettyPrinter(ForegroundColor ForegroundColor, std::ostream *outputStream);
    PrettyPrinter(BackgroundColor BackgroundColor, std::ostream *outputStream);
    PrettyPrinter(ForegroundColor ForegroundColor, BackgroundColor backgroundColor, std::ostream *outputStream);
    void setForegroundColor(ForegroundColor ForegroundColor);
    void setBackgroundColor(BackgroundColor backgroundColor);
    void setOutputStream(std::ostream *outputStream);
    ForegroundColor foregroundColor() const;
    BackgroundColor backgroundColor() const;
    std::ostream *outputStream();

    static ForegroundColor randomForegroundColor();
    static BackgroundColor randomBackgroundColor();
    void resetBackgroundColor();
    void resetForegroundColor();
    
    template<typename T>
    void operator<<(const T &toPrint)
    {
        return this->print(toPrint, *this->m_outputStream, this->m_foregroundColor, this->m_backgroundColor);
    }

    template <typename T>
    void print(const T &toPrint)
    {
        return this->print(toPrint, this->m_foregroundColor, this->m_backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void print(const T& toPrint, ForegroundColor foregroundColor)
    {
        return this->print(toPrint, foregroundColor, this->m_backgroundColor, *this->m_outputStream);
    }
        
    template <typename T>
    void print(const T& toPrint, BackgroundColor backgroundColor)
    {
        return this->print(toPrint, this->m_foregroundColor, backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void print(const T& toPrint, ForegroundColor foregroundColor, BackgroundColor backgroundColor)
    {
        return this->print(toPrint, foregroundColor, backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void print(const T& toPrint, std::ostream &outputStream)
    {
        return this->print(toPrint, this->m_foregroundColor, this->m_backgroundColor, outputStream); 
    }

    template <typename T>
    void print(const T& toPrint, ForegroundColor foregroundColor, std::ostream &outputStream)
    {
        return this->print(toPrint, foregroundColor, this->m_backgroundColor, outputStream); 
    }

    template <typename T>
    void print(const T& toPrint, BackgroundColor backgroundColor, std::ostream &outputStream)
    {
        return this->print(toPrint, this->m_foregroundColor, backgroundColor, outputStream); 
    }

    template <typename T>
    void print(const T& toPrint, ForegroundColor foregroundColor, BackgroundColor backgroundColor, std::ostream &outputStream)
    {
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << foregroundColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << backgroundColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
        outputStream << toPrint;
        this->resetStreamToDefault(&outputStream);
    }

    template <typename T>
    void println(const T &toPrint)
    {
        return this->println(toPrint, this->m_foregroundColor, this->m_backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void println(const T& toPrint, ForegroundColor foregroundColor)
    {
        return this->println(toPrint, foregroundColor, this->m_backgroundColor, *this->m_outputStream);
    }
        
    template <typename T>
    void println(const T& toPrint, BackgroundColor backgroundColor)
    {
        return this->println(toPrint, this->m_foregroundColor, backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void println(const T& toPrint, ForegroundColor foregroundColor, BackgroundColor backgroundColor)
    {
        return this->println(toPrint, foregroundColor, backgroundColor, *this->m_outputStream);
    }

    template <typename T>
    void println(const T& toPrint, std::ostream &outputStream)
    {
        return this->println(toPrint, this->m_foregroundColor, this->m_backgroundColor, outputStream); 
    }

    template <typename T>
    void println(const T& toPrint, ForegroundColor foregroundColor, std::ostream &outputStream)
    {
        return this->println(toPrint, foregroundColor, this->m_backgroundColor, outputStream); 
    }

    template <typename T>
    void println(const T& toPrint, BackgroundColor backgroundColor, std::ostream &outputStream)
    {
        return this->println(toPrint, this->m_foregroundColor, backgroundColor, outputStream); 
    }

    template <typename T>
    void println(const T& toPrint, ForegroundColor foregroundColor, BackgroundColor backgroundColor, std::ostream &outputStream)
    {
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << foregroundColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << backgroundColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
        outputStream << toPrint;
        outputStream << std::endl;
        this->resetStreamToDefault(&outputStream);
    }

    void println()
    {
        return this->println("");
    }

private:
    ForegroundColor m_foregroundColor;
    BackgroundColor m_backgroundColor;
    std::ostream *m_outputStream;

    void resetStreamToDefault(std::ostream *oStream);
    static const char *TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE;
    static const char *TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
    static const char *DEFAULT_TERMINAL_FOREGROUND_COLOR;
    static const char *DEFAULT_TERMINAL_BACKGROUND_COLOR;

    static const std::vector<ForegroundColor> s_FOREGROUND_TERMINAL_COLORS_CONTAINER;
    static const std::vector<BackgroundColor> s_BACKGROUND_TERMINAL_COLORS_CONTAINER;
};

#endif //TJUTILS_PRETTYPRINTER_H