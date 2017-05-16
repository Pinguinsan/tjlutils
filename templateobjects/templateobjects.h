/***********************************************************************
*    templateobjects.h:                                                *
*    User defined classes, inheriting from TemplateObject<T>, to use   *
*    primitive types as objects, Java style (int becomes Integer)      *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations and implementations for general  *
*    object classes that mimick primitive data types, including        *
*    "parse" and "toString" methods that allow the primitive data types*
*    to be easily cast to and from std::strings, or any other string   *
*    by overriding the aforementioned methods for whatever purpose     *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TEMPLATEOBJECTS_H
#define TJLUTILS_TEMPLATEOBJECTS_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <string>
#include <sstream>
#include <cassert>
#include <initializer_list>

template<typename T>
class Object
{
public:
    //virtual static T parse(std::string parseString) = 0;
    virtual std::string toString() const = 0;
    virtual T value() const = 0;
    virtual int getHashCode() const = 0;
};

template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
class TemplateObject : public Object<T>
{
public:
    explicit TemplateObject(const T &value) : m_value{value} {}
    explicit TemplateObject(T value) : m_value{value} {}
    explicit TemplateObject(std::initializer_list<T> ini) {
        assert(ini.size() == 1);
        m_value = *(ini.begin());
    }
    //TemplateObject<T>& operator=(const TemplateObject<T>& other) { return *this; } //Copy constructor
    //TemplateObject<T>& operator=(TemplateObject<T>&& other) { return *this; } //Move constructor
    TemplateObject<T> &operator=(T value) {
        this->m_value = value;
        return &this;
    }

    TemplateObject<T>& operator++() {
        this->m_value = (this->m_value+1);
        return *this;
    }

    TemplateObject<T>& operator--() {
        this->m_value = (this->m_value-1);
        return *this;
    }

    TemplateObject<T> operator++(int) {
        TemplateObject previous{*this};
        operator++();
        return previous;
    }

    TemplateObject<T> operator--(int) {
        TemplateObject previous{*this};
        operator--();
        return previous;
    }

    /*binary math operators*/
    friend inline TemplateObject<T> operator+(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs += rhs.value()); }
    friend inline TemplateObject<T> operator-(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs -= rhs.value()); }
    friend inline TemplateObject<T> operator*(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs *= rhs.value()); }
    friend inline TemplateObject<T> operator/(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs /= rhs.value()); }
    friend inline TemplateObject<T> operator%(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs %= rhs.value()); }
    friend inline TemplateObject<T> operator^(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs ^= rhs.value()); }
    friend inline TemplateObject<T> operator&(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs &= rhs.value()); }
    friend inline TemplateObject<T> operator|(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs |= rhs.value()); }

    friend inline TemplateObject<T> operator+(TemplateObject<T> lhs, const T &rhs) { return (lhs += rhs); }
    friend inline TemplateObject<T> operator-(TemplateObject<T> lhs, const T &rhs) { return (lhs -= rhs); }
    friend inline TemplateObject<T> operator*(TemplateObject<T> lhs, const T &rhs) { return (lhs *= rhs); }
    friend inline TemplateObject<T> operator/(TemplateObject<T> lhs, const T &rhs) { return (lhs /= rhs); }
    friend inline TemplateObject<T> operator%(TemplateObject<T> lhs, const T &rhs) { return (lhs %= rhs); }
    friend inline TemplateObject<T> operator^(TemplateObject<T> lhs, const T &rhs) { return (lhs ^= rhs); }
    friend inline TemplateObject<T> operator&(TemplateObject<T> lhs, const T &rhs) { return (lhs &= rhs); }
    friend inline TemplateObject<T> operator|(TemplateObject<T> lhs, const T &rhs) { return (lhs |= rhs); }

    friend inline T operator+(T &lhs, const TemplateObject<T> &rhs) { return (lhs += rhs.value()); }
    friend inline T operator-(T &lhs, const TemplateObject<T> &rhs) { return (lhs -= rhs.value()); }
    friend inline T operator*(T &lhs, const TemplateObject<T> &rhs) { return (lhs *= rhs.value()); }
    friend inline T operator/(T &lhs, const TemplateObject<T> &rhs) { return (lhs /= rhs.value()); }
    friend inline T operator%(T &lhs, const TemplateObject<T> &rhs) { return (lhs %= rhs.value()); }
    friend inline T operator^(T &lhs, const TemplateObject<T> &rhs) { return (lhs ^= rhs.value()); }
    friend inline T operator&(T &lhs, const TemplateObject<T> &rhs) { return (lhs &= rhs.value()); }
    friend inline T operator|(T &lhs, const TemplateObject<T> &rhs) { return (lhs |= rhs.value()); }

    friend inline std::ostream& operator<<(std::ostream& lhs, const TemplateObject<T>& rhs) { return (lhs << rhs.value()); }
    friend inline std::istream& operator>>(std::istream& lhs, const TemplateObject<T>& rhs) { return (lhs >> rhs.value()); }

    /*Equality/Relational Operators*/
    friend inline bool operator<(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs.value() < rhs.value()); }
    friend inline bool operator>(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs.value() > rhs.value()); }
    friend inline bool operator<=(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs.value() <= rhs.value()); }
    friend inline bool operator>=(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return  (lhs.value() >= rhs.value()); }
    friend inline bool operator==(TemplateObject<T> lhs, const TemplateObject<T>& rhs) { return (lhs.value() == rhs.value()); }

    friend inline bool operator<(const T &lhs, const TemplateObject<T> &rhs) { return (lhs < rhs.value()); }
    friend inline bool operator>(const T &lhs, const TemplateObject<T> &rhs) { return (lhs > rhs.value()); }
    friend inline bool operator<=(const T &lhs, const TemplateObject<T> &rhs) { return (lhs <= rhs.value()); }
    friend inline bool operator>=(const T &lhs, const TemplateObject<T> &rhs) { return  (lhs >= rhs.value()); }
    friend inline bool operator==(const T &lhs, const TemplateObject<T> &rhs) { return (lhs == rhs.value()); }

    friend inline bool operator<(const TemplateObject<T> &lhs, const T &rhs) { return (lhs.value() < rhs); }
    friend inline bool operator>(const TemplateObject<T> &lhs, const T &rhs) { return (lhs.value() > rhs); }
    friend inline bool operator<=(const TemplateObject<T> &lhs, const T &rhs) { return (lhs.value() <= rhs); }
    friend inline bool operator>=(const TemplateObject<T> &lhs, const T &rhs) { return  (lhs.value() >= rhs); }
    friend inline bool operator==(const TemplateObject<T> &lhs, const T &rhs) { return (lhs.value() == rhs); }

    /*Compound assignment operators*/
    TemplateObject<T> &operator+=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value + rhs.value();
        return *this;
    }

    TemplateObject<T> &operator-=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value - rhs.value();
        return *this;
    }
    TemplateObject<T> &operator*=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value * rhs.value();
        return *this;
    }
    TemplateObject<T> &operator/=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value / rhs.value();
        return *this;
    }
    TemplateObject<T> &operator%=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value % rhs.value();
        return *this;
    }
    TemplateObject<T> &operator^=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value ^ rhs.value();
        return *this;
    }
    TemplateObject<T> &operator&=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value & rhs.value();
        return *this;
    }
    TemplateObject<T> &operator|=(const TemplateObject<T> &rhs) {
        this->m_value = this->m_value | rhs.value();
        return *this;
    }

    TemplateObject<T> &operator+=(const T &rhs) {
        this->m_value = this->m_value + rhs;
        return *this;
    }

    TemplateObject<T> &operator-=(const T &rhs) {
        this->m_value = this->m_value - rhs;
        return *this;
    }
    TemplateObject<T> &operator*=(const T &rhs) {
        this->m_value = this->m_value * rhs;
        return *this;
    }
    TemplateObject<T> &operator/=(const T &rhs) {
        this->m_value = this->m_value / rhs;
        return *this;
    }
    TemplateObject<T> &operator%=(const T &rhs) {
        this->m_value = this->m_value % rhs;
        return *this;
    }
    TemplateObject<T> &operator^=(const T &rhs) {
        this->m_value = this->m_value ^ rhs;
        return *this;
    }
    TemplateObject<T> &operator&=(const T &rhs) {
        this->m_value = this->m_value & rhs;
        return *this;
    }
    TemplateObject<T> &operator|=(const T &rhs) {
        this->m_value = this->m_value | rhs;
        return *this;
    }

    /*toString using stringstream*/
    std::string toString() const {
        std::string returnString;
        std::stringstream transfer;
        transfer << this->m_value;
        transfer >> returnString;
        return returnString;
    }

    T value() const { return m_value; }
    operator T() { return this->m_value; }
    void setValue(T value) { this->m_value = value; }
    void setValue(const T &value) { this->m_value = value; }
    void setValue(TemplateObject<T> object) { this->m_value = object.value(); }
    void setValue(const TemplateObject<T> &object) { this->m_value = object.value(); }
    int getHashCode() const { return (this->m_value); }
protected:
        T m_value;
};


class Integer : public TemplateObject<int>
{
    using TemplateObject<int>::TemplateObject;
public:
    Integer(int value) : TemplateObject<int>({value}) {}
    operator int() { return this->m_value; }
};

class Long : public TemplateObject<long>
{
    using TemplateObject<long>::TemplateObject;
public:
    Long(long value) : TemplateObject<long>({value}) {}
    operator long() { return this->m_value; }
};

class Boolean : public TemplateObject<bool>
{
    using TemplateObject<bool>::TemplateObject;
public:
    Boolean(bool value) : TemplateObject<bool>({value}) {}
    operator bool() { return this->m_value; }
    std::string toString() { return (this->m_value) ? "True" : "False"; }

};

class Double : public TemplateObject<double>
{
    using TemplateObject<double>::TemplateObject;
public:
    Double(double value) : TemplateObject<double>({value}) {}
    operator double() { return this->m_value; }
};

class Float : public TemplateObject<float>
{
    using TemplateObject<float>::TemplateObject;
public:
    Float(float value) : TemplateObject<float>({value}) {}
    operator float() { return this->m_value; }
};

class Character : public TemplateObject<char>
{
    using TemplateObject<char>::TemplateObject;
public:
    Character(char value) : TemplateObject<char>({value}) {}
    operator char() { return this->m_value; }
};

#endif //TJLUTILS_TEMPLATEOBJECTS_H
