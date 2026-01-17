#include <iostream>

void f(int i) {
	int a = i;
	std::cout << "a: " << a << " Address of a: " << &a << std::endl;
	if (i > 0) {
		f(i - 1);
	}
}


int main(void) {
	f(3);
	return 0;
}
