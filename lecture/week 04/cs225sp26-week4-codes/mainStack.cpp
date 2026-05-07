#include <iostream>
#include "Stack.h"

// compile:  g++ -std=c++1y  mainStack.cpp

int main() {

    Stack <char> *stack = new Stack <char>;

    stack->push('5');
    stack->push('2');
    stack->push('2');
    stack->push('s');
    stack->push('c');

    stack->printStack();

    stack->pop();
    stack->pop();

    stack->printStack();

    stack->pop();
    stack->pop();
    stack->pop();

    delete stack;


    return 0;
}

