#ifndef _STICKERSHEET_H_
#define _STICKERSHEET_H_

#include "Image.h"
#include <memory>
#include <vector>

class StickerSheet {
      public:
        StickerSheet(const Image &picture, unsigned max);
        StickerSheet(const StickerSheet &other);
        ~StickerSheet() = default;
        const StickerSheet &operator=(const StickerSheet &other);
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, unsigned x, unsigned y);
        bool translate(unsigned index, unsigned x, unsigned y);
        void removeSticker(unsigned index);
        Image *getSticker(unsigned index) const;
        Image render() const;

      private:
        Image base_;
        unsigned max_;
        std::vector<std::unique_ptr<Image>> stickers_;
        std::vector<unsigned> x_cords_;
        std::vector<unsigned> y_cords_;
        void CopyStickers(const StickerSheet &other);
};

#endif // _STICKERSHEET_H_
