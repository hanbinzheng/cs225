#include <list>
#include <string>
#include <iostream>

struct Animal {
  std::string name, food;
  bool big;
  Animal(std::string name = "blob", std::string food = "you", bool big = true) :    name(name), food(food), big(big) { /* none */ } };

int main() {
  Animal g("giraffe", "leaves", true), p("penguin", "fish", false), b("bear");
  std::list<Animal> zoo;

  zoo.push_back(g);
  zoo.push_back(p);   // std::list’s insertAtEnd
  zoo.push_back(b);

  for ( std::list<Animal>::iterator it = zoo.begin(); it != zoo.end(); it++ ) {    std::cout << (*it).name << " " << (*it).food << std::endl;
  }

  return 0;
}

