#include <cstdlib>
#include <cstring>
#include <iostream>

#include "CobolStandardLibrary.h"

using namespace cobol;

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

