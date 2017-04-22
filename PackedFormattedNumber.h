#ifndef PACKEDFORMATTEDNUMBER_H
#define PACKEDFORMATTEDNUMBER_H

//include "CobolStandardLibrary.h"

namespace cobol {

template<std::size_t Integers,
        std::size_t Decimals,
        bool Sign>
struct PackedFormattedNumber {
    friend FormattedNumber<Integers, Decimals, Sign>;
    
    private:
        // +1 to round up to whole bytes, and +1 for sign (ALWAYS present!)
        // The sign is 0x0C for positive, 0x0D for negative, 0x0F for unsigned
        // For an oddd number of digits+decimals, everything is shifted.
        // For an even number of digits+decimals, the last byte is the sign mask
        // TODO: double check this is true...
        typedef ByteArray<(Integers + Decimals + 1 + 1)/2> data_t;
        
        data_t data;
        
    public:
        template<bool ImpliedDecimal, bool SignFirst>
        PackedFormattedNumber(
                const FormattedNumber<Integers, Decimals, Sign, ImpliedDecimal, SignFirst>& unpacked) {
            const std::size_t length = sizeof(typename FormattedNumber<Integers, Decimals, Sign>::data_t);
            
            // Even length: Append sign in LSB, keep 
            
            // if((length % 0) == 1)
            
            for(std::size_t i = 0; i < length; i++) {
                if((i%2) == 0) {
                    data[i/2] = unpacked.data[i] << 4;
                } else {
                    data[i/2] |= unpacked.data[i] & 0xF;
                }
            }
        }
              
        PackedFormattedNumber() : data() {
        }
        
        PackedFormattedNumber(unsigned long integer, unsigned long decimal) {
            PackedFormattedNumber(
                    FormattedNumber<Integers, Decimals, Sign>(integer, decimal));
        }
        
        PackedFormattedNumber& operator+=(const PackedFormattedNumber& rhs) {
            FormattedNumber<Integers, Decimals, Sign> unpacked(*this);
            FormattedNumber<Integers, Decimals, Sign> rhsUnpacked(*this);
            
            return *this;
        }

        /*PackedUnsignedFormattedNumber operator+(PackedUnsignedFormattedNumber lhs,
                const PackedUnsignedFormattedNumber& rhs) {
            lhs += rhs;
            return lhs;
        }*/
        
        void print() {
            FormattedNumber<Integers, Decimals, Sign> unpacked(*this);
            unpacked.print();
        }
};

}

        
        /*    std::size_t index = 0;
            
            int base = 1;
            for(std::size_t i = 0; i < Integers; i++) {
                int value = (integer / base) % 10;
                
                if((index % 2) == 0) {
                    data[index/2] = value << 4;
                } else {
                    data[index/2] |= value & 0xF;
                }
                
                base *= 10;
                index++;
            }
            
            base = 1;
            for(std::size_t i = 0; i < Decimals; i++) {
                int value = (decimal / base) % 10;
                
                if((index % 2) == 0) {
                    data[index/2] = value << 4;
                } else {
                    data[index/2] |= value & 0xF;
                }
                
                base *= 10;
                index++;
            }*/

#endif /* PACKEDFORMATTEDNUMBER_H */
