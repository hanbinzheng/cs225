#include "sphere.h"
#include <iostream>

int main() {
  cs225::Sphere s;
  cs225::Sphere *p = &s;

  std::cout << "&s: " << &s << std::endl;
  std::cout << "&p: " << &p << std::endl;

  return 0;
}

