#include "StickerSheet.h"

static void print_size(Image const &img) {
        std::cout << img.width() << " x " << img.height() << std::endl;
}

int main() {
        Image alma;
        alma.readFromFile("alma.png");
        print_size(alma);

        Image sticker_1, sticker_2, sticker_3, sticker_4;
        Image sticker_5, sticker_6, sticker_7, sticker_8;

        sticker_1.readFromFile("images/img_1.png"); /* cat */
        print_size(sticker_1);
        sticker_2.readFromFile("images/img_2.png"); /* nz */
        print_size(sticker_2);
        sticker_3.readFromFile("images/img_3.png"); /* cat */
        print_size(sticker_3);
        sticker_4.readFromFile("images/img_4.png"); /* nz */
        print_size(sticker_4);
        sticker_5.readFromFile("images/img_5.png"); /* cat */
        print_size(sticker_5);
        sticker_6.readFromFile("images/img_6.png"); /* nz */
        print_size(sticker_6);
        sticker_7.readFromFile("images/img_7.png"); /* cat */
        print_size(sticker_7);
        sticker_8.readFromFile("images/img_8.png"); /* go work */
        print_size(sticker_8);

        sticker_1.scale(180, 180);
        print_size(sticker_1);
        sticker_2.scale(180, 180);
        print_size(sticker_2);
        sticker_3.scale(180, 180);
        print_size(sticker_3);
        sticker_4.scale(90, 90);
        print_size(sticker_4);
        sticker_5.scale(180, 180);
        print_size(sticker_5);
        sticker_6.scale(90, 90);
        print_size(sticker_6);
        sticker_7.scale(90, 90);
        print_size(sticker_7);
        sticker_8.scale(200, 200);
        print_size(sticker_8);

        StickerSheet sheet(alma, 2);
        // sheet.addSticker(sticker_1, 20, 20);
        sheet.addSticker(sticker_2, 60, 60);
        // sheet.addSticker(sticker_3, 100, 100);

        sheet.changeMaxStickers(9);

        sheet.addSticker(sticker_4, 250, 90);
        // sheet.addSticker(sticker_5, 200, 200);

        sheet.removeSticker(0);
        std::cout << sheet.addSticker(sticker_6, 550, 90) << std::endl;
        std::cout << sheet.addSticker(sticker_7, 400, 30) << std::endl;
        std::cout << sheet.addSticker(sticker_8, 700, 409) << std::endl;

        Image sticker_down;
        sticker_down.readFromFile("images/down_2.png");
        std::cout << sticker_down.width() << std::endl
                  << sticker_down.height() << std::endl;

        Image result = sheet.render();
        result.writeToFile("myImage.png");

        return 0;
}
