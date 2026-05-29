#ifndef TOOLS_H_
#define TOOLS_H_

#include "point.h"
#include "cs225/HSLAPixel.h"

static inline Point<3> convertToLAB(cs225::HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
}

/* define new hash funcion */
struct PointHash {
    std::size_t operator()(const Point<3>& p) const {
	std::size_t seed = 0;
	for (int i = 0; i < 3; ++i) {
		/* magic number: engineering practice? */
            	seed ^= std::hash<double>{}(p[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

#endif /* TOOLS_H_ */