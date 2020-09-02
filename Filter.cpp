/* 
 * File:   Filter.cpp
 * Author: Peter Hjalmarsson 
 * 
 * Created on August 31, 2015, 7:24 PM
 */

#include "Filter.h"
const schar EMPTY[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

const schar ORIG[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

const schar SHARPEN[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, -1, -1, -1, 0, 0},
    {0, 0, -1, 9, -1, 0, 0},
    {0, 0, -1, -1, -1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

const schar BLUR1[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 2, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

const schar BLUR2[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 1, 2, 2, 2, 1, 0},
    {0, 1, 2, 3, 2, 1, 0},
    {0, 1, 2, 2, 2, 1, 0},
    {0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

const schar BLUR3[7][7] = {
    {0, 0, 1, 1, 1, 0, 0},
    {0, 1, 2, 2, 2, 1, 0},
    {1, 2, 3, 3, 3, 2, 1},
    {1, 2, 3, 4, 3, 2, 1},
    {1, 2, 3, 3, 3, 2, 1},
    {0, 1, 2, 2, 2, 1, 0},
    {0, 0, 1, 1, 1, 0, 0}
};

Filter::Filter() {

}

void Filter::setType(FilterType type) {
    switch (type) {
        case FilterType::SHARPEN:
            filter_ = SHARPEN;
            break;
        case FilterType::BLUR1:
            filter_ = BLUR1;
            break;
        case FilterType::BLUR2:
            filter_ = BLUR2;
            break;
        case FilterType::BLUR3:
            filter_ = BLUR3;
            break;
        default:
            filter_ = EMPTY;
            break;
    }

}

void Filter::apply(HeightMap* map, Mask *mask) {
    HeightMap *temp = new HeightMap(*map);
    for (int x = 0; x < temp->getWidth(); x++) {
        for (int y = 0; y < temp->getHeight(); y++) {
            if (!mask->get(x, y))
                continue;
            map->set(x, y, getSum(temp, x, y));
        }
    }
    delete temp;
    map->setLimits();
}

double Filter::getSum(Map* map, int x, int y) {
    int div = 0;
    double sum = 0;
    for (int x1 = 0; x1 < 7; x1++) {
        for (int y1 = 0; y1 < 7; y1++) {
            if (filter_[x1][y1] == 0)
                continue;
            if (map->inRange(x1 + x - 3, y1 + y - 3)) {
                div += filter_[x1][y1];
                sum += filter_[x1][y1] * map->get(x1 + x - 3, y1 + y - 3);
            }
        }
    }
    return div == 0 ? sum : sum / div;
}

void Filter::apply(Mask* mask, int count, bool readVal, bool writeVal) {
    Mask *temp = new Mask(*mask);
    for (int x = 0; x < temp->getWidth(); x++) {
        for (int y = 0; y < temp->getHeight(); y++) {
            if (getCount(temp, readVal, x, y) >= count)
                mask->set(x, y, writeVal);
            else
                mask->set(x, y, !writeVal);
        }
    }
    delete temp;
}

int Filter::getCount(Mask* mask, bool readVal, int x, int y) {
    int count = 0;
    for (int x1 = 0; x1 < 7; x1++) {
        for (int y1 = 0; y1 < 7; y1++) {
            if (filter_[x1][y1] == 0)
                continue;
            if (mask->inRange(x1 + x - 3, y1 + y - 3)
                    && mask->get(x1 + x - 3, y1 + y - 3) == readVal)
                count++;
        }
    }
    return count;
}
