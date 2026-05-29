#ifndef HSLAPIXEL_H_
#define HSLAPIXEL_H_

namespace cs225{ // namespace begins

class HSLAPixel{

public:
    // constructors
    HSLAPixel();
    HSLAPixel(double hue, double saturation, double luminance);
    HSLAPixel(double hue, double saturation, double luminance, double alpha);

    // attributes
    double h; // hue, [0, 360]
    double s; // saturation, [0, 1]
    double l; // luminance, [0, 1]
    double a; // alpha, [0, 1]

    }; // HSLAPixel class ends
} // namespace ends

#endif // HSLAPIXEL_H_
