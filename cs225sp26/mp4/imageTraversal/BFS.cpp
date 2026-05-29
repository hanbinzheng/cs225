
#include <iterator>
#include <cmath>
#include <list>
#include <queue>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "BFS.h"

using namespace cs225;

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator BFS::begin() {
	/** @todo [Part 1] */
	if (q_.empty()) {
		std::cout << "[BFS::begin()]: you are using an empty ImageTraversal::BFS!"
			<< std::endl;
		return Iterator();
	} else if (q_.size() == 1){
		/* only contains the start */
		/* it will generally in this branch */
		return Iterator(this);
	} else { /* q_.size() >= 2 */
		/* by default, this branch will not be called */
		std::cout << "[BFS::begin()]: the BFS traversal has been modified, "
			<< "and is not simply an intergace!" << std::endl;
		BFS tmp(png_, start_, tolerance_);
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
ImageTraversal::Iterator BFS::end() {
  	/** @todo [Part 1] */
  	return ImageTraversal::Iterator();
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void BFS::add(const Point & point) {
  	/** @todo [Part 1] */
	if (point_in_png(point, png_)) {
		q_.push(point);
	} else {
		std::cout << "[BFS::add()]: Stupid user !" <<
			" You are trying to add a point out of range !" << std::endl;
	}
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point BFS::pop() {
  	/** @todo [Part 1] */
	if (q_.empty()) {
		std::cout << "[BFS::pop()]: Stypid user and documentation writer !"
			<< "You are trying to pop() an empty container!" << std::endl;
		return Point();
	} else {
		Point tmp = q_.front();
		q_.pop();
		return tmp;
	}
}

/**
 * Returns the current Point in the traversal.
 */
Point BFS::peek() const {
  	/** @todo [Part 1] */
	if (q_.empty()) {
		std::cout << "[BFS::peek()]: Metamorphic peeping tom! "
			<< "You are peeking an empty container!" << std::endl;
		return Point();
	} else {
		return q_.front();
	}	
}

/**
 * Returns true if the traversal is empty.
 */
bool BFS::empty() const {
	/** @todo [Part 1] */
	return q_.empty();
}
