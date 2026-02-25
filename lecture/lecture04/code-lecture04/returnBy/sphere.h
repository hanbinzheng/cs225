#ifndef SPHERE_H
#define SPHERE_H

namespace cs225 {

  class Sphere {

    public:
      Sphere();
      Sphere(double r);
      Sphere(const Sphere &s);

      double getRadius() const;
      void setRadius(double r);
      double getVolume() const;

    private:
      double r_;
  };
}

#endif

