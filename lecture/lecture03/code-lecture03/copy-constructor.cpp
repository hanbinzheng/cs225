#include <iostream>

class aClass {
    public:
        int a;
        double b;
        aClass() {
            ;
        }

        aClass(const aClass & another) : a(another.a), b(another.b) {
            std::cout << "custom copy constructor called" << std::endl;
        }

        aClass & operator=(const aClass & another) {
            this->a = another.a;
            this->b = another.b;
            std::cout << "operator = is called" << std::endl;
            return *this;
        }
};

int main() {
    aClass a;
    aClass b = a;
    b = a;
    return 0;
}
