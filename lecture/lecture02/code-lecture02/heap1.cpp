#include "sphere.h"

using namespace cs225;

int main() {

  int *p = new int;
  Sphere *s = new Sphere(10); 

  delete p; p = nullptr;
  delete s; s = nullptr;

  return 0;
}

