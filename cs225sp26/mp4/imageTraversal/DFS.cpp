#include <iterator>
#include <cmath>
#include <list>
#include <stack>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "DFS.h"

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator DFS::begin() {
	/** @todo [Part 1] */
	if (stk_.empty()) {
		std::cout << "[DFS::begin()]: you are using an empty ImageTraversal::DFS!"
			<< std::endl;
		return Iterator();
	} else if (stk_.size() == 1){
		/* only contains the start */
		/* it will generally in this branch */
		return Iterator(this);
	} else { /* stk_.size() >= 2 */
		/* by default, this branch will not be called */
		std::cout << "[DFS::begin()]: the DFS traversal has been modified, "
			<< "and is not simply an intergace!" << std::endl;
		DFS tmp(png_, start_, tolerance_);
		/* this operation is valid
		 * Iterator(ImageTraversal *) will immediately clone a new object on the heap
		 * before the return (destroy of tmp), construction of Iterator has been done
		 */
		return Iterator(&tmp);
	}		
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator DFS::end() {
  	/** @todo [Part 1] */
  	return ImageTraversal::Iterator();
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void DFS::add(const Point & point) {
	/** @todo [Part 1] */
  	if (point_in_png(point, png_)) {
		stk_.push(point);
  	} else {
		std::cout << "[DFS::add()]: Stupid user, you add a point out of range! " <<
			"And fuck those who wrote this fucking stupid and vague documentation. "
			<< "Obviously that guy lacks the simplest ability to explain things clearly. "
			<< std::endl;
  	}
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point DFS::pop() {
  	/** @todo [Part 1] */
  	if (stk_.empty()) {
		std::cout << "[DFS::pop()]: Stupid user, you are trying to pop() an empty container!"
			<< "And fuck those stupid writer of documentation." << std::endl;
		return Point();
  	} else {
		Point tmp = stk_.top();
		stk_.pop();
		return tmp;
	}
}

/**
 * Returns the current Point in the traversal.
 */
Point DFS::peek() const {
	/** @todo [Part 1] */
	if (stk_.empty()) {
		std::cout << "[DFS::peek()]: Metamorphic peeping tom! "
			<< "You are peeking an empty container!" << std::endl;
		return Point();
	} else {
		return stk_.top();
	}
}

/**
 * Returns true if the traversal is empty.
 */
bool DFS::empty() const {
  	/** @todo [Part 1] */
  	return stk_.empty();
}
