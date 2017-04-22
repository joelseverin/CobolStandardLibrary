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
        static const std::size_t DECIMALS_END = 4;//sizeof(data_t) - 1;
        static const std::size_t DECIMALS_START = DECIMALS_END + 1 - Decimals;
        
        static const std::size_t INTEGERS_END = DECIMALS_START - (ImpliedDecimal ? 1 : 2);
        static const std::size_t INTEGERS_START = 0;
        
        static const std::size_t SIGN_POSITION = SignFirst ? 0 : sizeof(data_t) - 1;
        static const std::size_t NONIMPLIED_DECIMAL_POSITION = ImpliedDecimal ? -1 : Integers;
        static const char NONIMPLIED_DECIMAL_CHAR = 0x4B; // EBCDIC dot
        
    public:
        FormattedNumber() : data() {
        }
        
        FormattedNumber(unsigned long integer, unsigned long decimal) {
            unsigned long base = 1;
            for(std::size_t i = DECIMALS_END + 1; i-- > DECIMALS_START; ) {
                unsigned long value = (decimal / base) % 10;
                data[i] = value,
                base *= 10;
            }

            if(!ImpliedDecimal) {
                data[NONIMPLIED_DECIMAL_POSITION] = NONIMPLIED_DECIMAL_CHAR;
            }

            base = 1;
            for(std::size_t i = INTEGERS_END + 1; i-- > INTEGERS_START; ) {
                unsigned long value = (integer / base) % 10;
                data[i] = value,
                base *= 10;
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
        
        void print() {
            std::cout << "[";
            
            for(std::size_t i = INTEGERS_START; i <= INTEGERS_END; i++) {
                std::cout << data[i];
            }
            
            if(Decimals > 0) {
                std::cout << ".";
            }
            
            for(std::size_t i = DECIMALS_START; i <= DECIMALS_END; i++) {
                std::cout << data[i];
            }
            
            std::cout << "]";
            std::cout << std::endl;
        }
};

}

#endif /* FORMATTEDNUMBER_H */
