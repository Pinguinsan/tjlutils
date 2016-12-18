/***********************************************************************
*    tstreamstrings.h:                                                 *
*    TScriptReader, strings used by TStreamReader and TStreamExecutor  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a strings used by the         *
*    TScriptReader and TStreamExecutor classes for TStream objects     *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TSTREAMSTRINGS_H
#define TJLUTILS_TSTREAMSTRINGS_H

namespace TStreamStrings
{
    extern const char *DELAY_IDENTIFIER;
    extern const char *DELAY_SECONDS_IDENTIFIER;
    extern const char *DELAY_MILLISECONDS_IDENTIFIER;
    extern const char *DELAY_MICROSECONDS_IDENTIFIER;
    extern const char *WRITE_IDENTIFIER;
    extern const char *READ_IDENTIFIER;
    extern const char *LOOP_IDENTIFIER;
    extern const char *FLUSH_IDENTIFIER;

    extern const char *NO_CLOSING_PARENTHESIS_FOUND_STRING;
    extern const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING;
    extern const char *NO_PARAMETER_SEPARATING_COMMA_STRING;
    extern const char *NO_LOOP_COUNT_SPECIFIED_STRING;
    extern const char *EXPECTED_HERE_STRING;
    extern const char *HERE_STRING;
    extern const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING;
    extern const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *GENERIC_CONFIG_WARNING_BASE_STRING;
    extern const char *GENERIC_CONFIG_WARNING_TAIL_STRING;
    extern const char *CONFIG_EXPRESSION_MALFORMED_STRING;
    extern const char *EXCEPTION_IN_CONSTRUCTOR_STRING;
    extern const char *COMMAND_TYPE_NOT_IMPLEMENTED_STRING;
    extern const char *NULL_IO_STREAM_PASSED_TO_EXECUTE_STRING;
    extern const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING;
    extern const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING;
    extern const char *LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *UNTERMINATED_LOOP_STRING;
    extern const char *UNEXPECTED_LOOP_CLOSING_STRING;
    extern const char *CLOSING_LOOP_IDENTIFIER;
}

#endif //TJLUTILS_TSTREAMSTRINGS_H