#include <iostream>

using namespace std;

int main() {

  int first = 42;
  int arr[6];

  cout << &first << endl;
  cout << &(arr[0]) << endl;
  cout << &(arr[1]) << endl;
  cout << &(arr[2]) << endl;

  return 0;
}
