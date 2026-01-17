#include "sphere.h"
#include <iostream>

using namespace std;
using namespace cs225;

int main() {

  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(int *) << std::endl;

  std::cout << sizeof(cs225::Sphere) << std::endl;
  std::cout << sizeof(cs225::Sphere *) << std::endl;

  return 0;
}

