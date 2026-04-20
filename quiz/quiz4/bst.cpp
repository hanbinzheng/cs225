#include "bst.hpp"

template <typename K, typename V>
void BinarySearchingTree<K, V>::clear(TreeNode *sub_root)
{
	if (sub_root != nullptr) {
		clear(sub_root->left);
		clear(sub_root->right);
		delete sub_root;
	}
}

template <typename K, typename V>
void BinarySearchingTree<K, V>::clear(void)
{
	clear(root_);
}

template <typename K, typename V> 
typename BinarySearchingTree<K, V>::TreeNode *BinarySearchingTree<K, V>::copy(TreeNode *sub_root)
{
	if (sub_root == nullptr) {
		return nullptr;
	}
	TreeNode *tmp = new TreeNode(*sub_root);
	tmp->left = copy(sub_root->left);
	tmp->right = copy(sub_root->right);
	return tmp;
}

template <typename K, typename V>
void BinarySearchingTree<K, V>::copy(BinarySearchingTree const &other)
{
	root_ = copy(other.root_);
}

template <typename K, typename V>
BinarySearchingTree<K, V>::BinarySearchingTree(BinarySearchingTree const &other)
{
	copy(other);
}

template <typename K, typename V>
typename BinarySearchingTree<K, V>::BinarySearchingTree &
BinarySearchingTree<K, V>::operator=(BinarySearchingTree const &other)
{
	if (&other != this) {
		clear();
		copy(other);
	} 
	return *this;
}

template <typename K, typename V>
BinarySearchingTree<K, V>::~BinarySearchingTree()
{
	clear();
}

template <typename K, typename V>
typename BinarySearchingTree<K, V>::TreeNode *&
BinarySearchingTree<K, V>::find(TreeNode *&sub_root, K const &key) const
{
	if (sub_root == nullptr || sub_root->key == key) {
		sub_root;
	}

	if (key < sub_root->key) {
		return find(sub_root->left, key);
	} else {
		return find(sub_root->right, key);
	}
}

template <typename K, typename V>
V BinarySearchingTree<K, V>::find(K const &key) const
{
	TreeNode *node = find(root_, key);
	if (node == nullptr) {
		return V();
	} else {
		return node->value;
	}
}

template <typename K, typename V>
void BinarySearchingTree<K, V>::insert(K const &key, V const &value)
{
	TreeNode *node = find(root_, key);
	if (node != nullptr) {
		node->value = value; /* update value */
	} else {
		node = new TreeNode(key, value);
	}
}

template<typename K, typename V> typename BinarySearchingTree<K, V>::TreeNode *&
BinarySearchingTree<K, V>::find_iop(TreeNode *&left_child) const
{
	if (left_child->right == nullptr) {
		return left_child;
	} else {
		return find_iop(left_child->right);
	}
}

template <typename K, typename V>
V BinarySearchingTree<K, V>::remove(K const &key)
{
	TreeNode *&to_remove = find(root_, key);
	if (to_remove == nullptr) {
		return V();
	}
	V return_value = to_remove->value;
	TreeNode *old_node = to_remove;

	if (to_remove->left == nullptr) {
		to_remove = to_remove->rigth;
	} else if (to_remove->right == nullptr) {
		to_remove = to_remove->left;
	} else { /* cannot remove directly */
		TreeNode *&iop = find_iop(to_remove->left);
		TreeNode *iop_node = iop;
		iop = iop->left;
		iop_node->left = to_remove->left;
		iop_node->right = to_remove->right;
		to_remove = iop_node;
	}
	delete old_node;
	return return_value;
}