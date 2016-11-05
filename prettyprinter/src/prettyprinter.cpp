#include "prettyprinter.h"

const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_BASE{"\033["};
const char *PrettyPrinter::TERMINAL_COLOR_ESCAPE_SEQUENCE_TAIL{"m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_COLOR{"\033[39m"};
const std::vector<TerminalColor> PrettyPrinter::s_TERMINAL_COLORS_CONTAINER{TerminalColor::FG_BLACK,
                                                                            TerminalColor::FG_RED,
                                                                            TerminalColor::FG_GREEN, 
                                                                            TerminalColor::FG_YELLOW, 
                                                                            TerminalColor::FG_BLUE, 
                                                                            TerminalColor::FG_MAGENTA, 
                                                                            TerminalColor::FG_CYAN, 
                                                                            TerminalColor::FG_LIGHT_GRAY, 
                                                                            TerminalColor::FG_DEFAULT,
                                                                            TerminalColor::FG_DARK_GRAY, 
                                                                            TerminalColor::FG_LIGHT_RED, 
                                                                            TerminalColor::FG_LIGHT_GREEN, 
                                                                            TerminalColor::FG_LIGHT_YELLOW, 
                                                                            TerminalColor::FG_LIGHT_BLUE, 
                                                                            TerminalColor::FG_LIGHT_MAGENTA, 
                                                                            TerminalColor::FG_LIGHT_CYAN, 
                                                                            TerminalColor::FG_WHITE, 
                                                                            TerminalColor::BG_RED, 
                                                                            TerminalColor::BG_GREEN, 
                                                                            TerminalColor::BG_BLUE, 
                                                                            TerminalColor::BG_DEFAULT};

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

TerminalColor PrettyPrinter::randomColor()
{
    using namespace MathUtilities;
    return PrettyPrinter::s_TERMINAL_COLORS_CONTAINER.at(MathUtilities::randomBetween(0, PrettyPrinter::s_TERMINAL_COLORS_CONTAINER.size()-1));
}