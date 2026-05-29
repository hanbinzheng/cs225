/**
 * @file skipList.cpp
 * Doubly Linked Skip List (lab_gdb2), skip list implementation
 *
 * @author Arman Tajback - Created
 * @author Jordi Paris Ferrer & Arman Tajback - Modified
 * @date (created) Fall 2016
 * @date (modified) Spring 2017
 * @date (modified) Fall 2017
 */

#include "skipList.h"
#include "cs225/HSLAPixel.h"
#include "skipNode.h"

/**
 * Default constructs the SkipList.
 * Uses two sentinel nodes, each initially of height 1
 */
SkipList::SkipList() {
    head = new SkipNode();
    tail = new SkipNode();

    head->nodePointers[0].next = tail;
    tail->nodePointers[0].prev = head;

    head->key = INT_MIN;
    head->value = HSLAPixel();

    tail->key = INT_MAX;
    tail->value = HSLAPixel();

    listHeight = 1;
    length = 0;

    probability = 50;
    maxLevel = 14; // log(128 * 128)
}

/**
 * The assignment= operator.
 * @param other The SkipList to copy into ourselves.
 * @return The current object
 * @see copy()
 */
const SkipList &SkipList::operator=(const SkipList &other) {
    if (this != &other) {
        clear();
        copy(other);
    }

    return *this;
}

/**
 * Inserts a new node into the sorted order in the list, initialized with the
 * provided values. Will replace the value at key if it already exists
 * @param key The key to associate with the new Node
 * @param value The value to associate with the new Node
 */
void SkipList::insert(int key, HSLAPixel value) {
    /* if the node exists, simply replace its value */
    SkipNode *is_exist = find(key);
    if (is_exist != nullptr) {
        is_exist->value = value;
        return;
    }

    int new_level = levelGenerator();
    if (new_level > maxLevel)
        new_level = maxLevel;

    while (head->nodePointers.size() < new_level) {
        head->nodePointers.push_back(SkipPointer(tail, nullptr));
        tail->nodePointers.push_back(SkipPointer(nullptr, head));
    }

    SkipNode *new_node = new SkipNode(key, value, new_level);
    SkipNode *curr = head;

    for (int i = head->nodePointers.size() - 1; i >= 0; i--) {
        while (curr->nodePointers[i].next != tail &&
               curr->nodePointers[i].next->key < key) {
            curr = curr->nodePointers[i].next;
        }

        if (i < new_level) {
            SkipNode *next = curr->nodePointers[i].next;
            new_node->nodePointers[i].next = next;
            new_node->nodePointers[i].prev = curr;
            curr->nodePointers[i].next = new_node;
            next->nodePointers[i].prev = new_node;
        }
    }

    length++;
    if (listHeight < new_level)
        listHeight = new_level;
}
/**
 * A function that searches for the given key and returns the associated
 * HSLAPixel Returns (0, 0, 0, 50) if it's not found
 * @param key The key to search for
 * @return The pixel with the specified key, or (0, 0, 0, 50) if not found
 */
HSLAPixel SkipList::search(int key) {
    SkipNode *node = find(key);

    if (node == nullptr)
        return HSLAPixel(0, 0, 0, 50);

    return node->value;
}

/**
 * Finds the given key in the list if it exists, and returns a pointer to the
 * node containing it. Randomly calls findR or findI
 * @param key The key to search for in the list
 * @return A pointer to the node containing key, or NULL if not found in the
 * list
 */
SkipNode *SkipList::find(int key) {
    SkipNode *retval;
    if ((rand() % 2) == 0)
        retval = findR(key);
    else
        retval = findI(key);

    return retval;
}

/**
 * Finds the given key in the list if it exists, and returns a pointer to the
 * node containing it.
 * @param key The key to search for in the list
 * @return A pointer to the node containing key, or NULL if not found in the
 * list
 */
SkipNode *SkipList::findR(int key) {
    if (key == INT_MAX || key == INT_MIN)
        return nullptr;

    return findRHelper(key, head->nodePointers.size() - 1, head);
}

/**
 * Helper function for findR.
 * @param key The key to search for
 * @param level The level we're searching through
 * @param curr The current node we're searching through
 * @return A pointer to the node with the given key, or NULL if it could not be
 * found
 */
SkipNode *SkipList::findRHelper(int key, int level, SkipNode *curr) {
    if (key == INT_MAX || key == INT_MIN || level < 0)
        return nullptr;

    while (curr->nodePointers[level].next->key < key) {
        curr = curr->nodePointers[level].next;
    }

    if (curr->nodePointers[level].next->key == key) {
        return curr->nodePointers[level].next;
    } else {
        return findRHelper(key, level - 1, curr);
    }
}

/**
 * An iterative find function
 * @param key The key to search for
 * @return A pointer to the node with the given key, or NULL if it could not be
 * found
 *
 * test case:
 * 1. key = INT_MIN or INT_MAX (return nullptr instrad of sentries)
 * 2. key doesn 't exist ( general,  between head and the first/last and tail )
 * 3. key exists ( general, first one, last one )
 */
SkipNode *SkipList::findI(int key) {
    if (key == INT_MIN || key == INT_MAX)
        return nullptr;

    SkipNode *traverse = head;
    int level = head->nodePointers.size() - 1;

    while (level >= 0) {
        while (traverse->nodePointers[level].next->key < key) {
            traverse = traverse->nodePointers[level].next;
        }

        if (traverse->nodePointers[level].next->key == key) {
            return traverse->nodePointers[level].next;
        } else {
            level--;
        }
    }

    return nullptr;
}

/**
 * Removes the node with the given key from the list.
 * @param key The key to search for and remove from the list
 * @return A boolean indicating whether a node was successfully removed from the
 * list
 */
bool SkipList::remove(int key) {
    // don't allow the removal of sentinel nodes
    if (key == INT_MAX || key == INT_MIN)
        return false;

    SkipNode *node = find(key);
    if (node == nullptr)
        return false;

    for (int i = node->nodePointers.size() - 1; i >= 0; i--) {
        SkipNode *prev = node->nodePointers[i].prev;
        SkipNode *next = node->nodePointers[i].next;

        prev->nodePointers[i].next = next;
        next->nodePointers[i].prev = prev;
    }

    delete node;
    length--;
    return true;
}

/**
 * A function that returns the keys of the list in a vector, using only next
 * pointers.
 * @return a vector containing the keys from head to tail, including the
 * sentinel values
 */
vector<int> SkipList::traverse() {
    SkipNode *listPrintingTraverser = head;

    vector<int> ret;

    while (listPrintingTraverser != tail->nodePointers[0].next) {
        ret.push_back(listPrintingTraverser->key);

        listPrintingTraverser = listPrintingTraverser->nodePointers[0].next;
    }

    return ret;
}
