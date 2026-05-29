#include "HSLAPixel.h"

static inline double clamp_value(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

namespace cs225{ // namespace begins

    // default constructor: completely opaque and white
    HSLAPixel::HSLAPixel()
        : h(0.0), s(0.0), l(1.0), a(1.0) {
        // l = 1, a = 1, hue and s are magic
    }

    HSLAPixel::HSLAPixel(double hue, double saturation, double luminance) {
        this->h = clamp_value(hue, 0.0, 360.0);
        this->s = clamp_value(saturation, 0.0, 1.0);
        this->l = clamp_value(luminance, 0.0, 1.0);
        this->a = 1; // opaque
    }

    HSLAPixel::HSLAPixel(double hue,
                         double saturation, double luminance, double alpha) {
        this->h = clamp_value(hue, 0.0, 360.0);
        this->s = clamp_value(saturation, 0.0, 1.0);
        this->l = clamp_value(luminance, 0.0, 1.0);
        this->a = clamp_value(alpha, 0.0, 1.0);
    }

} // namespace ends
