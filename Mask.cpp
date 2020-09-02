/* 
 * File:   Mask.cpp
 * Author: Peter Hjalmarsson 
 * 
 * Created on September 3, 2015, 10:01 PM
 */

#include "Mask.h"
#include "Map.h"
#include "HeightMap.h"

Mask::Mask(std::string name, int width, int height)
: Map(name, width, height) {
    min_ = 0;
    max_ = 1;
    mask_ = new uchar[(width_ * height_) / 8];
    reset();
}

Mask::Mask(std::string name, Map* map)
: Map(name, map->getWidth(), map->getHeight()) {
    min_ = 0;
    max_ = 1;
    mask_ = new uchar[(width_ * height_) / 8];
    reset();
}

Mask::Mask(const Mask& orig)
: Map(orig.name_, orig.width_, orig.height_) {
    mask_ = new uchar[(width_ * height_) / 8];
    min_ = 0;
    max_ = 1;
    std::copy_n(orig.mask_, (width_ * height_) / 8, mask_);
}

Mask::~Mask() {
    delete mask_;
}

void Mask::reset() {
    for (int i = 0; i < width_ * height_ / 8; i++) {
        mask_[i] = 0xFF;
    }
}

void Mask::invert() {
    for (int i = 0; i < width_ * height_ / 8; i++) {
        mask_[i] = ~mask_[i];
    }
}

double Mask::get(int x, int y) {
    return (mask_[(x + y * width_) >> 3] & (1 << (x & 7))) ? 1 : 0;
}

void Mask::set(int x, int y, double value) {
    if (value != 0)
        mask_[(x + y * width_) >> 3] |= 1 << (x & 7);
    else
        mask_[(x + y * width_) >> 3] &= ~(1 << (x & 7));
}

bool Mask::isMasked(int x, int y) {
    return (mask_[(x + y * width_) >> 3] & (1 << (x & 7))) == 0;
}

uint Mask::getColor(int x, int y, int bitDepth) {
    return get(x, y)*((1 << bitDepth) - 1);
}

void Mask::random(double frequency, bool setValue) {
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (get(x, y) == setValue)
                continue;
            if ((double) rand() / (double) RAND_MAX < frequency)
                set(x, y, setValue);
        }
    }
}

void Mask::dlaQuick(int count, double moveDist, double aggDist, bool setValue) {
    /*check there is something to stick to*/
    bool ok = false;
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if ((bool)get(x, y) == setValue) {
                ok = true;
                break;
            }
        }
    }
    if (!ok)
        set(width_ / 2, height_ / 2, setValue);
    int counter = 0;
    while (counter < count) {
        double x = (double) rand() / (double) RAND_MAX * (width_ - 2) + 1;
        double y = (double) rand() / (double) RAND_MAX * (height_ - 2) + 1;
        while (true) {
            if (checkNeighbors(x, y, aggDist, setValue)) {
                counter++;
                set(x + 0.5, y + 0.5, setValue);
                break;
            }
            double angle = (double) rand() / (double) RAND_MAX * 2 * 3.1416;
            x += cos(angle) * moveDist;
            y += sin(angle) * moveDist;
            if (x < 1 || x >= width_ - 1 || y < 1 || y >= height_ - 1) {
                break;
            }
        }
    }
}

void Mask::dla(int count, double moveDist, double aggDist, bool setValue) {
    /*check there is something to stick to*/
    bool ok = false;
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if ((bool)get(x, y) == setValue) {
                ok = true;
                break;
            }
        }
    }
    if (!ok)
        set(width_ / 2, height_ / 2, setValue);
    double dist = (width_ + height_) / 2;
    int counter = 0;
    while (counter < count) {
        double angle = (double) rand() / (double) RAND_MAX * 2 * 3.1416;
        double x = cos(angle) * dist + width_ / 2;
        double y = sin(angle) * dist + height_ / 2;
        while (true) {
            if (inRange(x, y) && checkNeighbors(x, y, aggDist, setValue)) {
                counter++;
                set(x + 0.5, y + 0.5, setValue);
                break;
            }
            angle = (double) rand() / (double) RAND_MAX * 2 * 3.1416;
            x += cos(angle) * moveDist;
            y += sin(angle) * moveDist;
            if ((x - width_ / 2)*(x - width_ / 2)
                    +(y - height_ / 2)*(y - height_ / 2) > dist * dist * 2) {
                break;
            }
        }
    }
}

void Mask::growth(bool setValue) {
    Mask *temp = new Mask(*this);
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (get(x, y) != setValue) {
                int chance = 0;
                for (int x1 = x - 1; x1 < x + 2; x1++) {
                    for (int y1 = y - 1; y1 < y + 2; y1++) {
                        if (inRange(x1, y1) && temp->get(x1, y1) == setValue) {
                            if (x1 == 0 || y1 == 0)
                                chance += 15;
                            else
                                chance += 10;
                        }
                    }
                }
                if (rand() % 100 < chance){
                    set(x, y, setValue);
                }
            }
        }
    }
    delete temp;
}

bool Mask::checkNeighbors(double x, double y, double aggDist, bool setValue) {
    for (int x1 = x - aggDist; x1 < x + aggDist + 1; x1++) {
        for (int y1 = y - aggDist; y1 < y + aggDist + 1; y1++) {
            double xsum = (double) x1 - x;
            double ysum = (double) y1 - y;
            if (inRange(x1, y1) && (xsum * xsum + ysum * ysum < aggDist * aggDist)
                    && ((bool)get(x1, y1) == setValue))
                return true;
        }
    }
    return false;
}

void Mask::maskLow(Map* map, double height, bool inverted, bool setValue) {
    double wScale = (double) map->getWidth() / (double) width_;
    double hScale = (double) map->getHeight() / (double) height_;
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (inverted) {
                if (map->get((int) (width_ * wScale + 0.5), (int) (height_ * hScale + 0.5)) > height)
                    set(x, y, setValue);
            } else {
                if (map->get((int) (width_ * wScale + 0.5), (int) (height_ * hScale + 0.5)) < height)
                    set(x, y, setValue);
            }
        }
    }
}

void Mask::maskBorder(int dist, bool inverted, bool setValue) {
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (!inverted) {
                if ((x < dist || x >= width_ - dist) || (y < dist || y >= height_ - dist))
                    set(x, y, setValue);
            } else {
                if (x >= dist && x < width_ - dist && y >= dist && y < height_ - dist)
                    set(x, y, setValue);
            }
        }
    }
}
