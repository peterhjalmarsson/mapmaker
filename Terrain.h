/* 
 * File:   Terrain.h
 * Author: Peter Hjalmarsson 
 *
 * Created on September 29, 2015, 4:14 PM
 */

#ifndef TERRAIN_H
#define	TERRAIN_H
#include <string>

class Terrain {
public:
    Terrain();
    Terrain(const Terrain& orig);
    virtual ~Terrain();
    virtual void toPng(std::string name) = 0;
private:

};

#endif	/* TERRAIN_H */

