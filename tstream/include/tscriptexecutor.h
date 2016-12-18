/***********************************************************************
*    tscriptexecutor.h:                                                *
*    TScriptReader, script executor for TStream objects                *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TScripExecutor class that   *
*    executes TStreamCommand objects directly on a TStream object      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_SCRIPTEXECUTOR_H
#define TJLUTILS_SCRIPTEXECUTOR_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <functional>
#include <cstdlib>
#include <utility>

#include <generalutilities.h>
#include <tstream.h>
#include <tstreamstrings.h>
#include <tscriptreader.h>

class TScriptExecutor
{
public:
    TScriptExecutor(const std::string &scriptFilePath);
    void setScriptFilePath(const std::string &scriptFilePath);
    std::string scriptFilePath() const;
    bool hasCommands() const;
    size_t numberOfCommands() const;
    void execute(std::shared_ptr<TStream> ioStream, 
                 const std::function<void(const std::string &)> &printRxResult, 
                 const std::function<void(const std::string &)> &printTxResult,
                 const std::function<void(DelayType, int)> &printDelayResult,
                 const std::function<void(FlushType)> &printFlushResult,
                 const std::function<void(LoopType, int, int)> &printLoopResult);
private:
    std::shared_ptr<TScriptReader> m_tScriptReader;
    std::vector<TStreamCommand> m_tScriptCommands;

    std::vector<TStreamCommand> doUnrollLoopCommands(const std::vector<TStreamCommand> &tStreamCommands);
    bool containsLoopStart(const std::vector<TStreamCommand> &commands);
    std::pair<int, int> findInnerLoopIndexes(const std::vector<TStreamCommand> &tStreamCommands);


};


#endif //TJLUTILS_SCRIPTEXECUTOR_H
