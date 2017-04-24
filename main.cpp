#include <cstdlib>
#include <cstring>
#include <iostream>

#include "CobolStandardLibrary.h"

using namespace cobol;

template<std::size_t L> class ALU {
    typedef ByteArray<L> unpacked_t;
    
    public:
        static unpacked_t add(unpacked_t lhs, unpacked_t rhs) {
            unpacked_t result;
            byte carry = 0;
            for(std::size_t i = L - 1; i-- > 0; ) {// watch out for overflow
                byte added = lhs[i] + rhs[i] + carry;
                if(added > 10) {
                    added = 0;
                    carry = 1;
                } else {
                    carry = 0;
                }

                result[i] = added;
            }
            
            if(carry) {
                throw "Overflow Exception";
            }
        }
};

int main(int argc, char** argv) {
    FormattedNumber<3, 2> f(333, 33);
    f.print();
    
    PackedFormattedNumber<3, 2> x(1, 03);
    x.print();
    
    PackedFormattedNumber<3, 2> y(3, 14);
    y.print();
    
    x += y + PackedFormattedNumber<3, 2>(4, 04) + FormattedNumber<3, 2>(1, 77);
    
    x.print();
}

