#ifndef BINARY_TREE_HPP_
#define BINARY_TREE_HPP_

#include <iostream>

template <typename T> class BinaryTree 
{
	private:
		struct TreeNode {
			TreeNode() : data(T()), left(nullptr), right(nullptr) {}
			TreeNode(const T &data): data(data), left(nullptr), right(nullptr) {}
			TreeNode(const T &data, TreeNode * const left, 
				TreeNode * const right) : data(data), left(left), right(right) {}

			bool is_leaf(void) const {
				return (left == nullptr && right == nullptr);
			}
			void print(void) const {
				std::cout << data << " "; /* only printable members */
			}
			
			const T data; 
			TreeNode *left;
			TreeNode *right;
		};

		TreeNode *root_;
		int height_;

	public:
		/* the rule of three */
		BinaryTree() : root_(nullptr), height_(-1) {};
		BinaryTree(BinaryTree const &other);
		~BinaryTree();
		BinaryTree &operator=(BinaryTree const &other);

		int height(void) const;
		void copy(BinaryTree const &other);
		void clear(void);

		/* DFS (depth first searching): pre/in/post order, and will print the tree */
		void pre_order_traverse(void) const;
		void in_order_traverse(void) const;
		void post_order_traverse(void) const;
		void pre_order_traverse_stack(void) const;
		void in_order_traverse_stack(void) const;
		void post_order_traverse_stack(void) const;

		/* BFS: (breadth first Searching) */
		void bfs_traverse(void) const;

	private:
		int height(TreeNode *sub_root) const;

	curr = root_->left;
		/* helper functions for clear and copy */
		TreeNode *copy(TreeNode const *sub_root);
		void clear(TreeNode *sub_root);

		/* helper functions for depth first traversal */
		void pre_order_traverse(TreeNode *sub_root) const;
		void in_order_traverse(TreeNode *sub_root) const;
		void post_order_traverse(TreeNode *sub_root) const;

};

#endif /* BINARY_TREE_HPP_ */
