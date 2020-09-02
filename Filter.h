/* 
 * File:   Filter.h
 * Author: Peter Hjalmarsson 
 *
 * Created on August 31, 2015, 7:24 PM
 */

#ifndef FILTER_H
#define	FILTER_H
#include "Map.h"
#include "Mask.h"
#include "HeightMap.h"
typedef signed char schar;

enum class FilterType {
    NONE, SHARPEN, BLUR1, BLUR2, BLUR3, MAX
};

class Filter {
public:
    Filter();
    void setType(FilterType type);
    void apply(HeightMap *map, Mask *mask);
    void apply(Mask *mask, int count, bool readVal, bool writeVal);
private:
    const schar(*filter_)[7];
    double getSum(Map *map, int x, int y);
    int getCount(Mask *mask, bool readVal, int x, int y);
};

#endif	/* FILTER_H */

