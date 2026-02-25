#include "sphere.h"
#include "joinSpheres.h"
#include <math.h>

namespace cs225 {

/*
 * Creates a new sphere that contains the exact volume * of the two input spheres.
 */
Sphere joinSpheres(const Sphere &s1, const Sphere &s2) {

  double totalVolume = s1.getVolume() + s2.getVolume();

  double newRadius = std::pow((3.0 * totalVolume) / (4.0 * 3.141592654), 1.0/3.0);

  return Sphere(newRadius);
}

}


