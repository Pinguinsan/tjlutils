/***********************************************************************
*    tjlutils.h:                                                       *
*    Meta-library header, pulling in all header files from tjlutils    *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file is a header that pulls in all the relevant headers for  *
*    the tjlutils library, so each one doesn't need to be included     *
*    It exists entirely as a convenience header                        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_COMMON_H
#define TJLUTILS_COMMON_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include "systemcommand.h"
#include "datetime.h"
#include "generalutilities.h"
#include "fileutilities.h"
#include "systemcommand.h"
#include "pythoncrypto.h"
#include "templateobjects.h"
#include "crypto.h"

#endif //TJLUTILS_COMMON_H
