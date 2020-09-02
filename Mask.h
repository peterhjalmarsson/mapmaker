/* 
 * File:   Mask.h
 * Author: Peter Hjalmarsson 
 *
 * Created on September 3, 2015, 10:01 PM
 */

#ifndef MASK_H
#define	MASK_H
#include <algorithm>
#include <iostream>
#include "Map.h"

enum class MaskMerge {
    NONE, AND, OR, XOR, NOT
};

class Mask : public Map {
public:
    Mask(std::string name, int width, int height);
    Mask(std::string name, Map *map);
    Mask(const Mask& orig);
    virtual ~Mask();
    void reset();
    void invert();
    double get(int x, int y);
    void set(int x, int y, double value);
    bool isMasked(int x, int y);
    uint getColor(int x, int y, int bitDepth);
    void random(double frequency, bool setValue);
    void dlaQuick(int count, double moveDist, double aggDist, bool setValue);
    void dla(int count, double moveDist, double aggDist, bool setValue);
    void growth(bool inverted);
    void maskLow(Map *map, double height, bool inverted, bool setValue);
    void maskBorder(int dist, bool inverted, bool setValue);
private:
    uchar *mask_;
    bool checkNeighbors(double x, double y, double aggDist, bool setValue);
};

#endif	/* MASK_H */

