/* 
 * File:   GlobalTerrain.h
 * Author: Peter Hjalmarsson 
 *
 * Created on September 29, 2015, 4:20 PM
 */

#ifndef GLOBALTERRAIN_H
#define	GLOBALTERRAIN_H
#include "Terrain.h"
class GlobalTerrain : public Terrain {
public:
    GlobalTerrain();
    GlobalTerrain(const GlobalTerrain& orig);
    virtual ~GlobalTerrain();
    void tpPng(std::string name);
private:

};

#endif	/* GLOBALTERRAIN_H */

