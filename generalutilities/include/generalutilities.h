/***********************************************************************
*    generalutilities.h:                                               *
*    Namespace GeneralUtilities, for general use functions             *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a GeneralUtilities namespace  *
*    The namespace contains several general use functions, including   *
*    STL algorithm fuinctions, delay functions, char to int functions, *
*    logging functions, and mathematical rounding functions            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with libraryprojects                         *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/


#ifndef TJLUTILS_GENERALUTILITIES_H
#define TJLUTILS_GENERALUTILITIES_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <chrono>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <exception>
#include <iterator>
#include <functional>
#include <typeinfo>
#include <list>

namespace GeneralUtilities
{
    const size_t generalnpos{std::string::npos};
    const int constexpr ASCII_WHITESPACE_MAXIMUM_VALUE{32};
    const unsigned int constexpr ASCII_HIGH_LIMIT{128};
    const unsigned int constexpr ASCII_LOW_LIMIT{0};
    const long long int constexpr NANOSECONDS_PER_MICROSECOND{1000};
    const long long int constexpr NANOSECONDS_PER_MILLISECOND{1000000};
    const long long int constexpr NANOSECONDS_PER_SECOND{1000000000};
    const long long int constexpr NANOSECONDS_PER_MINUTE{60000000000};
    const long long int constexpr NANOSECONDS_PER_HOUR{3600000000000};
    const long long int constexpr NANOSECONDS_PER_DAY {86400000000000};
    const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    const long long int constexpr MICROSECONDS_PER_SECOND{1000000};
    const long long int constexpr MICROSECONDS_PER_MINUTE{60000000};
    const long long int constexpr MICROSECONDS_PER_HOUR{3600000000};
    const long long int constexpr MICROSECONDS_PER_DAY{86400000000};
    const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    const long long int constexpr MILLISECONDS_PER_HOUR{3600000};
    const long long int constexpr MILLISECONDS_PER_DAY{86400000};
    const long long int constexpr SECONDS_PER_MINUTE{60};
    const long long int constexpr SECONDS_PER_HOUR{3600};
    const long long int constexpr MINUTES_PER_HOUR{60};
    const long long int constexpr MINUTES_PER_DAY{1440};
    const long long int constexpr HOURS_PER_DAY{24};
    const long long int constexpr MERSENNE_TWISTER_DISCARD_THRESHOLD{700000};

    template <typename Function, typename...FunctionArgs>
    void delaySecondsWithCallback(unsigned int howLong, const Function &function, FunctionArgs... functionArgs)
    {
        auto microsecondsToDelay{howLong * MICROSECONDS_PER_SECOND};
        auto startTime{std::chrono::high_resolution_clock::now()};
        auto endTime{std::chrono::high_resolution_clock::now()};
        auto elapsedTime{std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()};
        do {
            (void)function(functionArgs...);
            endTime = std::chrono::high_resolution_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        } while (elapsedTime <= microsecondsToDelay);
    }

    template <typename Function, typename...FunctionArgs>
    void delayMillisecondsWithCallback(unsigned int howLong, const Function &function, FunctionArgs... functionArgs)
    {
        auto microsecondsToDelay{howLong * MICROSECONDS_PER_MILLISECOND};
        auto startTime{std::chrono::high_resolution_clock::now()};
        auto endTime{std::chrono::high_resolution_clock::now()};
        auto elapsedTime{std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()};
        do {
            (void)function(functionArgs...);
            endTime = std::chrono::high_resolution_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        } while (elapsedTime <= microsecondsToDelay);
    }

    template <typename Function, typename...FunctionArgs>
    void delayMicrosecondsWithCallback(unsigned int howLong, const Function &function, FunctionArgs... functionArgs)
    {
        auto microsecondsToDelay{howLong * MICROSECONDS_PER_MILLISECOND};
        auto startTime{std::chrono::high_resolution_clock::now()};
        auto endTime{std::chrono::high_resolution_clock::now()};
        auto elapsedTime{std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()};
        do {
            (void)function(functionArgs...);
            endTime = std::chrono::high_resolution_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        } while (elapsedTime <= microsecondsToDelay);
    }

    
    
    bool isNonAsciiChar (char c);
    std::string stripNonAsciiCharacters(const std::string &str);    
    std::string stripLineEndings(const std::string &str);

    std::string toLowercase(const std::string &str);
    std::string toUppercase(const std::string &str);
    std::string trimWhitespaceFromBeginning(const std::string &str);
    std::string trimWhitespaceFromEnd(const std::string &str);
    std::string trimWhitespace(const std::string &str);

    std::vector<std::string> trimWhitespaceFromBeginning(const std::vector<std::string> &vec);
    std::vector<std::string> trimWhitespaceFromEnd(const std::vector<std::string> &vec);
    std::vector<std::string> trimWhitespace(const std::vector<std::string> &vec);

    bool isWhitespace(const std::string &stringToCheck);
    bool isWhitespace(char charToCheck);
    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip);
    std::string stripFromString(const std::string &stringToStrip, char whatToStrip);
    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip);
    std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip);
    std::string tWhitespace(unsigned int howMuch);
    int charToInt(char charToConvert);
    char intToChar (int intToConvert);
    void delaySeconds(double howLong);
    void delayMilliseconds(double howLong);
    void delayMicroseconds(double howLong);
    void delayNanoseconds(double howLong);
    std::pair<std::string, std::string> splitFileName(const std::string &fullPath);
    bool isDigit(char charToCheck);
    int intExp(int base, int super);
    bool endsWith(const std::string &stringToCheck, const std::string &matchString);
    bool endsWith(const std::string &stringToCheck, char matchChar);
    bool endsWithNotIncludingWhitespace(const std::string &stringToCheck, const std::string &matchString);
    bool endsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar);
    bool startsWith(const std::string &stringToCheck, const std::string &matchString);
    bool startsWith(const std::string &stringToCheck, char matchChar);
    bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, const std::string &matchString);
    bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar);
    void logString(const std::string &stringToLog);
    void logString(const std::string &stringToLog, std::string fileName);
    std::string toString(const std::string &convert);
    std::string toString(const char *convert);
    std::string tQuoted(const std::string &convert);
    std::string tQuoted(const char *convert);
    unsigned int hexStringToUInt(const std::string &str);
    unsigned char hexStringToUChar(const std::string &str);

    std::string toFixedWidth(const std::string &inputString, unsigned int fixedWidth);
    std::string toFixedWidth(const char *inputString, unsigned int fixedWidth);
    
    std::string toBinaryString(int number);
    std::string toDecString(int number);
    std::string toHexString(int number);

    std::string toBinaryString(uint32_t number);
    std::string toDecString(uint32_t number);
    std::string toHexString(uint32_t number);

    std::string toBinaryString(uint8_t number);
    std::string toDecString(uint8_t number);
    std::string toHexString(uint8_t number);
    
    std::string toBinaryString(bool number);
    std::string toDecString(bool number);
    std::string toHexString(bool number);

    std::string stripTrailingWhitespace(const std::string &stringToStrip);
    std::string tEndl();
    std::string tParenthesis(const std::string &convert);
    std::string tParenthesis(const char *convert);

    template<typename Function>
    std::string stripFromStringIf(const std::string &stringToStrip, Function function)
    {
        bool substitutionMade{false};
        std::string returnString{""};
        for (auto iter = stringToStrip.begin(); iter != stringToStrip.end(); iter++) {
            if (function(*iter) && !substitutionMade) {
                substitutionMade = true;
                continue;
            }
            returnString += *iter;
        }
        return returnString;
    }

    template <typename Function>
    std::string stripAllFromStringIf(const std::string &stringToStrip, Function function)
    {
        std::string returnString{""};
        for (auto iter = stringToStrip.begin(); iter != stringToStrip.end(); iter++) {
            if (!function(*iter)) {
                returnString += *iter;
            }
        }
         return returnString;
    }

    /*Log a numerical value. On file opening failure, the value is output to stdout*/
    template <typename T>
    void logDataMember(const T &valueToLog, const std::string &nameOfVariable) { std::cout << nameOfVariable << " = " << valueToLog << std::endl; }
    template<typename T>
    void logDataMember(const T &valueToLog, const std::string &nameOfVariable, const std::string &fileName)
    {
        std::ofstream writeToFile;
        writeToFile.open(fileName, std::ios::app);
        if (writeToFile.is_open()) {
            writeToFile << nameOfVariable << " = " << valueToLog << std::endl;
            writeToFile.close();
        }
        else {
            std::cout << "Error opening file " << tQuoted(fileName) << " for writing, writing to stdout instead:" << std::endl;
            std::cout << nameOfVariable << " = " << valueToLog << std::endl;
        }
    }
    
    /*Converting objects to strings*/
    template<typename T>
    std::string toString(const T &convert) 
    {
         std::string returnString{""};
         std::stringstream transfer;
         transfer << convert;
         transfer >> returnString;
         return returnString;
    }

    template<typename T>
    bool vectorContains(const std::vector<T> &vectorToCheck, const T &compareObject)
    {
        for (typename std::vector<T>::const_iterator iter = vectorToCheck.begin(); iter != vectorToCheck.end(); iter++) {
            if ((*iter) == (compareObject)) {
                return true;
            }
        }
        return false;
    }

    template <typename TKey, typename TValue>
    bool unorderedMapContainsKey(const std::unordered_map<TKey, TValue> &tMap, const TKey &tKey)
    {
        return (tMap.find(tKey) != tMap.end());
    }

    template <typename TKey, typename TValue>
    bool mapContainsKey(const std::map<TKey, TValue> &tMap, const TKey &tKey) 
    {
        return (tMap.find(tKey) != tMap.end());
    }

    template<typename T>
    struct isCallableImplicit 
    {
    private:
        typedef char(&yes)[1];
        typedef char(&no)[2];

        struct Fallback { void operator()(); };
        struct Derived : T, Fallback { };

        template<typename U, U> struct Check;

        template<typename>
        yes test(...);

        template<typename C>
        no test(Check<void (Fallback::*)(), &C::operator()>*);

        public:
        const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
    };

    template<typename T>
    struct isCallable : std::conditional < std::is_class<T>::value, isCallableImplicit<T>, std::false_type >::type { };

    template <typename BeginningIterator, typename EndingIterator, typename T>
    bool itemExists(const BeginningIterator &bit, const EndingIterator &eit, const T &searchTerm)
    {
        static_assert(std::is_same<typename std::iterator_traits<BeginningIterator>::value_type,  typename std::decay<T>::type>::value, "BeginningIter must be of same type as search term");
        static_assert(std::is_same<typename std::iterator_traits<EndingIterator>::value_type,  typename std::decay<T>::type>::value, "EndingIter must be of same type as search term");
        static_assert(std::is_same<typename std::decay<decltype(*bit)>::type, typename std::decay<T>::type>::value, "Search term must be same type as container contents");
        for (auto iter = bit; iter != eit; iter++) {
            if (*iter == searchTerm) {
                return true;
            }
        }
        return false;
    }

    template <typename Container, typename T>
    bool itemExists(const Container &container, const T &searchTerm)
    {
        static_assert(std::is_same<typename std::decay<decltype(std::begin(container))>::type, typename std::decay<T>::type>::value, "Search term must be same type as container contents");
        for (auto iter = std::begin(container); iter != std::end(container); iter++) {
            if (*iter == searchTerm) {
                return true;
            }
        }
    }

    template <typename T>
    bool itemExistsInList(const std::list<T> &container, const T &searchTerm)
    {
        for (auto &it : container) {
            if (searchTerm == it) {
                return true;
            }
        }
        return false;
    }

    template<class T>
    class isIterator
    {   
    public:
        T makeT();
        typedef void * twoptrs[2];  // sizeof(twoptrs) > sizeof(void *)
        twoptrs & test(...); // Common case
        template<class R> typename R::iterator_category * test(R); // Iterator
        template<class R> void * test(R *); // Pointer
        const bool value = sizeof(test(makeT())) == sizeof(void *); 
    };
    
    template <typename BeginningIterator, typename EndingIterator, typename Function>
    size_t findLastOfIf(const BeginningIterator &bit, const EndingIterator &eit, Function function)
    {
        size_t foundPosition{GeneralUtilities::generalnpos};
        for (auto iter = bit; iter != eit; iter++) {
            if (function(*iter)) {
                foundPosition = std::distance(bit, iter);
            }
        }
        return foundPosition;
    }

    template <typename BeginningIterator, typename EndingIterator, typename Function>
    size_t findIf(const BeginningIterator &bit, const EndingIterator &eit, Function function)
    {
        size_t foundPosition{GeneralUtilities::generalnpos};
        for (auto iter = bit; iter != eit; iter++) {
            if (function(*iter)) {
                return foundPosition;
            }
        }
        return foundPosition;
    }

    template <typename T, typename dT>
    std::vector<T> parseToVector(const T &thingToParse, dT delimiter)
    {
        static_assert(std::is_same<typename std::decay<decltype(*(std::begin(thingToParse)))>::type,  typename std::decay<dT>::type>::value, "The object to parse must dereference to the same type as the delimiters type");
        std::vector<T> returnVector;
        T copyThing{thingToParse};
        if (copyThing.length() > 0) {
            if (copyThing[0] == delimiter) {
                copyThing = copyThing.substr(1);
            }
        }
        if (copyThing.find(delimiter) == std::string::npos) {
            returnVector.emplace_back(copyThing);
            return returnVector;
        }
        while (copyThing.length() > 0) {
            size_t foundPosition{copyThing.find(delimiter)};
            if (foundPosition != std::string::npos) {
                returnVector.emplace_back(copyThing.substr(0, foundPosition));
                copyThing = copyThing.substr(foundPosition+1);
            } else {
                returnVector.emplace_back(copyThing);
                copyThing = "";
            }
        }           
        return returnVector;
    }

    template <typename T, typename dT, typename Container = std::vector<T>>
    Container parseToContainerAdapter(const T &thingToParse, dT delimiter) 
    {
        static_assert(std::is_same<typename std::decay<decltype(*(std::begin(thingToParse)))>::type,  typename std::decay<dT>::type>::value, "The object to parse must dereference to the same type as the delimiters type");
        Container returnContainer;
        T copyThing{thingToParse};
        if (copyThing.length() > 0) {
            if (copyThing[0] == delimiter) {
                copyThing = copyThing.substr(1);
            }
        }
        if (copyThing.find(delimiter) == std::string::npos) {
            returnContainer.emplace(copyThing);
            return returnContainer;
        }
        while (copyThing.length() > 0) {
            size_t foundPosition{copyThing.find(delimiter)};
            if (foundPosition != std::string::npos) {
                returnContainer.emplace(copyThing.substr(0, foundPosition));
                copyThing = copyThing.substr(foundPosition+1);
            } else {
                returnContainer.emplace(copyThing);
                copyThing = "";
            }
        }
        return returnContainer;           
    }


    template <typename T, typename dT, typename Container = std::vector<T>>
    Container parseToContainer(const T &thingToParse, dT delimiter) 
    {
        static_assert(std::is_same<typename std::decay<decltype(*(std::begin(thingToParse)))>::type,  typename std::decay<dT>::type>::value, "The object to parse must dereference to the same type as the delimiters type");
        Container returnContainer;
        T copyThing{thingToParse};
        if (copyThing.length() > 0) {
            if (copyThing[0] == delimiter) {
                copyThing = copyThing.substr(1);
            }
        }
        if (copyThing.find(delimiter) == std::string::npos) {
            returnContainer.emplace_back(copyThing);
            return returnContainer;
        }
        while (copyThing.length() > 0) {
            size_t foundPosition{copyThing.find(delimiter)};
            if (foundPosition != std::string::npos) {
                returnContainer.emplace_back(copyThing.substr(0, foundPosition));
                copyThing = copyThing.substr(foundPosition+1);
            } else {
                returnContainer.emplace_back(copyThing);
                copyThing = "";
            }
        }
        return returnContainer;
    }

    template<typename T>
    std::string tQuoted(const T &convert)
    {
        return ("\"" + toString(convert) + "\"");
    }

    template<typename T>
    std::string tParenthesis(const T &convert)
    {
        return ("(" + toString(convert) + ")");
    }
    
    template<typename Container, typename T, typename Function>
    void addFormattedThing(Container &container, const T& thingToAdd, Function function) 
    {
        static_assert(std::is_same<typename std::decay<decltype(*(std::begin(container)))>::type,  typename std::decay<T>::type>::value, "The object to add must dereference to the same type as the container contents");
        container.push_back(function(thingToAdd));
    }


}

#endif // TJLUTILS_GENERALUTILITIES_H
