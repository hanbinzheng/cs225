#include "sphere.h"
#include "joinSpheres.h"
#include <iostream>

// compile: g++ sphere.cpp joinSpheres.cpp assignSpheres.cpp

using namespace std;

int main() {

  cs225::Sphere s1(3), s2(4);

  cout << s1.getRadius() << endl;
  cout << s2.getRadius() << endl;

  s1 = s2;

  cout << s1.getRadius() << endl;
  cout << s2.getRadius() << endl;

  return 0;
}

