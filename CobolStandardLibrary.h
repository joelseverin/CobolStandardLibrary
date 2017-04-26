#ifndef COBOLSTANDARDLIBRARY_H
#define COBOLSTANDARDLIBRARY_H

#include <cstdlib>

namespace cobol {

typedef unsigned char byte;

template<std::size_t L> struct ByteArray {
    private:
        byte data[L];
    
    public:
        ByteArray() : data() {}
        
        byte& operator[](std::size_t idx) {
            return data[idx];
        }

        const byte operator[](std::size_t idx) const {
            return data[idx];
        }
};

template<std::size_t N>
using Filler = ByteArray<N>;

template<std::size_t Integers,
        std::size_t Decimals = 0,
        bool Sign = false,
        bool ImpliedDecimal = true,
        bool SignFirst = false>
struct FormattedNumber;

template<std::size_t Integers,
        std::size_t Decimals = 0,
        bool Sign = false>
struct PackedFormattedNumber;

}

#include "FormattedNumber.h"
#include "PackedFormattedNumber.h"
#include "PIC.h"

#endif /* COBOLSTANDARDLIBRARY_H */

