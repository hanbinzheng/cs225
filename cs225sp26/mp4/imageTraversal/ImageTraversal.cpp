#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 * 
 * @param p1 First pixel
 * @param p2 Second pixel
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
	double h = fabs(p1.h - p2.h);
	double s = p1.s - p2.s;
	double l = p1.l - p2.l;
	
	/* Handle the case where we found the bigger angle between two hues: */ 
	if (h > 180) { h = 360 - h; }
	h /= 360;
	
	return sqrt( (h*h) + (s*s) + (l*l) );    
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
	/** @todo [Part 1] */
	traversal_.reset(nullptr);
	curr_ = Point();
	visited_ = std::vector<std::vector<bool>>();
}

/* custom constructor */
ImageTraversal::Iterator::Iterator(ImageTraversal const *traversal)
{
	if (traversal != nullptr) {
		/* it is assumed that traversal only contains the start point */
		traversal_.reset(traversal->clone());
		curr_ = traversal_->peek();
		visited_.assign(traversal_->png_w(), std::vector<bool>(traversal_->png_h(), false));
		visited_[curr_.x][curr_.y] = true;
	}
}

ImageTraversal::Iterator::Iterator(Iterator const &other)
{
	traversal_.reset(other.traversal_->clone());
}

void ImageTraversal::Iterator::SetToEnd(void) 
{
	curr_ = Point();
	visited_ = std::vector<std::vector<bool>>();
	traversal_.reset(nullptr);
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  	/** @todo [Part 1] */

	/* add the neighbors to the containers */
	/* noted that, add to container doesn' t mean it has been visited */
	/* it means this point is to be visited */
	if (curr_.x < traversal_->png_w() - 1) {
		traversal_->add(Point(curr_.x + 1, curr_.y));
	}
	if (curr_.y < traversal_->png_h() - 1) {
		traversal_->add(Point(curr_.x, curr_.y + 1));
	}
	if (curr_.x >= 1) {
		traversal_->add(Point(curr_.x - 1, curr_.y));
	}
	if (curr_.y >= 1) {
		traversal_->add(Point(curr_.x, curr_.y - 1));
	}

	/* visit the next point */
	while(! traversal_->empty()) {
		Point tmp = traversal_->peek();

		/* if visited or out of tolerance, skip this node */
		if (visited_[tmp.x][tmp.y] == true || traversal_->in_tolerance(tmp) == false) {
			traversal_->pop();
			continue;
		}

		/* this point will be visited */
		curr_ = traversal_->pop();
		visited_[curr_.x][curr_.y] = true;
		return *this;
	}

	/* meet the end, and this iterator is the end */
	SetToEnd();
	return *this;
}

/**
 * Iterator accessor opreator.
 * 
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  	/** @todo [Part 1] */
	return curr_;
}

bool ImageTraversal::Iterator::operator==(const ImageTraversal::Iterator &other) const
{
	return (
		(other.curr_ == curr_) && 
		(other.visited_ == visited_) && (other.traversal_ == traversal_)
	);
}

/**
 * Iterator inequality operator.
 * 
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) const
{
	/** @todo [Part 1] */
	return !(operator==(other));
}
