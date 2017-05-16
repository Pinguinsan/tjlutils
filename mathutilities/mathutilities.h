/***********************************************************************
*    mathutilites.h:                                                   *
*    Namespace MathUtilities, for general math functions               *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a MathUtilities namespace     *
*    It is used to do general mathematical operations such as getting  *
*    a random value between two numbers or rounding up/down            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_MATHUTILITIES_H
#define TJLUTILS_MATHUTILITIES_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <limits>
#include <sstream>
#include <string>
#include <exception>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <random>
#include "generalutilities.h"

namespace MathUtilities
{
    const int constexpr ASCII_LOW_LIMIT{32};
    const int constexpr ASCII_HIGH_LIMIT{126};
    const long long int constexpr MERSENNE_TWISTER_DISCARD_THRESHOLD{700000};    

    bool isEvenlyDivisibleBy(int numberToCheck, int divisor);
    bool isEven(int numberToCheck);
    bool isOdd(int numberToCheck);

    bool approximatelyEquals(double lhs, double rhs, double threshold);
    bool approximatelyEquals(float lhs, float rhs, double threshold);
    bool approximatelyEquals(float lhs, double rhs, double threshold);
    bool approximatelyEquals(double lhs, float rhs, double threshold);

    int randomBetween(int lowLimit, int highLimit);
    char randomAsciiByte();
    int roundIntuitively(double numberToRound);
    int roundUp(double numberToRound, double threshold=0.00000001);
    int roundDown(double numberToRound, double threshold=0.999999999);
    int roundIntuitively(float numberToRound);
    int roundUp(float numberToRound, double threshold=0.00000001);
    int roundDown(float numberToRound, double threshold=0.999999999);
    int intExp(int base, int super);

    template <typename T>
    T intExp(T base, int super)
    {
        if (super == 0) {
            return 1;
        }
        T total{base};
        for (int i = 1; i < super; i++) {
            total = total * base;
        }
        return total;
    }

    template <typename T>
    T tAbs(T val)
    {
        return ((val >= 0) ? val : -1*val);
    }

    template <typename Lhs, typename Rhs>
    Lhs tAbs(Lhs lhs, Rhs rhs)
    {
        return (lhs-rhs >= 0 ) ? (lhs-rhs) : -1*(lhs-rhs);
    }

    template <typename Lhs, typename Rhs>
    Lhs tMax(Lhs lhs, Rhs rhs)
    {
        return (lhs >= rhs) ? lhs : rhs;
    }

    template <typename Lhs, typename Rhs>
    Lhs tMin(Lhs lhs, Rhs rhs)
    {
        return (lhs <= rhs) ? lhs : rhs;
    }
    std::mt19937 randomlySeededMersenneTwister();

    class Random 
    {
    public:
        Random() = default;
        Random(std::mt19937::result_type seed);
        int drawNumber(int min, int max);

    private:        
        std::mt19937 m_randomEngine{std::random_device{}()};
    };
}

#endif //TJLUTILS_MATHUTILITIES_H
