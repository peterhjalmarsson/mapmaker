/* 
 * File:   Map.cpp
 * Author: Peter Hjalmarsson 
 * 
 * Created on September 3, 2015, 9:34 PM
 */

#include "Map.h"

Map::Map(std::string name, int width, int height)
: name_(name), width_(width), height_(height) {
}

Map::~Map() {
}

int Map::getWidth() {
    return width_;
}

int Map::getHeight() {
    return height_;
}

bool Map::inRange(int x, int y) {
    return x >= 0 && x < width_ && y >= 0 && y<height_;
}

void Map::setName(std::string name) {
    name_ = name_;
}

std::string Map::getName() {
    return name_;
}

double Map::getMax() {
    return max_;
}

double Map::getMin() {
    return min_;
}
