
class Sphere {
  public:
    Sphere(double d) {  }
};

class Ball : public Sphere {
  public:
    Ball() : Sphere(1) {  }

};

int main() {
  Ball b;
  return 0;	
}

