#ifndef PIC_H
#define PIC_H

namespace cobol {

// Alphanumeric, cobol X, C++ X
template<std::size_t Repeat = 1>
class X {
    protected:
        char data[Repeat];
    
    public:
        X() : data() {}
        
        X(const char* input) {
            memcpy(&data, input, Repeat);
        }
        
        X(const char input) : data{input} { } // Means first input, rest 0 (if any)
        
        operator std::string() const {
            return std::string(data, Repeat);
        }
};

// Alphabetic, cobol A, C++ A
template<std::size_t Repeat = 1>
using A = X<Repeat>;

// Numeric, cobol 9, C++ N
template<std::size_t Repeat = 1>
using N = X<Repeat>;

// Decimal dot, cobol ., C++ D
class D : public X<1> {
    public:
        D() : X<1>('.') {}
};

// Comma, cobol ,, C++ C
class C : public X<1> {
    public:
        C() : X<1>(',') {}
};

// Implied decimal, cobol V, C++ V
class V {
    public:
        operator std::string() const {
            return std::string(",");
        }
};

// Trim leading zeroes, cobol Z, C++ Z
template<std::size_t Repeat = 1>
class Z : public X<Repeat> {
    public:
        operator std::string() const {
            std::string trimmed(this->data, Repeat);
            
            // Find leading zeroes, but skip the last if it contains all zero.
            std::size_t toFill = std::min(trimmed.find_first_not_of('0'),
                    trimmed.size() - 1);
            
            // Trim them off
            trimmed.erase(0, toFill);
            
            // Fill up and concatenate
            return std::string(toFill, ' ') + trimmed;
        }
};

template<typename... Tail>
class PIC {
    public:
        // Halt recursion
        PIC(const char* input) { }
        void display() const {
            std::cout << std::endl;
        }
};

template<typename Head, typename... Tail>
class PIC<Head, Tail...> : PIC<Tail...> {
    Head head;
    
    public:
        PIC(const char* input) : PIC<Tail...>(input + sizeof(Head)), head() {
            memcpy(&head, input, sizeof(Head));
        }
        
        void display() const {
            std::cout << static_cast<std::string>(head);
            PIC<Tail...>::display();
        }
};

}

#endif /* PIC_H */
