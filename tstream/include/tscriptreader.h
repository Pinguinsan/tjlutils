/***********************************************************************
*    tscriptreader.h:                                                  *
*    TScriptReader, script reader for TStream objects                  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TScripReader class that     *
*    reads and parses script files intended for use alongside the      *
*    TScriptExecutor to script commands for TStream objects            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TSCRIPTREADER_H
#define TJLUTILS_TSCRIPTREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>

#include <fileutilities.h>
#include <generalutilities.h>
#include <mathutilities.h>
#include <tstream.h>
#include <tstreamstrings.h>



class TScriptReader
{
public:
    TScriptReader(const std::string &scriptFilePath);
    std::shared_ptr<std::vector<TStreamCommand>> commands() const;
private:
    std::string m_tScriptFilePath;
    std::shared_ptr<std::vector<TStreamCommand>> m_commands;

};

#endif //TJLUTILS_TSCRIPTREADER_H
