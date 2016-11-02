#ifndef TJUTILS_PRETTYPRINTER_H
#define TJUTILS_PRETTYPRINTER_H

#include <iostream>
#include <ostream>

enum TerminalColor {
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
    FG_WHITE         = 97, 
    BG_RED           = 41, 
    BG_GREEN         = 42, 
    BG_BLUE          = 44, 
    BG_DEFAULT       = 49
};

class PrettyPrinter 
{
public:
    PrettyPrinter();
    PrettyPrinter(TerminalColor terminalColor);
    PrettyPrinter(std::ostream *outputStream);
    PrettyPrinter(TerminalColor terminalColor, std::ostream *outputStream);
    PrettyPrinter(std::ostream *outputStream, TerminalColor terminalColor);
    void setColor(TerminalColor terminalColor);
    void setOutputStream(std::ostream *outputStream);
    TerminalColor color() const;
    std::ostream *outputStream();

    template<typename T>
    void operator<<(const T &toPrint)
    {
        return this->print(toPrint, *this->m_outputStream, this->m_terminalColor);
    }

    template <typename T>
    void print(const T &toPrint)
    {
        return this->print(toPrint, this->m_terminalColor, *this->m_outputStream);
    }

    template <typename T>
    void print(const T& toPrint, TerminalColor terminalColor)
    {
        return this->print(toPrint, terminalColor, *this->m_outputStream);
    }

    template <typename T>
    void print(const T& toPrint, std::ostream &outputStream)
    {
        return this->print(toPrint, this->m_terminalColor, outputStream); 
    }

    template <typename T>
    void print(const T& toPrint, std::ostream &outputStream, TerminalColor terminalColor)
    {
        return this->print(toPrint, terminalColor, outputStream);
    }

    template <typename T>
    void print(const T& toPrint, TerminalColor terminalColor, std::ostream &outputStream)
    {
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << terminalColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL << toPrint;
    }

    template <typename T>
    void println(const T &toPrint)
    {
        return this->println(toPrint, this->m_terminalColor, *this->m_outputStream);
    }

    template <typename T>
    void println(const T& toPrint, TerminalColor terminalColor)
    {
        return this->println(toPrint, terminalColor, *this->m_outputStream);
    }

    template <typename T>
    void println(const T& toPrint, std::ostream &outputStream)
    {
        return this->println(toPrint, this->m_terminalColor, outputStream); 
    }

    template <typename T>
    void println(const T& toPrint, std::ostream &outputStream, TerminalColor terminalColor)
    {
        return this->println(toPrint, terminalColor, outputStream);
    }

    template <typename T>
    void println(const T& toPrint, TerminalColor terminalColor, std::ostream &outputStream)
    {
        outputStream << TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE << terminalColor << TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL << toPrint << std::endl;
    }

    void println()
    {
        return this->println("");
    }

private:
    TerminalColor m_terminalColor;
    std::ostream *m_outputStream;
    static const char *TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE;
    static const char *TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL;
};

#endif //TJUTILS_PRETTYPRINTER_H