#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

#include "CobolStandardLibrary.h"

using namespace cobol;

int main(int argc, char** argv) {
    FormattedNumber<3, 2> f(333, 33);
    f.display();
    
    PackedFormattedNumber<3, 2> x(1, 03);
    x.display();
    
    PackedFormattedNumber<3, 2> y(3, 14);
    y.display();
    
    x += y + PackedFormattedNumber<3, 2>(4, 04) + FormattedNumber<3, 2>(1, 77);
    
    x.display();
    
    std::cout << "cos(fi) = " << cos(FormattedNumber<3, 2>(3, 14)) << std::endl;
    
    PIC<X<5>, C, Z<3>, D, N<2>> WS_HELLO("Hello,003.14");
    
    WS_HELLO.display();
}

