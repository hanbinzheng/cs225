#include "StickerSheet.h"
#include "Image.h"
#include "cs225/HSLAPixel.h"
#include <cstddef>

StickerSheet::StickerSheet(const Image &picture, unsigned max)
    : base_(picture), max_(max) {}

void StickerSheet::CopyStickers(const StickerSheet &other) {
        x_cords_ = other.x_cords_;
        y_cords_ = other.y_cords_;

        stickers_.clear();
        stickers_.resize(other.stickers_.size());
        for (size_t i = 0; i < other.stickers_.size(); ++i) {
                if (other.stickers_[i] != nullptr)
                        stickers_[i].reset(new Image(*other.stickers_[i]));
        }
}

StickerSheet::StickerSheet(const StickerSheet &other)
    : base_(other.base_), max_(other.max_) {
        CopyStickers(other);
}

const StickerSheet &StickerSheet::operator=(const StickerSheet &other) {
        if (this == &other)
                return *this;

        base_ = Image(other.base_);
        max_ = other.max_;
        CopyStickers(other);
        return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
        max_ = max;
        stickers_.resize(max);
        x_cords_.resize(max);
        y_cords_.resize(max);
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
        /* iterate all stickers */
        for (size_t i = 0; i < stickers_.size(); ++i) {
                if (stickers_[i] != nullptr)
                        continue;

                stickers_[i].reset(new Image(sticker));
                x_cords_[i] = x;
                y_cords_[i] = y;
                return i;
        }

        /* if current vector is not full, add to the back */
        if (max_ > stickers_.size()) {
                unsigned idx = stickers_.size();
                stickers_.emplace_back(new Image(sticker));
                x_cords_.emplace_back(x);
                y_cords_.emplace_back(y);
                return idx;
        }

        return -1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
        if (index >= max_ || stickers_[index] == nullptr)
                return false;

        x_cords_[index] = x;
        y_cords_[index] = y;
        return true;
}

void StickerSheet::removeSticker(unsigned index) {
        if (index >= max_ || stickers_[index] == nullptr)
                return;

        stickers_[index].reset();

        /* ATTENTION: this may render bugs */
        x_cords_[index] = 0;
        y_cords_[index] = 0;
}

Image *StickerSheet::getSticker(unsigned index) const {
        if (index >= max_ || index >= stickers_.size())
                return nullptr;

        return stickers_[index].get();
}

Image StickerSheet::render() const {
        Image img(base_);

        for (size_t i = 0; i < stickers_.size(); ++i) {
                Image *tmp = stickers_[i].get();
                if (tmp == nullptr)
                        continue;

                /* extend the canvas if: tmp->w + x_[i] - 1 > img.w - 1 */
                unsigned start_x = x_cords_[i];
                unsigned start_y = y_cords_[i];
                int dx = tmp->width() + start_x - img.width();
                int dy = tmp->height() + start_y - img.height();
                if (dx > 0 && dy > 0) {
                        img.resize(img.width() + dx, img.height() + dy);
                } else if (dx > 0 && dy <= 0) {
                        img.resize(img.width() + dx, img.height());
                } else if (dy > 0 && dx <= 0) {
                        img.resize(img.width(), img.height() + dy);
                }

                /* render a layer of sticker */
                for (unsigned y = 0; y < tmp->height(); y++) {
                        for (unsigned x = 0; x < tmp->width(); x++) {
                                cs225::HSLAPixel *p_img =
                                    img.getPixel(start_x + x, start_y + y);

                                if (tmp->getPixel(x, y)->a != 0)
                                        *p_img = *tmp->getPixel(x, y);
                        }
                }
        }

        return img;
}
