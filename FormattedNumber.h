#ifndef FORMATTEDNUMBER_H
#define FORMATTEDNUMBER_H

//include "CobolStandardLibrary.h"

namespace cobol {

template<std::size_t Integers,
        std::size_t Decimals,
        bool Sign,
        bool ImpliedDecimal,
        bool SignFirst>
struct FormattedNumber {
    friend PackedFormattedNumber<Integers, Decimals, Sign>;
    
    public:
        typedef ByteArray<Integers + Decimals + (ImpliedDecimal ? 0 : 1)> data_t;
        data_t data;
        
    private:
        static const std::size_t DECIMALS_END = sizeof(data_t) - 1;
        static const std::size_t DECIMALS_START = DECIMALS_END + 1 - Decimals;
        
        static const std::size_t INTEGERS_END = DECIMALS_START - (ImpliedDecimal ? 1 : 2);
        static const std::size_t INTEGERS_START = 0;
        
        static const std::size_t SIGN_POSITION = SignFirst ? 0 : sizeof(data_t) - 1;
        static const byte SIGN_PLUS = 0xC0;
        static const byte SIGN_MINUS = 0xD0;
        static const byte SIGN_UNSIGNED = 0xF0;
        
        static const std::size_t NONIMPLIED_DECIMAL_POSITION = ImpliedDecimal ? -1 : Integers;
        static const char NONIMPLIED_DECIMAL_CHAR = 0x4B; // EBCDIC dot
    
        byte getPure(std::size_t idx) const {
            if(idx < Integers) {
                return data[idx] & 0x0F;
            } else {
                return data[idx + (ImpliedDecimal ? 0 : 1)] & 0x0F;
            }
        }

        void setPure(std::size_t idx, byte value) {
            std::size_t realIdx = idx;
            if(!ImpliedDecimal && idx >= Integers) {
                realIdx++;
            }
            
            data[realIdx] = (data[realIdx] & 0xF0) | (value & 0x0F);
        }
        
        bool isNegative() const {
            return Sign && ((data[SIGN_POSITION] & 0xF0) == SIGN_MINUS);
        }
        
    public:
        FormattedNumber() {
            memset(data, 0xF0, sizeof(data));
            
            if(!ImpliedDecimal) {
                data[NONIMPLIED_DECIMAL_POSITION] = NONIMPLIED_DECIMAL_CHAR;
            }
            
            if(Sign) {
                data[SIGN_POSITION] = (data[SIGN_POSITION] & 0x0F) | SIGN_PLUS;
            }
        }
        
        FormattedNumber(long integerWithSign, unsigned long decimal) {
            long integer = abs(integerWithSign);
            
            unsigned long base = 1;
            for(std::size_t i = DECIMALS_END + 1; i-- > DECIMALS_START; ) {
                unsigned long value = (decimal / base) % 10;
                data[i] = value | 0xF0;
                base *= 10;
            }

            if(!ImpliedDecimal) {
                data[NONIMPLIED_DECIMAL_POSITION] = NONIMPLIED_DECIMAL_CHAR;
            }

            base = 1;
            for(std::size_t i = INTEGERS_END + 1; i-- > INTEGERS_START; ) {
                unsigned long value = (integer / base) % 10;
                data[i] = value | 0xF0;
                base *= 10;
            }
            
            if(Sign) {
                byte bitmask = (integerWithSign < 0) ? SIGN_MINUS : SIGN_PLUS;
                data[SIGN_POSITION] = (data[SIGN_POSITION] & 0x0F) | bitmask;
            }
        }
        
        FormattedNumber(const PackedFormattedNumber<Integers, Decimals, Sign>& packed) {
            for(std::size_t i = 0; i < sizeof(data_t); i++) {
                if(!ImpliedDecimal && i == NONIMPLIED_DECIMAL_POSITION) {
                    data[i] = NONIMPLIED_DECIMAL_CHAR;
                } else {                
                    byte part = packed.data[i/2];
                    data[i] = 0x0F & ((i%2) == 0 ? (part >> 4) : part);
                }
            }
        }
        
        FormattedNumber& operator+=(const FormattedNumber& rhs) {
            byte carry = 0;
            for(std::size_t i = Integers + Decimals /*- 1 + 1*/; i-- > 0; ) {
                unsigned short added = getPure(i) + rhs.getPure(i) + carry;
                
                if(added >= 10) {
                    added -= 10;
                    carry = 1;
                } else {
                    carry = 0;
                }
                
                setPure(i, added);
            }
            
            if(carry > 0) {
                // TODO: raise overflow signal
            }
            
            return *this;
        }

        FormattedNumber operator+(const FormattedNumber& rhs) {
            FormattedNumber lhs(this);
            lhs += rhs;
            return lhs;
        }
        
        void print() {
            std::cout << "[";
            
            for(std::size_t i = INTEGERS_START; i <= INTEGERS_END; i++) {
                std::cout << (data[i] & 0x0F);
            }
            
            if(Decimals > 0) {
                std::cout << ".";
            }
            
            for(std::size_t i = DECIMALS_START; i <= DECIMALS_END; i++) {
                std::cout << (data[i] & 0x0F);
            }
            
            std::cout << "]";
            std::cout << std::endl;
        }
};

}

#endif /* FORMATTEDNUMBER_H */
