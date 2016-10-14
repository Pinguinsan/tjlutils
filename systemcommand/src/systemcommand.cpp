/***********************************************************************
*    systemcommand.cpp:                                                *
*    User defined class, SystemCommand, for a bash-like interface      *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SystemCommand class       *
*    The class is made up entirely of static methods and constants     *
*    It is used to enable a bash-like iterface, allowing one to        *
*    retrieve the stdout/stderr from a call to the command interpreter *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "systemcommand.h"

const int SystemCommand::_DEFAULT_MAX_OUTPUT = 2147483647;

SystemCommand::SystemCommand() : 
    _command{""},
    _maxOutput{_DEFAULT_MAX_OUTPUT},
    _hasError{false},
    _repeatCommand{false}
{

}

SystemCommand::SystemCommand(const std::string &command) : 
    _command{command},
    _maxOutput{_DEFAULT_MAX_OUTPUT},
    _hasError{false},
    _repeatCommand{false}
{

}

SystemCommand::SystemCommand(const std::string &command, int maxOutput) :
    _command{command},
    _maxOutput{maxOutput},
    _hasError{false},
    _repeatCommand{false}
{

}

SystemCommand &SystemCommand::operator+=(const std::string &rhs)
{
    this->_command += rhs;
    return *this;
}

void SystemCommand::printCommand()
{
    std::cout << stripPipeFromCommand(this->_command) << std::endl;
}

void SystemCommand::insertIntoCommand(int position, const std::string &stringToInsert)
{
    if (static_cast<unsigned int>(position) > this->_command.length()) {
        return;
    }
    this->_command.insert(position, stringToInsert);
	this->_repeatCommand = false;
}

void SystemCommand::insertIntoCommand(int position, char charToInsert)
{
    std::string temp = "";
    temp += charToInsert;
    this->insertIntoCommand(position, temp);
}

int SystemCommand::maxOutput()
{
    return this->_maxOutput;
}

std::string SystemCommand::command()
{
    return stripPipeFromCommand(this->_command);
}

bool SystemCommand::hasError()
{
    return this->_hasError;
}

void SystemCommand::appendToCommand(const std::string &stringToAppend)
{
    _command += stringToAppend;
	this->_repeatCommand = false;
}

int SystemCommand::returnValue()
{
    return this->_returnValue;
}

void SystemCommand::setCommand(const std::string &command)
{
    this->_repeatCommand = false;
    this->_command = command;
    this->_hasError = false;
    this->_sizeOfOutput = 0;
    this->_outputAsVector.clear();
}

void SystemCommand::setMaxOutput(int maxOutput)
{
    this->_maxOutput = maxOutput;
}

std::string SystemCommand::outputAsString()
{
    std::string returnString = "";
    for (std::vector<std::string>::const_iterator iter = _outputAsVector.begin(); iter != _outputAsVector.end(); iter++) {
        returnString += (*iter);
    }
    return returnString;
}

void SystemCommand::stripShellControlCharactersFromCommand()
{
    //TODO: Implement
    return;
}

std::vector<std::string> SystemCommand::outputAsVector()
{
    return this->_outputAsVector;
}

void SystemCommand::execute()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
}

std::vector<std::string> SystemCommand::executeAndWaitForOutputAsVector()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
    return this->_outputAsVector;
}

void SystemCommand::executeWithoutPipe()
{
    this->systemCommandLaunch(PipeStatus::WITHOUT_PIPE);
}

std::string SystemCommand::stripPipeFromCommand(const std::string &command)
{
    using namespace GeneralUtilities;
    std::string returnString{command};
    #if defined(_WIN32) && defined(_MSC_VER)
        returnString = stripAllFromString(_command, "2>&1");
        returnString = stripAllFromString(_command, ">");
    #else
        returnString = stripAllFromString(_command, "2>&1");
        returnString = stripAllFromString(_command, ">");
    #endif
    return returnString;
}


std::string SystemCommand::executeAndWaitForOutputAsString()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
    return this->outputAsString();
}

int SystemCommand::sizeOfOutput()
{
    return this->_sizeOfOutput;
}

void SystemCommand::verifyValidMaxOutput()
{
    if ((_maxOutput <= 8) || (_maxOutput > (std::numeric_limits<int>::max)())) {
        _maxOutput = _DEFAULT_MAX_OUTPUT;
    }
}

std::unique_ptr<wchar_t> SystemCommand::getWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    std::unique_ptr<wchar_t> wc{new wchar_t[cSize]};
    mbstowcs (wc.get(), c, cSize);
    return std::move(wc);
}

FILE *SystemCommand::popenHandler(const char *args, const char *method)
{
    #ifdef _WIN32
        return _popen(args, method);
    #else
        return popen(args, method);
    #endif
}

int SystemCommand::pcloseHandler(FILE *fp)
{
    #ifdef _WIN32
        return _pclose(fp);
    #else
        return pclose(fp);
    #endif
}

void SystemCommand::systemCommandLaunch(PipeStatus pipe)
{
    using namespace GeneralUtilities;
    this->_command = stripPipeFromCommand(this->_command);
    if (this->_repeatCommand) {
    	this->_hasError = false;
    	this->_sizeOfOutput = 0;
    	this->_outputAsVector.clear();
    } else {
        this->_repeatCommand = true;
    }
	verifyValidMaxOutput();
    /*
    #ifdef _WIN32
        HANDLE g_hChildStd_IN_Rd = NULL;
        HANDLE g_hChildStd_OUT_Rd = NULL;
        HANDLE g_hChildStd_OUT_Wr = NULL;

        #define BUFSIZE 4096

        if (pipe == PipeStatus::WITH_PIPE) {
            this->_command += " 2>&1"; //merge stderr with stdout
        }
        std::unique_ptr<wchar_t> szCmdline(getWC(this->_command.c_str()));
        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
        BOOL bSuccess = FALSE;

        // Set up members of the PROCESS_INFORMATION structure.

        ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

        // Set up members of the STARTUPINFO structure.
        // This structure specifies the STDIN and STDOUT handles for redirection.

        std::unique_ptr<wchar_t> windowTitle{getWC("SystemCommandWindow")};
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = g_hChildStd_OUT_Wr;
        siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
        siStartInfo.hStdInput = g_hChildStd_IN_Rd;
        siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        siStartInfo.wShowWindow = SW_HIDE;
        siStartInfo.lpTitle = windowTitle.get();

        bSuccess = CreateProcess(NULL,
                   szCmdline.get(),// command line
                   NULL,          // process security attributes
                   NULL,          // primary thread security attributes
                   TRUE,          // handles are inherited
                   CREATE_NO_WINDOW, // creation flags
                   NULL,          // use parent's environment
                   NULL,          // use parent's current directory
                   &siStartInfo,  // STARTUPINFO pointer
                   &piProcInfo);  // receives PROCESS_INFORMATION

        HWND consoleName = FindWindow(NULL, windowTitle.get());
        if(consoleName){
            ShowWindow(consoleName,SW_HIDE);
        }
        // If an error occurs, exit the application.
        if (!bSuccess) {
            std::cout << "ERROR: Failed to execute command " << std::quoted(this->_command) << std::endl;
            this->_returnValue = -1;
            return;
        } else {
            // Close handles to the child process and its primary thread.
            // Some applications might keep these handles to monitor the status
            // of the child process, for example.
            CloseHandle(piProcInfo.hProcess);
            CloseHandle(piProcInfo.hThread);
        }

        // Read output from the child process's pipe for STDOUT
        // and write to the parent process's pipe for STDOUT.
        // Stop when there is no more data.
        DWORD dwRead;
        CHAR path[BUFSIZE];

        if (pipe == PipeStatus::WITH_PIPE) {
            for (;;) {
                bSuccess = ReadFile(g_hChildStd_OUT_Rd, path, BUFSIZE, &dwRead, NULL);
                //bSuccess = ReadFile(GetStdHandle(STD_INPUT_HANDLE), path, BUFSIZE, &dwRead, NULL);
                if ((!bSuccess) || (dwRead == 0) || (this->_sizeOfOutput >= this->_maxOutput)){
                    break;
                } else {
                    std::string formattedPath{static_cast<std::string>(path)};
                    addFormattedThing(this->_outputAsVector, formattedPath, [](const std::string &stringToStrip) -> std::string
                    {
                        std::string returnString{ stringToStrip };
                        std::vector<std::string> newLines{ "\r\n", "\n\r", "\n" };
                        for (std::vector<std::string>::const_iterator iter = newLines.begin(); iter != newLines.end(); iter++) {
                            if (returnString.find(*iter) != std::string::npos) {
                                size_t foundPosition = returnString.find(*iter);
                                returnString = returnString.substr(0, foundPosition);
                            }
                        }
                        return returnString;
                    });
                    this->_sizeOfOutput += sizeof(*(std::end(_outputAsVector)-1));
                    //bSuccess = WriteFile(hParentStdOut, chBuf,
                    //    dwRead, &dwWritten, NULL);
                    //if (!bSuccess) break;
                }
            }
        }

    #else
    */
    if (pipe == PipeStatus::WITH_PIPE) {
        this->_command += " 2>&1"; //Merges stderror with stdout
        FILE *fp;
        char path[PATH_MAX];

        fp = popenHandler(this->_command.c_str(), "r");
        if (fp == NULL) {
            std::cout << "ERROR: Failed to execute command " << GeneralUtilities::tQuoted(this->_command) << std::endl;
            this->_returnValue = -1;
            return;
        }
        int outputSize{ 0 };
        while ((fgets(path, PATH_MAX, fp) != NULL) && (outputSize <= this->_maxOutput)) {
            std::string formattedPath{static_cast<std::string>(path)};
            addFormattedThing(this->_outputAsVector, formattedPath, [](const std::string &stringToStrip) -> std::string
            {
                std::string returnString{ stringToStrip };
                std::vector<std::string> newLines{ "\r\n", "\n\r", "\n" };
                for (std::vector<std::string>::const_iterator iter = newLines.begin(); iter != newLines.end(); iter++) {
                    if (returnString.find(*iter) != std::string::npos) {
                        size_t foundPosition = returnString.find(*iter);
                        returnString = returnString.substr(0, foundPosition);
                    }
                }
                return returnString;
            });
            outputSize += sizeof(*(std::end(this->_outputAsVector)-1));
        }
        this->_returnValue = pcloseHandler(fp);
    } else {
        this->_command = stripPipeFromCommand(this->_command);
        #ifdef _WIN32
            this->_returnValue = system(this->_command.c_str());
        #else
            this->_returnValue = system(this->_command.c_str())/256;
        #endif
    }
    this->_hasError = (this->_returnValue != 0);
}
