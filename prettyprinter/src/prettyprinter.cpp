#include "prettyprinter.h"

const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE{"\033["};
const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL{"m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_COLOR{"\033[39m"};

PrettyPrinter::PrettyPrinter() :
    m_terminalColor{TerminalColor::FG_DEFAULT},
    m_outputStream{&std::cout}
{

}

PrettyPrinter::PrettyPrinter(TerminalColor terminalColor) :
    m_terminalColor{terminalColor},
    m_outputStream{&std::cout}
{

}

PrettyPrinter::PrettyPrinter(std::ostream *outputStream) :
    m_terminalColor{TerminalColor::FG_DEFAULT},
    m_outputStream{outputStream}
{

}

PrettyPrinter::PrettyPrinter(TerminalColor terminalColor, std::ostream *outputStream) :
    m_terminalColor{terminalColor},
    m_outputStream{outputStream}
{

}

PrettyPrinter::PrettyPrinter(std::ostream *outputStream, TerminalColor terminalColor) :
    m_terminalColor{terminalColor},
    m_outputStream{outputStream}
{

}

void PrettyPrinter::setColor(TerminalColor terminalColor)
{
    this->m_terminalColor = terminalColor;
}

void PrettyPrinter::setOutputStream(std::ostream *outputStream)
{
    this->m_outputStream = outputStream;
}

TerminalColor PrettyPrinter::color() const
{
    return this->m_terminalColor;
}

std::ostream *PrettyPrinter::outputStream()
{
    return this->m_outputStream;
}