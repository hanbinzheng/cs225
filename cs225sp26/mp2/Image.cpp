#include "Image.h"
#include "cs225/HSLAPixel.h"

static void limit(double &value, double const min, double const max) {
        value = (value > max) ? max : value;
        value = (value < min) ? min : value;
}

void Image::change_luminance_(double amount) {
        cs225::HSLAPixel *tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        tmp = this->getPixel(x, y);
                        tmp->l = tmp->l + amount;
                        limit(tmp->l, 0, 1);
                }
        }
}

void Image::change_saturation_(double amount) {
        cs225::HSLAPixel *tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        tmp = this->getPixel(x, y);
                        tmp->s = tmp->s + amount;
                        limit(tmp->s, 0, 1);
                }
        }
}

Image::Image(unsigned int width, unsigned int height)
    : cs225::PNG(width, height) {
        /* nothing */
}

Image::Image(PNG const &png) : PNG(png) { /* nothing */ }

void Image::lighten() { this->change_luminance_(0.1); }

void Image::lighten(double amount) { this->change_luminance_(amount); }

void Image::darken() { this->change_luminance_(-0.1); }

void Image::darken(double amount) { this->change_luminance_(-amount); }

void Image::saturate() { this->change_saturation_(0.1); }

void Image::saturate(double amount) { this->change_saturation_(amount); }

void Image::desaturate() { this->change_saturation_(-0.1); }

void Image::desaturate(double amount) { this->change_saturation_(-amount); }

void Image::grayscale() {
        /* gray scale means change saturation to 0 */
        cs225::HSLAPixel *tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        tmp = this->getPixel(x, y);
                        tmp->s = 0;
                }
        }
}

void Image::rotateColor(double degrees) {
        cs225::HSLAPixel *tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        tmp = this->getPixel(x, y);
                        tmp->h = tmp->h + degrees;

                        while (tmp->h < 0 || tmp->h > 360) {
                                if (tmp->h > 360)
                                        tmp->h = tmp->h - 360;
                                else if (tmp->h < 0)
                                        tmp->h = tmp->h + 360;
                        }
                }
        }
}

void Image::illinify() {
        cs225::HSLAPixel *tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        tmp = this->getPixel(x, y);
                        if (tmp->h < 113.5 || tmp->h > 293.5) {
                                /* closer to orange than blue */
                                tmp->h = 11; /* illinois orange */
                        } else {
                                /* closer to 216 */
                                tmp->h = 216; /* illinois blue */
                        }
                }
        }
}

void Image::scale(double factor) {
        Image img_tmp(*this);
        unsigned int new_w = (unsigned int)(this->width() * factor);
        unsigned int new_h = (unsigned int)(this->height() * factor);
        this->resize(new_w, new_h);

        cs225::HSLAPixel *p_tmp;
        for (unsigned int y = 0; y < this->height(); y++) {
                for (unsigned int x = 0; x < this->width(); x++) {
                        unsigned int old_x = (unsigned int)(x / factor);
                        unsigned int old_y = (unsigned int)(y / factor);
                        if (old_x >= img_tmp.width())
                                old_x = img_tmp.width() - 1;
                        if (old_y >= img_tmp.height())
                                old_y = img_tmp.height() - 1;

                        p_tmp = this->getPixel(x, y);
                        *p_tmp = *(img_tmp.getPixel(old_x, old_y));
                }
        }
}

void Image::scale(unsigned w, unsigned h) {
        Image img_tmp(*this);
        double factor_w = (double)w / (double)img_tmp.width();
        double factor_h = (double)h / (double)img_tmp.height();
        double factor = (factor_w > factor_h) ? factor_h : factor_w;
        this->scale(factor);
}
