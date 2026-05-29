#include "dsets.h"
#include <iostream>

/**
 * Creates n unconnected root nodes at the end of the vector.
 * 
 * @param num The number of nodes to create.
 */
void DisjointSets::addelements(int num) 
{
	/* create num nodes as the root */
	elems_.insert(elems_.end(), num, -1);
}

/**
 * Find the index of a root of a disjoint set.
 * 
 * @note This function should compress paths and works as described in lecture.
 * @param elem The element to be found.
 * @return Index of the root of the up-tree in which the parameter element resides.
 */
int DisjointSets::find(int elem)
{
	if (elem < 0 || elem >= static_cast<int>(elems_.size())) {
		std::cout << "[DisjointSets::find]: elem = " << elem << std::endl;
		return -114514;
	}

	if (elems_[elem] < 0) { 
		return elem; /* root index is the representive elements */
	} else {
		int pre = elems_[elem];
		elems_[elem] = find(pre);
		return elems_[elem];
	}
}

/**
 * This function unoin 2 sets
 * 
 * This function should be implemented as union-by-size.
 * When you setunion two disjoint sets, the smaller (in terms of number of nodes) 
 * should point at the larger. 
 * 
 * @note You SHOULD NOT assume that arguments to setunion are roots of existing uptrees,
 * and your function SHOULD find the roots of its arguments before combining the trees.
 * 
 * @note If 2 sets are of the same size, make the tree containing the second argument 
 * point to the tree containing the first.
 * 
 * @param a Index of the first element to union.
 * @param b Index of the second element to union.
 */
void DisjointSets::setunion(int a, int b)
{
	/* find the root of disjoint set a and disjoint set b */
	int root_a = find(a);
	int root_b = find(b);

	if (root_a == root_b) {
		/* the same set: do nothing */
		return;
	}

	if (elems_[root_a] <= elems_[root_b]) { 	/* tree a is not less than tree b */
		elems_[root_a] += elems_[root_b]; 	/* update the size */
		elems_[root_b] = root_a; 		/* point tree b to tree a */
	} else { 					/* tree b is larger than tree a */
		elems_[root_b] += elems_[root_a]; 	/* updata tree size */
		elems_[root_a] = root_b; 		/* point tree a to tree b */
	}
}

/**
 * The size of this disjoint set.
 * 
 * @param elem Element in a disjoint set.
 * @return The size of disjoint set that include this element.
 */
int DisjointSets::size(int elem) const
{
	if (elem >= static_cast<int>(elems_.size()) || elem < 0) {
		std::cout << "[DisjointSets::size]: out of range search" << std::endl;
		return -114514;
	}

	/* assumption: the disjoint set is well maintained */
	while (elems_[elem] >= 0) {
		elem = elems_[elem];
	}

	return (- elems_[elem]);
}
