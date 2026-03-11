#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

template <class T>
class List {

  public:
    List(); 

    void insert(T &t, unsigned index);
    T& remove(unsigned index);
    T& get(unsigned index) const;
    bool isEmpty() const;
    unsigned length() const;

    void insertAtFront(const T& t);
    void printReverse() const;

  private:
    struct ListNode {
      const T data;
      ListNode * next;
      ListNode(const T& data) : data(data), next(nullptr) { };
    };

    ListNode *head;

    void print_(ListNode *cur) const;
};

#include "List.cpp"

#endif
