/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
int AVLTree<K, V>::imbalance_factor(const Node *sub_root) const
{
	if (sub_root == nullptr) { /* generally, it will not be nullptr */
		return 0;
	} else {
		return heightOrNeg1(sub_root->right) - heightOrNeg1(sub_root->left);
	}
}

template <class K, class V>
int AVLTree<K, V>::get_height(const Node *sub_root) const
{
	if (sub_root == nullptr) {
		return -1;
	} else {
		return (1 + std::max(heightOrNeg1(sub_root->left), heightOrNeg1(sub_root->right)));
	}
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
	/* RR imbalance, and remember to update height */
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)

	/* basic rotation */
	Node *y = t->right;
	t->right = y->left;
	y->left = t;

	/* update height */
	t->height = get_height(t);
	y->height = get_height(y);

	/* revise the origin node */
	t = y;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
	/* LR imbalance */
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
	/* LL imbalance, and remember to update height */
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)

	/* basic notation */
	Node *y = t->left;
	t->left = y->right;
	y->right = t;

	/* update height */
	t->height = get_height(t);
	y->height = get_height(y);

	/* revise the origin node */
	t = y;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
	/* RL imbalance */
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
	rotateRight(t->right);
	rotateLeft(t);
}

/* first, this function description is fucking stupid vague */
/* for example, if the left-sub_tree is a tree with one node */
/* and the right sub_tree is a perfect tree with height 100 */
/* there is no way to balance this tree with simply rotation */
/* rotation only works when we simply change one node in a balanced avl tree */
template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
	/* Assume that node->left and node->right are both balanced. 
	 * Even if no rotations are required, you should update the node's height. 
	 */
	if (subtree == nullptr) { /* safety check */
		std::cout << "fuck stupid user, the pointer passed to rebalance() is nullptr ! and &ptr is: "  << &subtree << std::endl;
		return;
	}
	int balance = imbalance_factor(subtree);

	if (balance == -2) { /* LL or LR */
		/* guaranteed that sub_tree->left != nullptr */
		if (imbalance_factor(subtree->left) == -1) {
			/* LL case */	
			rotateRight(subtree);
		} else {
			rotateLeftRight(subtree);
		}
	} else if (balance == 2) { /* RR or RL */
		/* guarantee that subtree->right != nullptr */
		if (imbalance_factor(subtree->right) == 1) {
			/* RR case */
			rotateLeft(subtree);
		} else {
			rotateRightLeft(subtree);
		}
	} 

	/* update height anyway */
	subtree->height = get_height(subtree);
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
	if (subtree == nullptr) {
		/* reach base case, create new node */
		subtree = new Node(key, value);
	} else if (subtree->key < key) {
		/* insert to right subtree and rebalance */
		insert(subtree->right, key, value);
		rebalance(subtree->right);
	} else if (key < subtree->key) {
		/* insert to left subtree and rebalance */
		insert(subtree->left, key, value);
		rebalance(subtree->left);
	} else {
		/* key == subtree->key: override */
		subtree->key = key;
		subtree->value = value;
	}
	rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
typename AVLTree<K, V>::Node *AVLTree<K, V>::get_max(Node *sub_root) const
{
	if (sub_root == nullptr) {
		return sub_root;
	}
	while (sub_root != nullptr && sub_root->right != nullptr) {
		sub_root = sub_root->right;
	}
	return sub_root;
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
		remove(subtree->left, key);
		if (subtree->left != nullptr) { /* for safety check */
			rebalance(subtree->left);
		}
		rebalance(subtree);
    } else if (key > subtree->key) {
		remove(subtree->right, key);
		if (subtree->right != nullptr) { /* for safety check */
			rebalance(subtree->right);
		}
		rebalance(subtree);
    } else { /* key == subtree->key */
        if (subtree->left == NULL && subtree->right == NULL) {
			/* if single node, delete directly */
			/* else the rebalance will be done in previous parts */
			delete subtree;
			/* remember we passed in Node *&, and remember to set ptr nullptr */
			subtree = nullptr; 
        } else if (subtree->left != NULL && subtree->right != NULL) {
			Node *iop = get_max(subtree->left);
			swap(iop, subtree);
			remove(subtree->left, key);
			if (subtree->left != nullptr) {
				rebalance(subtree->left);
			}
			rebalance(subtree);
        } else {
			Node *tmp = subtree;
			if (subtree->left == nullptr) {
				/* right child case */
				subtree = subtree->right;
			} else if (subtree->right == nullptr) { 
				/* left child case */
				subtree = subtree->left;
			}
			
			delete tmp;
			if (subtree != nullptr) {
				rebalance(subtree);
			}
        }
    }

	if (subtree != nullptr) {
		rebalance(subtree);
	}
}
