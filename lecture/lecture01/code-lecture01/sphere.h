#ifndef SPHERE_H
#define SPHERE_H

namespace cs225 {

  class Sphere {
    public:
      Sphere();
      Sphere(double r);
      double getRadius();
      double getVolume();

    private:
      double r_;
  };
}

#endif

