#include "sphere.h"

namespace cs225 {

  Sphere::Sphere() : Sphere(1.0) {
  }

  Sphere::Sphere(double r) {
    r_ = r;
  }

  double Sphere::getRadius() {
    return r_;
  }

  void Sphere::setRadius(double r) {
    r_ = r;
  }

  double Sphere::getVolume() {
    return (4 * r_ * r_ * r_ * 3.14159265) / 3.0;
  }

}
