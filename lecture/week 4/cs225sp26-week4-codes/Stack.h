#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

template <class T>
class Stack {

  public:
    Stack(); 
    Stack(const Stack &other);
    ~Stack();
    Stack& operator=(const Stack &other);

    void push(const T &t);
    const T& pop();
    bool isEmpty() const;

    void printStack() const;

  private:
    struct ListNode {
      const T & data;
      ListNode * next;
      ListNode(const T& data) : data(data), next(nullptr) { };
    };

    ListNode *head;
};

#include "Stack.cpp"

#endif
