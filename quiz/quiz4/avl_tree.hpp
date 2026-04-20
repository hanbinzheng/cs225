#ifndef AVL_TREE_HPP_
#define AVL_TREE_HPP_

#include <algorithm>

template <class T>
class AVLTree {
private:
    struct treeNode {
        T key;
        int height;
        treeNode *left;
        treeNode *right;

        treeNode(const T & x, int h, treeNode * l, treeNode * r)
            : key(x), height(h), left(l), right(r) {}
    };

    treeNode *root;

    // 安全获取高度，避免 Segfault
    int height(treeNode *t) const {
        return (t == nullptr) ? -1 : t->height;
    }

    // 计算平衡因子：b = height(right) - height(left)
    int getBalance(treeNode *t) const {
        if (t == nullptr) return 0;
        return height(t->right) - height(t->left);
    }

public:
    void insert(const T & x) {
        insert_(x, root);
    }

private:
    /**
     * @brief 核心插入逻辑
     * 对应讲义中的 4 个步骤
     */
    void insert_(const T & x, treeNode * & t) {
        // 1. Base Case: Insert at proper place
        if (t == nullptr) {
            t = new treeNode(x, 0, nullptr, nullptr);
            return;
        }

        // 2. Recursive Insertion
        if (x < t->key) {
            insert_(x, t->left);
            
            // 3. Check imbalance and Rotate (Left side)
            if (getBalance(t) == -2) { /* 原来 balance 右 - 左 [-1, 1], 只可能变 -2 */
                // 判断是 Single Right (LL) 还是 Left-Right (LR)
                if (getBalance(t->left) == -1) {
                    rotateRight(t);
                } else {
                    rotateLeftRight(t);
                }
            }
        } else if (x > t->key) {
            insert_(x, t->right);
            
            // 3. Check imbalance and Rotate (Right side)
            if (getBalance(t) == 2) {
                // 判断是 Single Left (RR) 还是 Right-Left (RL)
                if (getBalance(t->right) == 1) {
                    rotateLeft(t);
                } else {
                    rotateRightLeft(t);
                }
            }
        }

        // 4. Update height (Recursively after rotation)
        t->height = 1 + std::max(height(t->left), height(t->right));
    }

    // --- 抽象出来的旋转函数 ---

    void rotateLeft(treeNode * & t) {
        // TODO: 实现 RR 型旋转

		treeNode *y = t->right;
		t->right = y->left;
		y->left = t;

		t->height = 1 + std::max(height(t->left), height(t->right));
		y->height = 1 + std::max(height(y->left), height(y->right));

		t = y;
    }

    void rotateRight(treeNode * & t) {
        // TODO: 实现 LL 型旋转

		/* a bad implementation */
		// treeNode *node = t->left->right;
		// t->left->right = t;
		// t = t->left;
		// t->right->left = node;

		treeNode *y = t->left;
		t->left = y->right;
		y->right = t;

		t->height = 1 + std::max(height(t->left), height(t->right));
		y->height = 1 + std::max(height(y->left), height(y->right));

		t = y;
    }

    void rotateLeftRight(treeNode * & t) {
        // TODO: 先 rotateLeft(t->left)，再 rotateRight(t)
		rotateLeft(t->left);
		rotateRight(t);
    }

    void rotateRightLeft(treeNode * & t) {
        // TODO: 先 rotateRight(t->right)，再 rotateLeft(t)
		rotateRight(t->right);
		rotateLeft(t);
    }
};

#endif