#ifndef TJLDATASTRUCTURES_BITSET_H
#define TJLDATASTRUCTURES_BITSET_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <exception>
#include <stdexcept>

#ifdef SMALL_BUFFER_SIZE
    #undef SMALL_BUFFER_SIZE
#endif
#define SMALL_BUFFER_SIZE 255

enum Endian
{ 
    LittleEndian, 
    BigEndian
};

template <size_t N>
class bitset
{
public:
    bitset(const char *str, Endian endian = Endian::LittleEndian) :
        m_numberOfBits{N},
        m_underlyingValue{0},
        m_endian{endian}
    {
        this->setFromString(str);
    }

    bitset(size_t initialValue, Endian endian = Endian::LittleEndian) :
        m_numberOfBits{N},
        m_underlyingValue{0},
        m_endian{endian}

    {
        this->setMultiple(initialValue);
    }

    bitset(Endian endian = Endian::LittleEndian) :
        m_numberOfBits{N},
        m_underlyingValue{0},
        m_endian{endian}
    {

    }

    bitset<N> &setFromString(const char *str)
    {
        std::string temp{str};
        if (temp.find("0b") == 0) {
            temp = temp.substr(2);
        }
        if (temp.length() != this->m_numberOfBits) {
            throw std::runtime_error("ERROR: In bitset<N>::setFromString(const char *): input string length must be equal to number of bits (" + std::to_string(temp.length()) + " != " + std::to_string(this->m_numberOfBits) + ")");
        }
        for (size_t i = 0; i < temp.length(); i++) {
            if (temp[i] == '0') {
                this->reset(i);
            } else {
                this->set(i);
            }
        }
        return *this;
    }
    
    //Set all bits
    bitset<N> &set()
    {
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            this->set(i);
        }
        return *this;
    }

    //Set single bit, range checked, throws std::out_of_range
    bitset<N> &set(size_t whichOne)
    {
        if (whichOne >= this->m_numberOfBits) {
            throw std::runtime_error("ERROR: In bitset<N>::set(size_t): index must be less than number of bits (" + std::to_string(whichOne) + " > " + std::to_string(this->m_numberOfBits) + ")");
        } else if (this->m_endian == Endian::LittleEndian) {
            this->m_underlyingValue |= 1 << (this->m_numberOfBits - whichOne - 1);
        } else {
            this->m_underlyingValue |= 1 << whichOne;
        }
        return *this;
    }

    //Sets multiple bits, from a passed in integer, nothrow (if byte is too large, it is truncated to fit)
    bitset<N> &setMultiple(size_t whatByte)
    {
        //TODO: Mask out unused bits
        for (unsigned i = 0; i < this->m_numberOfBits; i++) {
            ((whatByte >> i) & 1) ? this->set(i) : this->reset(i);
        }
        return *this;
    }
    
    //Reset all bits
    bitset<N> &reset()
    {
        this->m_underlyingValue = 0;
        return *this;
    }

    //Reset single bit, range checked, throws std::out_of_range
    bitset<N> &reset(size_t whichOne)
    {
        if (whichOne > this->m_numberOfBits) {
            throw std::runtime_error("ERROR: In bitset<N>::reset(size_t): index must be less than number of bits (" + std::to_string(whichOne) + " > " + std::to_string(this->m_numberOfBits) + ")");
        } else if (this->m_endian == Endian::LittleEndian) {
            this->m_underlyingValue &= ~(1 << (this->m_numberOfBits - whichOne - 1));
        } else {
            this->m_underlyingValue &= ~(1 << whichOne);
        }
        return *this;
    }
    
    bitset<N> &flip() //Flip all bits
    {
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            this->test(i) ? this->reset(i) : this->set(i);
        }
        return *this;
    }
    
    //Flip single bit, range checked throws std::out_of_range
    bitset<N> &flip(size_t whichOne)
    {
        if (whichOne > this->m_numberOfBits) {
            throw std::runtime_error("ERROR: In bitset<N>::flip(size_t): index must be less than number of bits (" + std::to_string(whichOne) + " > " + std::to_string(this->m_numberOfBits) + ")");
        }
        this->test(whichOne) ? this->reset(whichOne) : this->set(whichOne);
        return *this;
    }

    //Check if any bit is set
    bool any() const
    {
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            if (this->test(i)) {
                return true;
            }
        }
        return false;
    }
    
    //Check if no bits are set
    bool none() const
    {
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            if (this->test(i)) {
                return false;
            }
        }
        return true;
    }
    
    //Check if all bits are set
    bool all() const
    {
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            if (!this->test(i)) {
                return false;
            }
        }
        return true;
    }

    //Check single bit, range checked, throws std::out_of_range
    bool test(size_t whichOne) const
    {
        if (whichOne > this->m_numberOfBits) {
            throw std::runtime_error("ERROR: In bitset<N>::test(size_t): index must be less than number of bits (" + std::to_string(whichOne) + " > " + std::to_string(this->m_numberOfBits) + ")");
        } else if (this->m_endian == Endian::LittleEndian) {
            return static_cast<bool>((this->m_underlyingValue >> (this->m_numberOfBits - whichOne - 1)) & 1);
        } else {
            return static_cast<bool>((this->m_underlyingValue >> whichOne) & 1);
        }
    }
    
    //Check single bit, non range checked
    bool operator[](size_t whichOne) const
    {
        if (this->m_endian == Endian::LittleEndian) {
            return static_cast<bool>((this->m_underlyingValue >> (this->m_numberOfBits - whichOne - 1)) & 1);
        } else {
            return static_cast<bool>((this->m_underlyingValue >> whichOne) & 1);
        }
    }

    //Number of set bits
    size_t count() const
    {
        size_t returnCount{0};
        for (size_t i = 0; i < this->m_numberOfBits; i++) {
            if (this->test(i)) {
                returnCount++;
            }
        }
        return returnCount;
    }
    
    //Total number of bits
    size_t size() const
    {
        return this->m_numberOfBits;
    }

    //Endianness of bitset
    Endian endian() const
    {
        return this->m_endian;
    }

    //Retrieve underlying value of bitset
    size_t underlyingValue() const
    {
        return this->m_underlyingValue;
    }

    //String representation
    size_t toString(char *out, uint8_t spacing = 0) const
    {
        char temp[2];
        memset(temp, '\0', 2);
        size_t maximumSize = this->m_numberOfBits  + (spacing * (this->m_numberOfBits - 1)) + 1;
        if (this->m_endian == Endian::LittleEndian) {
            for (size_t i = this->m_numberOfBits - 1; i >= 0; i--) {
                snprintf(temp, 2, "%i", this->operator[](i));
                if (i == (this->m_numberOfBits - 1)) {
                    strncpy(out, temp, maximumSize);
                } else {
                    strncat(out, temp, maximumSize);
                }
                if ((i - 1) >= 0) {
                    for (uint8_t spaceCount = 0; spaceCount < spacing; spaceCount++) {
                        strncat(out, " ", maximumSize);
                    }
                }
                if (i == 0) {
                    break;
                }
            }
        } else if (this->m_endian == Endian::BigEndian) {
            for (size_t i = 0; i < this->m_numberOfBits; i++) {
                snprintf(temp, 2, "%i", this->operator[](i));
                if (i == 0) {
                    strncpy(out, temp, maximumSize);
                } else {
                    strncat(out, temp, maximumSize);
                } 
                if ((i + 1) != this->m_numberOfBits) {
                    for (uint8_t spaceCount = 0; spaceCount < spacing; spaceCount++) {
                        strncat(out, " ", maximumSize);
                    }
                }
            }
        } else {

        }
        return strlen(out);
    }

    std::string toString()
    {
        char temp[SMALL_BUFFER_SIZE];
        memset(temp, '\0', SMALL_BUFFER_SIZE);
        this->toString(temp, SMALL_BUFFER_SIZE);
        return std::string{temp};
    }

    //&= operator overload
    bitset<N> &operator&= (const bitset<N> &rhs)
    {
        size_t minimumSize{this->size() < rhs.size() ? this->size() : rhs.size()};
        for (size_t i = 0; i < minimumSize; i++) {
            (rhs[i] && this[i]) ? this->set(i) : this->reset(i);
        } 
        return *this;
    }

    //|= operator overload
    bitset<N> &operator|= (const bitset<N> &rhs)
    {
        size_t minimumSize{this->size() < rhs.size() ? this->size() : rhs.size()};
        for (size_t i = 0; i < minimumSize; i++) {
            (rhs[i] || this[i]) ? this->set(i) : this->reset(i);
        }
        return *this; 
    }

    //^= operator overload
    bitset<N> &operator^= (const bitset<N> &rhs)
    {
        size_t minimumSize{this->size() < rhs.size() ? this->size() : rhs.size()};
        for (size_t i = 0; i < minimumSize; i++) { 
            (rhs[i] != this[i]) ? this->set(i) : this->reset(i);
        } 
        return *this;
    }

    //<<= operator overload (bit shift, not extraction)
    bitset<N> &operator<<= (size_t pos)
    {
        //TODO: Implement
        return *this;
    }

    //>>= operator overload (bit shift, not insertion)
    bitset<N> &operator>>= (size_t pos)
    {
        //TODO: Implement
        return *this;
    }

    //<< operator overload (bit shift, not extraction)
    bitset<N> operator<<(size_t pos) const
    {
        //TODO: Implement
        return *this;
    }

    //>> operator overload (bit shift, not insertion)
    bitset<N> operator>>(size_t pos) const
    {
        //TODO: Implement
        return *this;
    }

    //~ operator overload (bitwise NOT)
    bitset<N> &operator~()
    {
        this->flip();
        return *this;
    }

    //Member operator ==
    bool operator==(const bitset<N> &rhs)
    {
        if (this->size() != rhs.size()) {
            return false;
        }
        return (this->count() == rhs.count());
    }

    //Member operator !=
    bool operator!=(const bitset<N> &rhs)
    {
        return !(*this == rhs);
    }

private:
    size_t m_numberOfBits;
    size_t m_underlyingValue;
    Endian m_endian;

};

//Non-member operator ==
template <size_t N>
bool operator==(const bitset<N> &lhs, const bitset<N> &rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs.sie(); i++) {
        if (lhs.test(i) != rhs.test(i)) {
            return false;
        }
    }
    return true;
}

//Non-member operator & (AND)
template <size_t N>
bitset<N> operator& (const bitset<N> &lhs, const bitset<N> &rhs)
{
    bitset<N> returnBitset;
    size_t minimumSize{lhs.size() < rhs.size() ? lhs.size() : rhs.size()};
    for (size_t i = 0; i < minimumSize; i++) {
        (lhs[i] && rhs[i]) ? returnBitset.set(i) : returnBitset.reset(i);
    }
    return returnBitset;
}

//Non-member operator | (OR)
template <size_t N>
bitset<N> operator| (const bitset<N> &lhs, const bitset<N> &rhs)
{
    bitset<N> returnBitset;
    size_t minimumSize{lhs.size() < rhs.size() ? lhs.size() : rhs.size()};
    for (size_t i = 0; i < minimumSize; i++) {
        (lhs[i] || rhs[i]) ? returnBitset.set(i) : returnBitset.reset(i);
    }
    return returnBitset;
}

//Non-member operator ^ (XOR)
template <size_t N>
bitset<N> operator^ (const bitset<N> &lhs, const bitset<N> &rhs)
{
    bitset<N> returnBitset;
    size_t minimumSize{lhs.size() < rhs.size() ? lhs.size() : rhs.size()};
    for (size_t i = 0; i < minimumSize; i++) {
        (lhs[i] != rhs[i]) ? returnBitset.set(i) : returnBitset.reset(i);
    }
    return returnBitset;
}

#endif //ARDUINOPC_BITSET_H
