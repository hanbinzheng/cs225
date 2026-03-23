#include "sphere.h"
#include <iostream>

using namespace std;

namespace cs225 {

  Sphere::Sphere() : Sphere(1.0) {
    cout << "Sphere Default ctor" << endl;
  }

  Sphere::Sphere(double r) {
    cout << "Sphere 1-param ctor" << endl;
    r_ = r;
    props_ = new std::string[5];
    props_max_ = 5;
    props_ct_ = 0;
  }

  Sphere::Sphere(const Sphere &s) {
    cout << "Sphere copy ctor" << endl;
    copy_(s);
  }

  Sphere::~Sphere() {
    cout << "Sphere dtor" << endl;
    destroy_();
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

  void Sphere::addProperty(const std::string &s) { 
    props_[props_ct_] = s; 
    props_ct_++; 
  }

  Sphere& Sphere::operator=(const Sphere& other) {
    cout << "Sphere = opt" << endl;
    if (this != &other) {
      destroy_();
      copy_(other);
    }
    return *this;
  }

  void Sphere::destroy_() {
    delete [] props_;
    props_ = nullptr;
  }

  void Sphere::copy_(const Sphere &other) {
    r_ = other.r_; 
    props_max_ = other.props_max_; 
    props_ct_ = other.props_ct_;
    props_ = new std::string[5]; 
    for (int i = 0; i < props_ct_; i++)
      props_[i] = other.props_[i];  
  }
}
