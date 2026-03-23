
class Sphere {
  public:
    Sphere(double d) {  }
    Sphere() : Sphere(1) {  }
};

class Ball : public Sphere {

};

int main() {
  Ball b;
  return 0;	
}

