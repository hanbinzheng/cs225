/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T> int BinaryTree<T>::height() const {
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T> int BinaryTree<T>::height(const Node *subRoot) const {
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T> void BinaryTree<T>::printLeftToRight() const {
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    cout << endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node *subRoot) const {
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

template <typename T> void BinaryTree<T>::mirror(Node *&sub_root) {
    if (sub_root == nullptr)
        return;

    mirror(sub_root->left);
    mirror(sub_root->right);
    Node *tmp = sub_root->left;
    sub_root->left = sub_root->right;
    sub_root->right = tmp;
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T> void BinaryTree<T>::mirror() { mirror(root); }

template <typename T>
void BinaryTree<T>::in_order(Node *sub_root, vector<T> &tree_vector) const {
    if (sub_root != nullptr) {
        in_order(sub_root->left, tree_vector);
        tree_vector.push_back(sub_root->elem);
        in_order(sub_root->right, tree_vector);
    }
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T> bool BinaryTree<T>::isOrdered() const {
    vector<T> result_in_order;
    in_order(root, result_in_order);

    if (result_in_order.size() <= 1)
        return true;

    for (unsigned size = 0; size < result_in_order.size() - 1; size++) {
        if (result_in_order[size] > result_in_order[size + 1])
            return false;
    }

    return true;
}

template <typename T>
void BinaryTree<T>::get_all_paths(vector<vector<T>> &paths,
                                  vector<T> &curr_path, Node *sub_root) const {
    if (sub_root == nullptr)
        return;

    curr_path.push_back(sub_root->elem);
    if (sub_root->left == nullptr && sub_root->right == nullptr) {
        paths.push_back(curr_path);
    } else {
        get_all_paths(paths, curr_path, sub_root->left);
        get_all_paths(paths, curr_path, sub_root->right);
    }
    curr_path.pop_back();
}

/**
 * creates vectors of all the possible paths from the root of the tree to any
 * leaf node and adds it to another vector. Path is, all sequences starting at
 * the root node and continuing downwards, ending at a leaf node. Paths ending
 * in a left node should be added before paths ending in a node further to the
 * right.
 * @param paths vector of vectors that contains path of nodes
 */
template <typename T>
void BinaryTree<T>::printPaths(vector<vector<T>> &paths) const {
    vector<T> curr_path;
    get_all_paths(paths, curr_path, root);
}

/* treat sub_root as root, calculate the sum of subtree */
template <typename T>
int BinaryTree<T>::sum_distance(Node *sub_root, int sub_root_depth) const {
    if (sub_root == nullptr)
        return 0;
    int left_child_sum = sum_distance(sub_root->left, sub_root_depth + 1);
    int right_child_sum = sum_distance(sub_root->right, sub_root_depth + 1);
    return (sub_root_depth + left_child_sum + right_child_sum);
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root
 */
template <typename T> int BinaryTree<T>::sumDistances() const {
    if (root == nullptr)
        return 0;
    return sum_distance(root, 0);
}
