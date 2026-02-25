#include "sphere.h"
#include <iostream>

// compile: g++ sphere.cpp redSphere-main.cpp

using namespace std;

int main() {

  cs225::Sphere s1(10);
  s1.addProperty("Red");
  s1.addProperty("Rubber");

  cs225::Sphere s2;
  s2 = s1;

  s2.addProperty("Purple");
  s1.addProperty("Green");

  return 0;
}

