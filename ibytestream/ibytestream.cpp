/***********************************************************************
*    ibytestream.cpp:                                                  *
*    IByteStream, base class for simple read and write operations      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                          *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementaiton for the methods used in        *
*    IByteStreamScriptReader and IByteStreamScriptExecutor             * 
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "ibytestream.h"

/*
const char *DELAY_IDENTIFIER{"delay"};
const char *DELAY_SECONDS_IDENTIFIER{"delayseconds("};
const char *DELAY_MILLISECONDS_IDENTIFIER{"delaymilliseconds("};
const char *DELAY_MICROSECONDS_IDENTIFIER{"delaymicroseconds("};
const char *WRITE_IDENTIFIER{"write("};
const char *READ_IDENTIFIER{"read("};
const char *LOOP_IDENTIFIER{"loop("};
const char *FLUSH_IDENTIFIER{"flush"};
const char *NO_CLOSING_PARENTHESIS_FOUND_STRING{"    No matching parenthesis found, ignoring option"};
const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING{"    No matching quotation marks found, ingoring option"};
const char *NO_PARAMETER_SEPARATING_COMMA_STRING{"    No parameter separating comma found, ignoring option"};
const char *NO_LOOP_COUNT_SPECIFIED_STRING{"    Loop() parameter must include an integer parameter (the number of loops to execute), ignoring option"};
const char *EXPECTED_HERE_STRING{"^---expected here"};
const char *HERE_STRING{"^---here"};
const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING{"    Write() parameter must be enclosed in parentheses, ignoring option"};
const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelaySeconds() parameter is not an integer, ignoring option"};
const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMilliseconds() parameter is not an integer, ignoring option"};
const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMicroseconds() parameter is not an integer, ignoring option"};
const char *GENERIC_CONFIG_WARNING_BASE_STRING{"WARNING: line "};
const char *GENERIC_CONFIG_WARNING_TAIL_STRING{" of configuration file:"};
const char *CONFIG_EXPRESSION_MALFORMED_STRING{"    expression is malformed/has invalid syntax, ignoring option"};
const char *EXCEPTION_IN_CONSTRUCTOR_STRING{"WARNING: Standard exception caught in IByteStreamScriptFileReader constructor: "};
const char *COMMAND_TYPE_NOT_IMPLEMENTED_STRING{"WARNING: Command type not implemented, skipping command: "};
const char *NULL_IO_STREAM_PASSED_TO_EXECUTE_STRING{"WARNING: Null IByteStream passed to IByteStreamScriptExecutor::execute(std::shared_ptr<IByteStream>), skipping script execution"};
const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING{"WARNING: Script file does not exist, skipping script: "};
const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING{"WARNING: Unable to open script file, skipping script: "};
const char *LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING{"LoopCount() parameter is not an integer, ignoring option"};
const char *UNTERMINATED_LOOP_STRING{"WARNING: The script contains an unterminated loop,  skipping script execution"};
const char *UNEXPECTED_LOOP_CLOSING_STRING{"WARNING: A loop closure was found, but no loop was currently being populated, ignoring option"};
const char *CLOSING_LOOP_IDENTIFIER{"}"};
*/

#if defined(_WIN32) && !defined(__CYGWIN__)
    #include <Windows.h>
    #include "Shlwapi.h"
    #include <direct.h>
#else
    #include <sys/types.h>
    #include <sys/stat.h>
	#include <dirent.h>
    #include <unistd.h>
#endif

IByteStreamScriptReader::IByteStreamScriptReader(const std::string &scriptFilePath) :
    m_scriptFilePath{scriptFilePath},
    m_commands{std::make_shared<std::vector<IByteStreamCommand>>()}
{
    if (!fileExists(this->m_scriptFilePath)) {
        throw std::runtime_error(SCRIPT_FILE_DOES_NOT_EXISTS_STRING + tQuoted(this->m_scriptFilePath));
    }
    std::vector<std::string> buffer;
    std::ifstream readFromFile;
    readFromFile.open(this->m_scriptFilePath);
    if (readFromFile.is_open()) {
        std::string tempString{""};
        while (std::getline(readFromFile, tempString)) {
            buffer.emplace_back(tempString);
        }
        readFromFile.close();
    } else {
        throw std::runtime_error(UNABLE_TO_OPEN_SCRIPT_FILE_STRING + tQuoted(this->m_scriptFilePath));
    }
    buffer = trimWhitespace(buffer);
    int loops{0};
    int loopCount{0};
    for (std::vector<std::string>::const_iterator iter = buffer.begin(); iter != buffer.end(); iter++) {
        try {
            std::string copyString{*iter};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            //TODO: Replace with regex for searching
            size_t foundDelayPosition{copyString.find(DELAY_IDENTIFIER)};
            size_t foundWritePosition{copyString.find(WRITE_IDENTIFIER)};
            size_t foundReadPosition{copyString.find(READ_IDENTIFIER)};
            size_t foundFlushPosition{copyString.find(FLUSH_IDENTIFIER)};
            size_t foundLoopPosition{copyString.find(LOOP_IDENTIFIER)};
            size_t foundClosingLoopPosition{copyString.find(CLOSING_LOOP_IDENTIFIER)};
            if (copyString.length() != 0) {
                std::string otherCopy{copyString};
                unsigned int numberOfWhitespace{0};
                while (otherCopy.length() > 1 && isWhitespace(otherCopy[0])) {
                    stripFromString(otherCopy, ' ');
                    numberOfWhitespace++;
                }
                if (copyString.length() > numberOfWhitespace) {
                    if (copyString[numberOfWhitespace] == '#') {
                        continue;
                    }
                }
            } else {
                continue;
            }
            if (isWhitespace(copyString)) {
                continue;
            }

            long int currentLine{std::distance<std::vector<std::string>::const_iterator>(buffer.begin(), iter)+1};
            if (foundLoopPosition != std::string::npos) {
                std::string targetLoopCount{0};
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetLoopCount = getBetween("(", ")", copyString);
                if (trimWhitespace(targetLoopCount) == "") {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_LOOP_COUNT_SPECIFIED_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                } else {
                    try {
                        int temp{std::stoi(targetLoopCount)};
                        loopCount = temp > 0 ? temp : temp*-1;
                        loops++;
                        this->m_commands->emplace_back(IByteStreamCommandType::LOOP_START, ::toString(loopCount));
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->find(")")-1) << EXPECTED_HERE_STRING << std::endl;
                    }
                }
            } else if ((foundClosingLoopPosition != std::string::npos) && (trimWhitespace(copyString).length() == 1)) {
                if (loops == 0) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << UNEXPECTED_LOOP_CLOSING_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                } else {
                    loops--;
                    this->m_commands->emplace_back(IByteStreamCommandType::LOOP_END, "");
                } 
            } else if (foundDelayPosition != std::string::npos) {
                std::string targetDelay{""};
                std::string identifier{""};
                size_t foundDelaySecondsPosition{copyString.find(DELAY_SECONDS_IDENTIFIER)};
                size_t foundDelayMillisecondsPosition{copyString.find(DELAY_MILLISECONDS_IDENTIFIER)};
                size_t foundDelayMicrosecondsPosition{copyString.find(DELAY_MICROSECONDS_IDENTIFIER)};
                IByteStreamCommandType commandType{IByteStreamCommandType::COMMAND_UNSPECIFIED};
                if (foundDelaySecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_SECONDS_IDENTIFIER);
                    commandType = IByteStreamCommandType::DELAY_SECONDS;
                } else if (foundDelayMillisecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MILLISECONDS_IDENTIFIER);
                    commandType = IByteStreamCommandType::DELAY_MILLISECONDS;
                } else if (foundDelayMicrosecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MICROSECONDS_IDENTIFIER);
                    commandType = IByteStreamCommandType::DELAY_MICROSECONDS;
                } else {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                }
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetDelay = getBetween("(", ")", copyString);
                try {
                    long long int delay{::decStringToLongLong(targetDelay)};
                    this->m_commands->emplace_back(commandType, ::toString(delay));
                } catch (std::exception &e) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    if (commandType == IByteStreamCommandType::DELAY_SECONDS) {
                        std::cout << DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == IByteStreamCommandType::DELAY_MILLISECONDS) {
                        std::cout << DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == IByteStreamCommandType::DELAY_MICROSECONDS) {
                        std::cout << DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    }
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->find(")")-1) << EXPECTED_HERE_STRING << std::endl;
                }
            } else if (foundWritePosition != std::string::npos) {
                std::string targetString{""};
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetString = getBetween("(", ")", *iter);
                if (targetString.find_last_of("\"") == targetString.find("\"")) {
                    if (targetString.find(targetString.find("\"")) != std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_QUOTATION_MARKS_FOUND_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->find("\"")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    } else {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                }
                targetString = getBetween("\"", "\"", targetString);
                this->m_commands->emplace_back(IByteStreamCommandType::WRITE, targetString);
            } else if (foundReadPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                this->m_commands->emplace_back(IByteStreamCommandType::READ, "");
            } else if (foundFlushPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                this->m_commands->emplace_back(IByteStreamCommandType::FLUSH_RX, "");
            } else {
                std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                std::cout << *iter << std::endl;
                std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
            }
        } catch (std::exception &e) {
            std::cout << EXCEPTION_IN_CONSTRUCTOR_STRING << e.what() << std::endl;
            this->m_commands->clear();
            return;
        }
    }
    if (loops != 0) {
        std::cout << UNTERMINATED_LOOP_STRING << std::endl;
        this->m_commands->clear();
        return; 
    }
}


bool IByteStreamScriptReader::fileExists(const std::string &fileToCheck)
{
    #if defined(_WIN32)
        return (PathFileExists(directoryToCheck.c_str()) == 1);
    #else
        return (access(fileToCheck.c_str(),F_OK) != -1);
    #endif
}

bool IByteStreamScriptReader::fileExists(const char *fileToCheck)
{
    return fileExists(static_cast<std::string>(fileToCheck));
}

std::string IByteStreamScriptReader::scriptFilePath() const
{
    return this->m_scriptFilePath;
}

std::shared_ptr<std::vector<IByteStreamCommand>> IByteStreamScriptReader::commands() const
{
    return this->m_commands;
}

IByteStreamScriptExecutor::IByteStreamScriptExecutor(const std::string &tScriptFilePath) :
    m_tScriptReader{std::make_shared<IByteStreamScriptReader>(tScriptFilePath)}
{

}

std::string IByteStreamScriptExecutor::scriptFilePath() const
{
    return this->m_tScriptReader->scriptFilePath();
}

bool IByteStreamScriptExecutor::hasCommands() const
{
    return (this->m_tScriptReader->commands()->size() > 0);
}

size_t IByteStreamScriptExecutor::numberOfCommands() const
{
    return this->m_tScriptReader->commands()->size();
}

void IByteStreamScriptExecutor::setScriptFilePath(const std::string &tScriptFilePath)
{
    this->m_tScriptReader.reset();
    this->m_tScriptReader = std::make_shared<IByteStreamScriptReader>(tScriptFilePath);
}

std::vector<IByteStreamCommand> IByteStreamScriptExecutor::doUnrollLoopCommands(const std::vector<IByteStreamCommand> &IByteStreamCommands)
{
    std::vector<IByteStreamCommand> copyCommands{IByteStreamCommands};
    if (!IByteStreamScriptExecutor::containsLoopStart(copyCommands)) {
        return copyCommands;
    }
    while (IByteStreamScriptExecutor::containsLoopStart(copyCommands)) {
        std::pair<int, int> innerLoopPositions = findInnerLoopIndexes(copyCommands);
        std::vector<IByteStreamCommand> toUnroll{};
        std::vector<IByteStreamCommand> temp{};
        int numberOfLoops{std::stoi(copyCommands.at(innerLoopPositions.first).commandArgument())};
        for (int loops = 0; loops < numberOfLoops; loops++) {
            for (int i = innerLoopPositions.first+1; i < innerLoopPositions.second; i++) {
                toUnroll.emplace_back(copyCommands.at(i));
            }
        }
        for (int i = 0; i < innerLoopPositions.first; i++) {
            temp.emplace_back(copyCommands.at(i));
        }
        for (auto &it : toUnroll) {
            temp.emplace_back(it);
        }
        for (unsigned int i = innerLoopPositions.second+1; i < copyCommands.size(); i++) {
            temp.emplace_back(copyCommands.at(i));
        }
        copyCommands = temp; 
    }
    return copyCommands;
}
std::pair<int, int> IByteStreamScriptExecutor::findInnerLoopIndexes(const std::vector<IByteStreamCommand> &IByteStreamCommands)
{
    int lastLoopStartPosition{0};
    int lastLoopEndPosition{0};
    for (unsigned int i = 0; i < IByteStreamCommands.size(); i++) {
        if (IByteStreamCommands.at(i).commandType() == IByteStreamCommandType::LOOP_START) {
            lastLoopStartPosition = i;
        }
    }

    for (unsigned int i = lastLoopStartPosition; i < IByteStreamCommands.size(); i++) {
        if (IByteStreamCommands.at(i).commandType() == IByteStreamCommandType::LOOP_END) {
            if (lastLoopStartPosition == 0) {
                lastLoopEndPosition = lastLoopStartPosition + i;
            } else {
                lastLoopEndPosition = lastLoopStartPosition + i - 1;
            }
            break;
        }
    }
    return std::make_pair(lastLoopStartPosition, lastLoopEndPosition);
}

bool IByteStreamScriptExecutor::containsLoopStart(const std::vector<IByteStreamCommand> &commands) 
{
    for (auto &it : commands) {
        if (it.commandType() == IByteStreamCommandType::LOOP_START) {
            return true;
        }
    }
    return false;
}