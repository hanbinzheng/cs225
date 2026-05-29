#include "mp1.h"
#include "cs225/PNG.h"
#include <iostream>

void rotate(std::string inputFile, std::string outputFile) {
    cs225::PNG input_img;

    // read the image and check whether valid
    if (! input_img.readFromFile(inputFile)) {
        std::cout << "failed to read from the file " << inputFile << std::endl;
        return;
    }

    // create output image, get height and width
    cs225::PNG output_img = cs225::PNG(input_img);
    unsigned int height = input_img.height();
    unsigned int width = input_img.width();

    // rotate the image
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            *(output_img.getPixel(x, y)) =
                *(input_img.getPixel(width - x - 1, height - y - 1));
        }
    }

    // write to the output
    if (! output_img.writeToFile(outputFile)) {
        std::cout << "failed to write to the file " << outputFile << std::endl;
        return;
    }
}
