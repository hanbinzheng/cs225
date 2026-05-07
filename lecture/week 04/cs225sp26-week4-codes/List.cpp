// this implementation has no error checking !!!


template <class T>
void List<T>::insert(const T& t, unsigned index) {
  ListNode *node = new ListNode(t);
  ListNode *&ptr = find_(index);
  node->next = ptr;
  ptr=node;
}

template <class T>
void List<T>::printReverse() const {
  print_(head->next);
  cout << endl;
}

template <class T>
void List<T>::printForward() const {
  ListNode *cur = head->next;
  while (cur != nullptr) {
    cout << cur->data << " ";
    cur = cur->next;
  }
  cout << endl;
}

template <class T>
List<T>::List() : head(new ListNode((T)0))  {
}

template <class T>
List<T>::~List() {
  ListNode *thru = head;
  while (thru != nullptr) {
    ListNode *oldNode = thru;
    thru = thru->next;
    delete oldNode;
  }
}

template <class T>
T& List<T>::remove(unsigned index) {
  ListNode *& ptr = find_(index);
  ListNode *oldNode = ptr;
  ptr = ptr->next;
  T& data = oldNode->data;
  delete oldNode;
  return data;
}

template <class T>
T& List<T>::get(unsigned index) const {
  ListNode* &ptr = find_(index);
  return ptr->data;
}

template <class T>
bool List<T>::isEmpty() const {
  return (head->next == nullptr);
}

template <class T>
unsigned List<T>::length() const {
  ListNode *thru = head;
  unsigned count=0;
  while (thru->next != nullptr) {
    thru = thru->next;
    count++;
  }
  return count;
}

