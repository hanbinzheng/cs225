/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <class T> List<T>::~List() { clear(); }

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <class T> void List<T>::clear() {
    ListNode *curr = head_;
    ListNode *tmp = head_;

    while (curr != nullptr) {
        tmp = curr->next;
        delete curr;
        curr = tmp;
    }

    head_ = nullptr;
    tail_ = nullptr;
    length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T> void List<T>::insertFront(T const &ndata) {
    if (empty()) { /* head == tail == nullptr */
        ListNode *node = new ListNode(ndata);
        head_ = node;
        tail_ = node;
        length_++;
    } else {
        ListNode *node = new ListNode(ndata);
        head_->prev = node;
        node->next = head_;
        head_ = node;
        length_++;
    }
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T> void List<T>::insertBack(const T &ndata) {
    if (empty()) {
        ListNode *node = new ListNode(ndata);
        head_ = node;
        tail_ = node;
        length_++;
    } else {
        ListNode *node = new ListNode(ndata);
        node->prev = tail_;
        tail_->next = node;
        tail_ = node;
        length_++;
    }
}

/**
 * Reverses the current List.
 */
template <class T> void List<T>::reverse() { reverse(head_, tail_); }

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <class T>
void List<T>::reverse(ListNode *&startPoint, ListNode *&endPoint) {
    /* return if contains only one node or invalid case */
    if (startPoint == endPoint || startPoint == nullptr || endPoint == nullptr)
        return;

    /* interaction with parts beyond startPoint and endPoint */
    ListNode *prev_to_start = startPoint->prev;
    if (prev_to_start != nullptr)
        prev_to_start->next = endPoint;
    ListNode *next_to_end = endPoint->next;
    if (next_to_end != nullptr)
        next_to_end->prev = startPoint;

    ListNode *curr = startPoint;
    ListNode *next = startPoint->next;
    while (true) {
        ListNode *old_prev = curr->prev;
        ListNode *old_next = curr->next;

        if (curr == startPoint) {
            curr->next = next_to_end;
        } else {
            curr->next = old_prev;
        }

        if (curr == endPoint) {
            curr->prev = prev_to_start;
            break; /* to avoid the next->next wrong */
        } else {
            curr->prev = old_next;
        }

        curr = next;
        next = next->next;
    }

    /* swap the startPoint and endPoint */
    ListNode *tmp = startPoint;
    startPoint = endPoint;
    endPoint = tmp;

    /* standard implementation */
    // if (startPoint == endPoint || startPoint == nullptr || endPoint ==
    // nullptr)
    //     return;

    // ListNode *before_start = startPoint->prev;
    // ListNode *after_end = endPoint->next;

    // ListNode *curr = startPoint;
    // ListNode *stop = after_end;

    // while (curr != stop) {
    //     ListNode *tmp = curr->next;
    //     curr->next = curr->prev;
    //     curr->prev = tmp;
    //     curr = tmp;
    // }

    // startPoint->next = after_end;
    // if (after_end != nullptr) {
    //     after_end->prev = startPoint;
    // }

    // endPoint->prev = before_start;
    // if (before_start != nullptr) {
    //     before_start->next = endPoint;
    // }

    // ListNode *tmp = startPoint;
    // startPoint = endPoint;
    // endPoint = tmp;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <class T> void List<T>::reverseNth(int n) {
    if (n <= 1 || empty())
        return;
    if (n >= length_) {
        reverse();
        return;
    }

    ListNode *start = head_;
    ListNode *end = head_;

    while (start != nullptr) {
        end = start;
        for (int i = 1; i < n && end->next != nullptr; i++) {
            end = end->next;
        }

        ListNode *next_start = end->next;
        if (start == head_) {
            if (end == tail_) {
                reverse(head_, tail_);
            } else {
                reverse(head_, end); /* remember to update head */
            }
        } else {
            if (end == tail_) {
                reverse(start, tail_);
            } else {
                reverse(start, end);
            }
        }
        start = next_start;
    }
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <class T> void List<T>::waterfall() {
    if (empty() || length_ <= 2)
        return;

    ListNode *curr = head_;
    ListNode *to_move = head_->next;
    ListNode *tmp = nullptr;
    while (to_move != nullptr && to_move != tail_) {
        tail_->next = to_move;
        curr->next = to_move->next;
        to_move->next->prev = curr;
        to_move->next = nullptr;

        tail_ = to_move;
        curr = curr->next;
        to_move = curr->next;
    }
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <class T> List<T> List<T>::split(int splitPoint) {
    if (splitPoint > length_)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode *secondHead = split(head_, splitPoint);

    int oldLength = length_;
    if (secondHead == head_) {
        // current list is going to be empty
        head_ = NULL;
        tail_ = NULL;
        length_ = 0;
    } else {
        // set up current list
        tail_ = head_;
        while (tail_->next != NULL)
            tail_ = tail_->next;
        length_ = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head_ = secondHead;
    ret.tail_ = secondHead;
    if (ret.tail_ != NULL) {
        while (ret.tail_->next != NULL)
            ret.tail_ = ret.tail_->next;
    }
    ret.length_ = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <class T>
typename List<T>::ListNode *List<T>::split(ListNode *start, int splitPoint) {
    if (splitPoint <= 0 || start == nullptr)
        return start;

    ListNode *curr = start;
    while (curr->next != nullptr && splitPoint > 0) {
        splitPoint--;
        curr = curr->next;
    }

    if (splitPoint == 0) {
        curr->prev->next = nullptr;
        curr->prev = nullptr;
        return curr;
    } else {
        return nullptr;
    }
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <class T> void List<T>::mergeWith(List<T> &otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <class T>
typename List<T>::ListNode *List<T>::merge(ListNode *first, ListNode *second) {
    if (first == nullptr) {
        return second;
    } else if (second == nullptr) {
        return first;
    } /* this guarantee that the final list has at least 2 elements */

    /* a much more elegant implementation */
    // vector<ListNode *> vec_node;
    // while (first != nullptr && second != nullptr) {
    //     if (first->data < second->data) {
    //         vec_node.push_back(first);
    //         first = first->next;
    //     } else {
    //         vec_node.push_back(second);
    //         second = second->next;
    //     }
    // }

    // /* the left elements */
    // while (first != nullptr) {
    //     vec_node.push_back(first);
    //     first = first->next;
    // }
    // while (second != nullptr) {
    //     vec_node.push_back(second);
    //     second = second->next;
    // }

    // /* handle the boundary case */
    // vec_node[0]->prev = nullptr;
    // vec_node[0]->next = vec_node[1];
    // vec_node[vec_node.size() - 1]->next = nullptr;
    // vec_node[vec_node.size() - 1]->prev = vec_node[vec_node.size() - 2];

    // for (unsigned i = 1; i < vec_node.size() - 1; i++) {
    //     vec_node[i]->prev = vec_node[i - 1];
    //     vec_node[i]->next = vec_node[i + 1];
    // }

    // return vec_node[0];

    ListNode *start;
    if (first->data < second->data) {
        start = first;
        first = first->next;
    } else {
        start = second;
        second = second->next;
    }

    ListNode *curr = start;
    while (first != nullptr && second != nullptr) {
        if (first->data < second->data) {
            curr->next = first;
            first->prev = curr;
            first = first->next;
        } else {
            curr->next = second;
            second->prev = curr;
            second = second->next;
        }
        curr = curr->next;
    }

    if (first != nullptr) {
        curr->next = first;
        first->prev = curr;
    } else if (second != nullptr) {
        curr->next = second;
        second->prev = curr;
    }

    return start;
}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <class T> void List<T>::sort() {
    if (empty())
        return;
    head_ = mergesort(head_, length_);
    tail_ = head_;
    while (tail_->next != NULL)
        tail_ = tail_->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <class T>
typename List<T>::ListNode *List<T>::mergesort(ListNode *start,
                                               int chainLength) {
    if (chainLength <= 1) {
        return start;
    }

    int half_len = chainLength / 2;
    ListNode *second_half = split(start, half_len);
    start = mergesort(start, half_len);
    second_half = mergesort(second_half, chainLength - half_len);

    return merge(start, second_half);
}
