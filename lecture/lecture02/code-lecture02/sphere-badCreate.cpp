#include "sphere.h"
#include <iostream>

using namespace cs225;

Sphere *CreateUnitSphere() {
  Sphere s(1);
  return &s;
}

int main() {

  Sphere *s = CreateUnitSphere();

  double r = s->getRadius();
  std::cout << "r: " << r << std::endl;
  double v = s->getVolume();
  std::cout << "v: " << v << std::endl;

  return 0;
}


