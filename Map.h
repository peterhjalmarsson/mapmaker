/* 
 * File:   Map.h
 * Author: Peter Hjalmarsson 
 *
 * Created on September 3, 2015, 9:34 PM
 */

#ifndef MAP_H
#define	MAP_H
#include <string>
typedef unsigned char uchar;

class Map {
public:
    Map(std::string name, int width, int height);
    virtual ~Map();
    int getWidth();
    int getHeight();
    bool inRange(int x, int y);
    virtual double get(int x, int y) = 0;
    virtual void set(int x, int y, double value) = 0;
    virtual void reset()=0;
    virtual void invert()=0;
    std::string getName();
    void setName(std::string name);
    virtual uint getColor(int x, int y, int bitDepth)=0;
    double getMin();
    double getMax();
protected:
    const int width_;
    const int height_;
    std::string name_;
    double min_;
    double max_;
};

#endif	/* MAP_H */

