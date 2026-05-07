
template <class T>
void Stack<T>::push(const T& t) {
  ListNode *node = new ListNode(t);
  node->next = head;
  head = node;
}

template <class T>
const T& Stack<T>::pop() {
  // missing error checking
  ListNode *node = head;
  const T& data = node->data;
  head = head->next;
  delete node;
  return data;
}

template <class T>
bool Stack<T>::isEmpty() const {
  return (head == nullptr);
}

template <typename T>
void Stack<T>::printStack() const {
  ListNode *node = head;
  while (node != nullptr) {
    const T& data = node->data;
    cout << data;
    node = node->next;
  }
  cout << endl;
}


template <typename T>
Stack<T>::Stack() : head(nullptr) {
}

template <typename T>
Stack<T>::Stack(const Stack &other) {
  // not implemented
}

template <typename T>
Stack<T>::~Stack() {
  // not implemented
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack &other) {
  // not implemented
  return *this;
}

