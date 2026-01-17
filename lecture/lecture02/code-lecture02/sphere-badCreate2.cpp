#include "sphere.h"
#include <iostream>

using namespace std;
using namespace cs225;

Sphere *CreateUnitSphere() {
  Sphere s(1);
  return &s;
}

int main() {

  Sphere *s = CreateUnitSphere();

  cout << s->getRadius() << endl;

  cout << "s->getRadius(): " << s->getRadius() << endl;
  cout << "&s (main): " << &s << endl;
  cout << " s (main): " <<  s << endl;

  double r = s->getRadius();

  cout << "&r (main): " << &r << endl;
  cout << " r (main): " <<  r << endl;

  double v = s->getVolume();

  cout << "&v (main): " << &v << endl;
  cout << " v (main): " <<  v << endl;
}

