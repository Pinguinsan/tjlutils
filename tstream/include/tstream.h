/***********************************************************************
*    tstream.h:                                                        *
*    TStream, abstract base class for simple read and write operations *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an abstract base class TSream *
*    It includes very basic IO operations like read and write          *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TSTREAM_H
#define TJLUTILS_TSTREAM_H

#include <iostream>
#include <string>

enum class LineEnding {
    LE_None,
    LE_CarriageReturn,
    LE_LineFeed,
    LE_CarriageReturnLineFeed
};


enum class TStreamCommandType { DELAY_SECONDS, DELAY_MILLISECONDS, DELAY_MICROSECONDS, WRITE, READ, FLUSH_RX, FLUSH_TX, FLUSH_RX_TX, LOOP_START, LOOP_END, COMMAND_UNSPECIFIED };
enum class DelayType { SECONDS, MILLISECONDS, MICROSECONDS };
enum class FlushType { RX, TX, RX_TX };
enum class LoopType { START, END };

class TStreamCommand
{
public:
    TStreamCommand(TStreamCommandType commandType, const std::string &commandArgument) :
        m_commandType{commandType},
        m_commandArgument{commandArgument} { }
    
    TStreamCommandType commandType() const { return this->m_commandType; }
    std::string commandArgument() const { return this->m_commandArgument; }
    void setCommandType(const TStreamCommandType &commandType) { this->m_commandType = commandType; }
    void setCommandArgument(const std::string &commandArgument) { this->m_commandArgument = commandArgument; }

private:
    TStreamCommandType m_commandType;
    std::string m_commandArgument;
};


class TStream
{
public:
    virtual ~TStream() { }

    virtual void setTimeout(unsigned int timeout) = 0;
    virtual unsigned int timeout() const = 0;
    LineEnding lineEnding() const;
    void setLineEnding(LineEnding lineEnding);

    virtual ssize_t writeString(const std::string &str) = 0;
    virtual ssize_t writeString(const char *str) = 0;
    virtual ssize_t writeByte(char toSend) = 0;
    virtual bool isOpen() const = 0;
    virtual void openPort() = 0;
    virtual void closePort() = 0;

    virtual std::string portName() const = 0;
    virtual void flushRX() = 0;
    virtual void flushTX() = 0;
    virtual void flushRXTX() = 0;

    virtual std::string peek() = 0;
    virtual char peekByte() = 0;

    virtual void putBack(const std::string &str) = 0;
    virtual void putBack(const char *str) = 0;
    virtual void putBack(char back) = 0;

    virtual std::string readString(int maximumReadSize = NO_MAXIMUM_READ_SIZE) = 0;
    virtual std::string readStringUntil(const std::string &until, int maximumReadSize = NO_MAXIMUM_READ_SIZE) = 0;
    virtual std::string readStringUntil(const char *until, int maximumReadSize = NO_MAXIMUM_READ_SIZE) = 0;
    virtual std::string readStringUntil(char until) = 0;

    static const constexpr int NO_MAXIMUM_READ_SIZE{-1};
};

#endif