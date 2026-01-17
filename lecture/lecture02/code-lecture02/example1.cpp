#include <iostream>

int main() {
  int a;
  int b = -3;
  int c = 12345;

  int *p = &b;

  std::cout << "&a: " << &a << std::endl;
  std::cout << "&b: " << &b << std::endl;
  std::cout << "&c: " << &c << std::endl;
  std::cout << "&p: " << &p << std::endl;

  return 0;
}

