
template <class T>
void List<T>::insertAtFront(const T& t) {
  ListNode *node = new ListNode(t);
  node->next = head;
  head = node;
}

template <class T>
void List<T>::printReverse() const {
  if (head != nullptr)
    print_(head);
}

template <class T>
void List<T>::print_(ListNode *cur) const {
  if(cur->next != nullptr)
    print_(cur->next);
  cout << cur->data << endl;
}

template <typename T>
List<T>::List() : head(nullptr) {
}

template <class T>
void List<T>::insert(T &t, unsigned index) {
  // not implemented
}

template <class T>
T& List<T>::remove(unsigned index) {
  // not implemented
  return nullptr;
}

template <class T>
T& List<T>::get(unsigned index) const {
  // not implemented
  return nullptr;
}

template <class T>
bool List<T>::isEmpty() const {
  return (head == nullptr);
}

template <class T>
unsigned List<T>::length() const {
  // not implemented
  return 0;
}

