#include "binary_tree.hpp"
#include <algorithm>
#include <stack>
#include <queue>

template <typename T>
void BinaryTree<T>::clear(TreeNode *sub_root)
{
	if (sub_root != nullptr) {
		clear(sub_root->left);
		clear(sub_root->right);
		delete sub_root;
	}
}

template <typename T>
void BinaryTree<T>::clear(void)
{
	clear(root_);
	root_ = nullptr;
	height_ = -1;
}

template <typename T>
BinaryTree<T>::TreeNode *BinaryTree<T>::copy(TreeNode const *sub_root) 
{
	if (sub_root == nullptr) {
		return nullptr;
	}

	TreeNode *tmp = new TreeNode(sub_root->data);
	tmp->left = copy(sub_root->left);
	tmp->right = copy(sub_root->right);
	return tmp;
}

template <typename T>
void BinaryTree<T>::copy(BinaryTree const &other) 
{
	root_ = copy(other.root_);
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree const &other) 
{
	copy(other);
}

template <typename T>
BinaryTree<T> & BinaryTree<T>::operator=(BinaryTree const &other)
{
	if (&other != this) {
		clear();
		copy(other);
	}
	return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
    clear();
}

template <typename T>
int BinaryTree<T>::height(TreeNode *sub_root) const
{
	if (sub_root == nullptr) {
		return -1;
	} else {
		return 1 + std::max(height(sub_root->tmpleft), height(sub_root->right));
	}
}

template <typename T>
int BinaryTree<T>::height(void) const 
{
	return height(root_);
}

template <typename T>
void BinaryTree<T>::pre_order_traverse(TreeNode *sub_root) const
{
	if (sub_root != nullptr) {
		sub_root->print();
		pre_order_traverse(sub_root->left);
		pre_order_traverse(sub_root->right);
	}
}

template <typename T>
void BinaryTree<T>::pre_order_traverse(void) const 
{
	pre_order_traverse(root_);
}

template <typename T>
void BinaryTree<T>::in_order_traverse(TreeNode *sub_root) const
{
	if (sub_root != nullptr) {
		in_order_traverse(sub_root->left);
		sub_root->print();
		in_order_traverse(sub_root->right);
	}
}

template <typename T>
void BinaryTree<T>::in_order_traverse(void) const{
	in_order_traverse(root_);
}

template <typename T>
void BinaryTree<T>::post_order_traverse(TreeNode *sub_root) const
{
	if (sub_root != nullptr) {
		post_order_traverse(sub_root->left);
		post_order_traverse(sub_root->right);
		sub_root->print();
	}
}

template <typename T>
void BinaryTree<T>::post_order_traverse(void) const
{
	post_order_traverse(root_);
}

template <typename T>
void BinaryTree<T>::pre_order_traverse_stack(void) const
{
	if (root_ == nullptr)	return;

	std::stack<TreeNode *> stk;
	stk.push(root_);

	while (! stk.empty()) {
		TreeNode *tmp = stk.top();
		stk.pop();

		/* simply for safety, and will not be triggered */
		if (tmp == nullptr)	continue; 
		tmp->print();

		if (tmp->right != nullptr) {
			stk.push(tmp->right);
		}
		if (tmp->left != nullptr) {
			stk.push(tmp->left);
		}
	}
}

template <typename T>
void BinaryTree<T>::in_order_traverse_stack(void) const
{
	if (root_ == nullptr)	return;

	std::stack<TreeNode *> stk;
	stk.push(root_);
	TreeNode *curr = root_->left;

	while (! stk.empty()) {
		/* push all left part to the stack */
		while (curr != nullptr) {
			stk.push(curr)	;
			curr = curr->left;
		}

		TreeNode *tmp = stk.top();
		stk.pop();
		/* only for safety and will not be triggered */
		if (tmp == nullptr)	continue;
		tmp->print();

		if (tmp->right != nullptr) {
			stk.push(tmp->right);
			curr = tmp->right;
		}
	}
}

template <typename T>
void BinaryTree<T>::post_order_traverse_stack(void) const
{
	if (root_ == nullptr)	return;
	
	/* firstly: root, right, left, 
	 * the in reverse by stack to achieve 
	 * left, right, root
	 */
	std::stack<TreeNode *> stk1, stk2;
	stk1.push(root_);
	TreeNode *curr = root_;

	/* first push left, then push right */
	while (! stk1.empty()) {
		TreeNode *tmp = stk1.top();
		stk1.pop();

		if (tmp == nullptr)	continue;

		stk2.push(curr);
		if (tmp->left != nullptr) {
			stk1.push(tmp->left);
		}
		if (tmp->right != nullptr) {
			stk1.push(tmp->right);
		}
	}

	while (!stk2.empty()) {
        stk2.top()->print();
        stk2.pop();
    }
}

template <typename T>
void BinaryTree<T>::bfs_traverse(void) const
{
	if (root_ == nullptr) 	return;

	std::queue<TreeNode *> que;
	que.push(root_);

	while (! que.empty()) {
		TreeNode *tmp = que.front()	;
		que.pop();

		/* for safety and this will not be triggered */
		if (tmp == nullptr) continue;
		tmp->print();
		if (tmp->left != nullptr) {
			que.push(tmp->left);
		}
		if (tmp->right != nullptr) {
			que.push(tmp->right);
		}
	}
}