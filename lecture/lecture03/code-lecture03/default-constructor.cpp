#include <iostream>

class B {
    public:
    int b;
    double c;
};

class A {
    public:
    int x;
    std::vector<int> v;
    B b;
};

class C {
    public:
    int x;
    std::vector<int> v;
    B b;

    C() {
        std::cout << "class C default constructor, fuck you cpp" << std::endl;
    }
};

/*
class D {
    public:
    int x;
    std::vector<int> v;
    B b;

    D(int r) : x(r){
        std::cout << "class D custom constructor, fuck you cpp" << std::endl;
    }
};
*/

int main() {
    // test behavior of class A, implicitly-declared default constructor
    A obj_a;
    A obj_a_ = A();

    // test behavior of class C, custom default constructor
    C obj_c;
    C obj_c_ = C();

    // test behavior of class D, no implicitly-declared default constructor
    // D obj_d; // compile error
    // D obj_d_ = D();
}
