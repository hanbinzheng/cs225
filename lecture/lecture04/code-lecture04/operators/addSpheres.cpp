#include "sphere.h"
#include "joinSpheres.h"
#include <iostream>

// compile: g++ sphere.cpp joinSpheres.cpp addSpheres.cpp

using namespace std;

int main() {

  cs225::Sphere s1(3), s2(4);
  cs225::Sphere s3 = s1 + s2;

  cout << s3.getRadius() << endl;

  return 0;
}

