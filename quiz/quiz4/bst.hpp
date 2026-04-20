#ifndef BST_HPP_
#define BST_HPP_

#include <iostream>

template <typename K, typename V>
class BinarySearchingTree{
	private:
		struct TreeNode {
			const K key;	
			V value;
			TreeNode *left;
			TreeNode *right;

			TreeNode() : key(K()), value(V()), left(nullptr), right(nullptr) {}
			TreeNode(K const &key, V const &value) : key(key), value(value), left(nullptr), right(nullptr) {}
			TreeNode(K const &key, V const &value, TreeNode *left, TreeNode *right):
			 key(key), value(value), left(left), right(right) {}

			void print(void) const {
				std::cout << "( " << key << ", " << value << " ) ";
			}
			bool is_leaf(void) const {
				return (left == nullptr && right == nullptr);
			}
		};

		TreeNode *root_;

	public:
		BinarySearchingTree(void) : root_(nullptr) {}
		BinarySearchingTree(BinarySearchingTree const &other);
		~BinarySearchingTree();
		BinarySearchingTree &operator=(BinarySearchingTree const &other);
		void clear(void);
		void copy(BinarySearchingTree const &other);
		void insert(K const &key, V const &value);
		V remove(K const &key);
		V find(K const &key) const;

	private:
		void clear(TreeNode *sub_root);
		TreeNode *copy(TreeNode *sub_root);
		TreeNode *&find(TreeNode *&sub_root, K const &key) const;
		/* for root_, input root_->left, get iop, left_child is not nullptr */
		TreeNode *&find_iop(TreeNode *&left_child) const;
};

#endif /* BST_HPP_ */