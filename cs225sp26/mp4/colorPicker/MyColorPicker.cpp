#include "../cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"

#include <cstdlib> 

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
	double h, s, l, a;
	h = ((double)(rand() % 101)) / 100.0f * 360.0f;
	s = ((double)(rand() % 101)) / 100.0f;
	l = ((double)(rand() % 101)) / 100.0f;
	a = ((double)(rand() % 101)) / 100.0f;
	return HSLAPixel(h, s, l, a);
}
