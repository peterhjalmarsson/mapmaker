/* 
 * File:   Interface.h
 * Author: Peter Hjalmarsson 
 *
 * Created on August 31, 2015, 2:21 PM
 */

#ifndef INTERFACE_H
#define	INTERFACE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "HeightMap.h"
#include "Mask.h"
#include "Filter.h"

class Interface {
public:
    Interface();
    virtual ~Interface();
    void openFile(std::string file);
    void command(std::string text);
private:
    std::istringstream stream_;
    std::vector<Map*> heightMaps_;
    std::vector<Map*> masks_;
    //std::vector<Map*> terrains_;

    template<typename T, typename... Args>
    bool read(T &value, Args&... args);
    bool read();
    bool readMaskType(std::string maskType);
    MaskMerge readMaskMerge(std::string merge);
    MapMerge readMapMerge(std::string merge);
    FilterType readFilterType(std::string filter);
    NoiseType readNoiseType(std::string noise);
    Map* findName(std::string name, std::vector<Map*>& vect);
    void help();
    void script();
    void map();
    void map_create(std::string name);
    void map_scale(HeightMap *map);
    void map_min(HeightMap *map);
    void map_max(HeightMap *map);
    void map_merge(HeightMap *map);
    void map_name(HeightMap *map);
    void map_copy(HeightMap *map);
    void map_topng(HeightMap *map);
    void map_noise(HeightMap *map);
    void map_raise(HeightMap *map);
    void map_stretch(HeightMap *map);
    void map_lift(HeightMap *map);
    void mask();
    void mask_create(std::string name);
    void mask_high(Mask *mask);
    void mask_low(Mask *mask);
    void mask_edge(Mask *mask);
    void mask_middle(Mask *mask);
    void mask_random(Mask *mask);
    void mask_dla(Mask *mask);
    void mask_dlaquick(Mask *mask);
    void mask_growth(Mask *mask);
    void mask_name(Mask *mask);
    void mask_copy(Mask *mask);
    void mask_topng(Mask *mask);
    void terrain();
    void filter();
    void filter_mask();
    void filter_map();
    //void deform(HeightMap *map);
};

#endif	/* INTERFACE_H */

