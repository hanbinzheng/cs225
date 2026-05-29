#include <cstdlib>
#include <cmath>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
using namespace cs225;

// debug log
#ifdef DEBUG_ON
#include <iostream>
#define LOG(...) std::cout << "Line [" << __LINE__ << "]: " << __VA_ARGS__ << std::endl
#else
#define LOG(...) do {} while (0)
#endif

// sets up the output image
PNG* setupOutput(unsigned w, unsigned h) {
    PNG* image = new PNG(w, h);
    return image;
}

// Returns my favorite color
HSLAPixel* myFavoriteColor(double saturation) {
    // new memory on the heap
    // otherwise this object will be destroyed as the lifecycle of myFavoriteColor ended
    HSLAPixel * pixel = new HSLAPixel(216, saturation, 0.5); // my favorite: 216
    return pixel;
}

void sketchify(std::string inputFile, std::string outputFile) {
    // Load in.png
    // origin: NULL, calling its member function results in segfault
    PNG* original = new PNG();

    LOG("reached");
    original->readFromFile(inputFile);
    unsigned width = original->width();
    unsigned height = original->height();
    LOG("reached");

    // Create out.png
    PNG* output = setupOutput(width, height);

    // Load our favorite color to color the outline
    HSLAPixel* myPixel = myFavoriteColor(0.5);

    // Go over the whole image, and if a pixel differs from that to its upper
    // left, color it my favorite color in the output
    for (unsigned y = 1; 0 < y && y < height; y++) {
        for (unsigned x = 1; 0 < x && x < width; x++) {
            // Calculate the pixel difference
            HSLAPixel* prev = original->getPixel(x - 1, y - 1);
            HSLAPixel* curr = original->getPixel(x, y);
            double diff = std::fabs(curr->h - prev->h);

            // If the pixel is an edge pixel,
            // color the output pixel with my favorite color
            if (diff > 20) {
                // avoid warning caused by -pedantic and -Wall -Werror
                HSLAPixel* currOutPixel = (*output).getPixel(x, y);
                *currOutPixel = *myPixel; // correct the algorithm logic
            }
        }
    }

    // Save the output file
    output->writeToFile(outputFile);

    // Clean up memory
    delete myPixel;
    delete output;
    delete original;
}
