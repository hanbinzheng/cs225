#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "cs225/PNG.h"

class Image : public cs225::PNG {
      public:
        Image() = default;
        Image(Image const &other) = default;
        Image(unsigned int width, unsigned int height);
        Image(PNG const &png);
        ~Image() = default;

        /* Increase the luminance of every pixel by 0.1. */
        void lighten();
        void lighten(double amount);

        /* Decrease the luminance of every pixel by 0.1. */
        void darken();
        void darken(double amount);

        /* increase the saturation of every pixel by 0.1. */
        void saturate();
        void saturate(double amount);

        /* decrease the saturation of every pixel by 0.1. */
        void desaturate();
        void desaturate(double amount);

        void grayscale();
        void rotateColor(double degrees);
        void illinify();
        void scale(double factor);
        void scale(unsigned w, unsigned h);

      private:
        /* change the luminance of all pixel by amount */
        void change_luminance_(double amount);
        /* change the saturation of all pixel by amount */
        void change_saturation_(double amount);
};

#endif /* _IMAGE_H_*/
