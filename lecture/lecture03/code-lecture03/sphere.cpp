#include "sphere.h"
#include <iostream>

using namespace std;

namespace cs225 {

  Sphere::Sphere() : Sphere(1.0) {
    cout << "Default ctor" << endl;
  }

  Sphere::Sphere(double r) {
    cout << "1-param ctor" << endl;
    r_ = r;
  }

  Sphere::Sphere(const Sphere &s) {
    cout << "copy ctor" << endl;
    r_ = s.r_;
  }


  double Sphere::getRadius() const {
    return r_;
  }

  void Sphere::setRadius(double r) {
    r_ = r;
  }

  double Sphere::getVolume() const {
    return (4 * r_ * r_ * r_ * 3.14159265) / 3.0;
  }

}
