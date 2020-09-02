/* 
 * File:   Output.h
 * Author: Peter Hjalmarsson 
 *
 * Created on August 29, 2015, 7:08 PM
 */

#ifndef OUTPUT_H
#define	OUTPUT_H

#include <png++/png.hpp>
#include <string.h>
//#include "Terrain.h"
#include "Map.h"
#include "Mask.h"

class ImageIO {
public:
    ImageIO();
    virtual ~ImageIO();
    void writeGrayscalePng8Bit(Map *map, std::string file);
    void writeGrayscalePng16Bit(Map *map, std::string file);
private:
};

#endif	/* OUTPUT_H */

