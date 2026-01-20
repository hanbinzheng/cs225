#include <iostream>

int main() {
    int * x = new int;
    int & y = *x;

    y = 4;

    std::cout << "&x, on stack: " << &x << std::endl;
    std::cout << "x, on heap: " << x << std::endl;
    std::cout << "*x, value: " << *x << std::endl;

    std::cout << "y " << y << std::endl;
    std::cout << "&y: " << &y << std::endl;
    // std::cout << "*y" << *y << std::endl; // invalid

    return 0;
}
