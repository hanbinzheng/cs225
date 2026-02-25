#ifndef SPHERE_H
#define SPHERE_H

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

      Sphere operator+(const Sphere& s);
      Sphere& operator=(const Sphere& s);

    private:
      double r_;
  };
}

#endif

