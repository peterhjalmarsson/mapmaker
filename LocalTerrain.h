/* 
 * File:   LocalTerrain.h
 * Author: Peter Hjalmarsson 
 *
 * Created on September 29, 2015, 4:17 PM
 */

#ifndef LOCALTERRAIN_H
#define	LOCALTERRAIN_H
#include <vector>
#include "Terrain.h"

struct TerrainParam {
    double minElevation;
    double maxElevation;
    double minHeight;
    double maxHeight;

    void set(double minElevation, double maxElevation,
            double minHeight, double maxHeight) {
        this->minElevation = minElevation;
        this->maxElevation = maxElevation;
        this->minHeight = minHeight;
        this->maxHeight = maxHeight;
    }
};

class LocalTerrain : public Terrain {
public:
    LocalTerrain();
    LocalTerrain(const LocalTerrain& orig);
    virtual ~LocalTerrain();
    void toPng(std::string name);
private:
    std::vector<TerrainParam> param;
};

#endif	/* LOCALTERRAIN_H */

