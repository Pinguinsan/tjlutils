#ifndef TJLUTILS_COMMANDLINEPARSER_H
#define TJLUTILS_COMMANDLINEPARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

enum class CommandLineOptionModifier {
    SINGLE,
    MULTIPLE
};

enum class CommandLineOptionType {
    INTEGER,
    BOOLEAN,
    STRING,
    DIRECTORY,
    FILE
};

class CommandLineOption
{
public:
    CommandLineOption(CommandLineOptionType commandLineOptionType, CommandLineOptionModifier commandLineOptionModifier = CommandLineOptionModifier::SINGLE);   

    CommandLineOptionType type() const { return this->m_commandLineOptionType; }
    bool multipleEntries() const { return this->m_commandLineOptionModifier == CommandLineOptionModifier::MULTIPLE; }

    void setType(CommandLineOptionType commandLineOptionType) { this->m_commandLineOptionType = commandLineOptionType; }
    void setMultipleEntries(bool multiple) { this->m_commandLineOptionModifier = CommandLineOptionModifier::MULTIPLE; }

private:
    CommandLineOptionType m_commandLineOptionType;
    CommandLineOptionModifier m_commandLineOptionModifier;
};

class CommandLineParser
{
public:

private:
    std::map<int, CommandLineOption> m_commandLineOptions;
};


#endif //TJLUTILS_COMMANDLINEPARSER_H
