#include "RedBall.h"

using namespace std;

namespace cs225 {

  RedBall::RedBall() : Sphere(1.0) {
    cout << "RedBall Default ctor" << endl;
    addProperty("Red");
  }


  RedBall::RedBall(double r) : Sphere(r) {
    cout << "RedBall 1-param ctor" << endl;
    addProperty("Red");
  }

}

