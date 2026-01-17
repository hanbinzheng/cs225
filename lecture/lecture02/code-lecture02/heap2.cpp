#include "sphere.h"

using namespace cs225;

int main() {

  Sphere *s1 = new Sphere();
  Sphere *s2 = s1;

  s2->setRadius( 10 );

  delete s1; s1 = s2 = nullptr;

  return 0;
}

