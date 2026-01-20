#include "sphere.h"
#include <math.h>

using namespace cs225;

/*
 * Creates a new sphere that contains the exact volume * of tthe two input spheres.
 */
Sphere joinSpheres(Sphere *s1, Sphere *s2) {

  double totalVolume = s1->getVolume() + s2->getVolume();

  double newRadius = std::pow((3.0 * totalVolume) / (4.0 * 3.141592654), 1.0/3.0);

  Sphere result(newRadius);

  return result;
}


int main() {

  Sphere *s1 = new Sphere(4);
  Sphere *s2 = new Sphere(5);

  Sphere s3 = joinSpheres(s1, s2);  
  
  return 0;
}


