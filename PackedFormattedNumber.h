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
        // For an even number of digits, everything is shifted, so that the
        // first nibble is 0 and the last nibble contains the sign mask.
        // For an odd number of digits, the last nibble is the sign mask
        // TODO: double check this is true...
        typedef ByteArray<(Integers + Decimals + 1 + 1)/2> data_t;
        
        data_t data;
        
        static const byte SIGN_PLUS = 0xC;
        static const byte SIGN_MINUS = 0xD;
        static const byte SIGN_UNSIGNED = 0xF;
        
        byte getPure(std::size_t idx) const {
            std::size_t realIdx = idx;
            if(((Integers + Decimals) % 2) == 0) {
                // For even number of digits, we shift one (first nibble is 0).
                realIdx++;
            }
            
            byte picked = data[realIdx/2];
            return 0x0F & (((realIdx % 2) == 0) ? (picked >> 4) : picked);
        }
        
        void setPure(std::size_t idx, byte value) {
            std::size_t realIdx = idx;
            if(((Integers + Decimals) % 2) == 0) {
                // For even number of digits, we shift one (first nibble is 0).
                realIdx++;
            }
            
            byte& picked = data[realIdx/2];
            if((realIdx % 2) == 0) {
                picked = (picked & 0x0F) | ((value << 4) & 0xF0);
            } else {
                picked = (picked & 0xF0) | (value & 0x0F);
            }
        }
        
    public:
        template<bool ImpliedDecimal, bool SignFirst>
        PackedFormattedNumber(
                const FormattedNumber<Integers, Decimals, Sign, ImpliedDecimal, SignFirst>& unpacked) {
            memset(&data, 0, sizeof(data));
            
            for(std::size_t i = 0; i < Integers + Decimals; i++) {
                setPure(i, unpacked.getPure(i));
            }
            
            
            bool signMask = SIGN_UNSIGNED;
            
            if(Sign) {
                signMask = unpacked.isNegative() ? SIGN_MINUS : SIGN_PLUS;
            }
            
            data[sizeof(data) - 1] |= signMask;
        }
        
        template<bool ImpliedDecimal, bool SignFirst>
        operator FormattedNumber<Integers, Decimals, Sign, ImpliedDecimal, SignFirst>() const {
            return FormattedNumber<Integers, Decimals, Sign, ImpliedDecimal, SignFirst>(*this);
        }
              
        PackedFormattedNumber() {
            memset(data, 0, sizeof(data));
            
            data[sizeof(data) - 1] |= Sign ? SIGN_PLUS : SIGN_UNSIGNED;
        }
        
        PackedFormattedNumber(long integerWithSign, unsigned long decimal) :
                PackedFormattedNumber(
                    FormattedNumber<Integers, Decimals, Sign>(integerWithSign, decimal)) {
        };

        
        PackedFormattedNumber& operator+=(const PackedFormattedNumber& rhs) {
            FormattedNumber<Integers, Decimals, Sign> unpacked(*this);
            FormattedNumber<Integers, Decimals, Sign> rhsUnpacked(rhs);
            
            unpacked += rhsUnpacked;
            
            PackedFormattedNumber<Integers, Decimals, Sign> packed(unpacked);
            memcpy(&data, &(packed.data), sizeof(data));
            
            return *this;
        }

        PackedFormattedNumber operator+(const PackedFormattedNumber& rhs) {
            PackedFormattedNumber lhs(*this);
            lhs += rhs;
            return lhs;
        }
        
        void print() {
            FormattedNumber<Integers, Decimals, Sign> unpacked(*this);
            unpacked.print();
        }
};

}

#endif /* PACKEDFORMATTEDNUMBER_H */
