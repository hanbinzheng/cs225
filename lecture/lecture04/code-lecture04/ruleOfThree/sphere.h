#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>

namespace cs225 {

  class Sphere {

    public:
      Sphere();
      Sphere(double r);
      Sphere(const Sphere &s);
      ~Sphere();

      double getRadius() const;
      void setRadius(double r);
      double getVolume() const;
      void addProperty(const std::string &s);

      Sphere& operator=(const Sphere& s);

    private:
      double r_;
      std::string *props_;
      int props_max_;
      int props_ct_;

      void copy_(const Sphere &s);
      void destroy_();
  };
}

#endif

