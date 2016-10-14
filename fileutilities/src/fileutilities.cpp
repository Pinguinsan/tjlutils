/***********************************************************************
*    fileutilities.cpp:                                                *
*    Namespace FileUtilities, for file/argv IO abstraction             *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a FileUtilities namespace   *
*    It is used to abstract file/argv IO into high level functions     *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "fileutilities.h"

namespace FileUtilities
{

    std::string getCurrentDirectory()
    {
        #if defined(_WIN32) || defined (__MINGW32__)
            char* buffer;
            buffer = _getcwd(NULL, 0);
            /*Check for null buffer, indicating a failure*/
            if (!buffer) {
                throw std::runtime_error(_CWD_MALLOC_EXCEPTION_STRING);
            } else {
                return std::string(buffer);
            }
        #else
            char *currentWorkingDirectory;
            char filePathBuffer[PATH_MAX + 1];

            currentWorkingDirectory = getcwd(filePathBuffer, PATH_MAX + 1 );
            //Check if cwd is null
            if (currentWorkingDirectory) {
                return std::string(currentWorkingDirectory);
            } else {
                throw std::runtime_error(_CWD_MALLOC_EXCEPTION_STRING);
            }
        #endif
    }

    bool directoryExists(const std::string &directoryToCheck)
    { 
        struct stat info;
        if(stat(directoryToCheck.c_str(), &info) != 0) {
            return false;
        } else if(info.st_mode & S_IFDIR)  { 
            return true;
        } else {
            return false;
        }
        return directoryExists(directoryToCheck.c_str());
    }

    bool directoryExists(const char *directoryToCheck)
    {
        return directoryExists(static_cast<std::string>(directoryToCheck));
    }

    bool fileExists(const std::string &fileToCheck)
    {
        #if defined(_WIN32_) || defined(__CYGWIN__)
            std::ifstream readFromFile;
            readFromFile.open(fileToCheck);
            if (readFromFile.is_open()) {
                readFromFile.close();
                return true;
            } else {
                return false;
            }
        #else
            return (access(fileToCheck.c_str(),F_OK) != -1);
        #endif
    }

    bool fileExists(const char *fileToCheck)
    {
        return fileExists(static_cast<std::string>(fileToCheck));
    }

    std::vector<std::string> parseArgsToVector(int argcIn,char **argvIn) 
    {
        if (argcIn == 0) {
            throw std::logic_error(_PATL_ARGC_ZERO_EXCEPTION_STRING);
        }
        std::vector<std::string> returnVector;
        //No need to check, c++ standard ensures argv[0..argc-1] is not null
        for (int i = 0; i < argcIn; i++) {
            returnVector.push_back(argvIn[i]);
        }
        return returnVector;
    }

    std::list<std::string> parseArgsToList(int argcIn, char **argvIn)
    {
        if (argcIn == 0) {
            throw std::logic_error(_PATL_ARGC_ZERO_EXCEPTION_STRING);
        }
        std::list<std::string> returnList;
        //No need to check, c++ standard ensures argv[0..argc-1] is not null
        for (int i = 0; i < argcIn; i++) {
            returnList.push_back(argvIn[i]);
        }
        return returnList;
    }
}