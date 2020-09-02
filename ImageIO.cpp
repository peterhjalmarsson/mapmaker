/* 
 * File:   Output.cpp
 * Author: Peter Hjalmarsson 
 * 
 * Created on August 29, 2015, 7:08 PM
 */
#include "ImageIO.h"

ImageIO::ImageIO() {
}

ImageIO::~ImageIO() {
}

void ImageIO::writeGrayscalePng8Bit(Map *map, std::string file) {
    std::cout << "Writing to grayscale image: " << file << std::endl;
    png::image< png::gray_pixel > image(map->getWidth(), map->getHeight());
    for (int x = 0; x < map->getWidth(); x++) {
        for (int y = 0; y < map->getHeight(); y++) {
            image.set_pixel(x, y, map->getColor(x, y, 8));
        }
    }
    image.write(file);
}

void ImageIO::writeGrayscalePng16Bit(Map* map, std::string file) {
std::cout << "Writing to grayscale image: " << file << std::endl;
    png::image< png::gray_pixel_16 > image(map->getWidth(), map->getHeight());
    for (int x = 0; x < map->getWidth(); x++) {
        for (int y = 0; y < map->getHeight(); y++) {
            image.set_pixel(x, y, map->getColor(x, y, 16));
        }
    }
    image.write(file);
}
