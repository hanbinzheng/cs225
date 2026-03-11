#include <iostream>
#include "List.h"

// compile:  g++ -std=c++1y  mainList.cpp

int main() {

    List <char> *list = new List <char>;

    list->insertAtFront('s');
    list->insertAtFront('c');

    list->printReverse();

    delete list;


    return 0;
}

