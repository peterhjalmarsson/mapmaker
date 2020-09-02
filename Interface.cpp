/* 
 * File:   Interface.cpp
 * Author: Peter Hjalmarsson 
 * 
 * Created on August 31, 2015, 2:21 PM
 */

#include "Interface.h"
#include "ImageIO.h"
#include "Filter.h"
//#include "Deformer.h"

Interface::Interface() {
}

Interface::~Interface() {
}

template<typename T, typename... Args>
bool Interface::read(T &value, Args&... args) {
    stream_>>value;
    return read(args...);
}

bool Interface::read() {
    return !stream_.fail();
}

bool Interface::readMaskType(std::string maskType) {
    if (maskType != "mask" && maskType != "unmask") {
        std::cerr << "   ILLEGAL ARGUMENT" << std::endl;
        return false;
    }
    return true;
}

MaskMerge Interface::readMaskMerge(std::string merge) {
    if (merge == "and") {
        return MaskMerge::AND;
    } else if (merge == "or") {
        return MaskMerge::OR;
    } else if (merge == "xor") {
        return MaskMerge::XOR;
    } else if (merge == "not") {
        return MaskMerge::NOT;
    } else {
        std::cerr << "   ILLEGAL ARGUMENT" << std::endl;
        return MaskMerge::NONE;
    }
}

MapMerge Interface::readMapMerge(std::string merge) {
    if (merge == "add") {
        return MapMerge::ADD;
    } else if (merge == "subtract" || merge == "sub") {
        return MapMerge::SUBTRACT;
    } else if (merge == "multiply" || merge == "mult") {
        return MapMerge::MULTIPLY;
    } else if (merge == "difference" || merge == "diff") {
        return MapMerge::DIFFERENCE;
    } else if (merge == "maximum" || merge == "max") {
        return MapMerge::MAXIMUM;
    } else if (merge == "minimum" || merge == "min") {
        return MapMerge::MINIMUM;
    } else if (merge == "exponential" || merge == "exp") {
        return MapMerge::EXPONENTIAL;
    } else {
        std::cerr << "   ILLEGAL ARGUMENT" << std::endl;
        return MapMerge::NONE;
    }
}

FilterType Interface::readFilterType(std::string filter) {
    if (filter == "blur1") {
        return FilterType::BLUR1;
    } else if (filter == "blur2") {
        return FilterType::BLUR2;
    } else if (filter == "blur3") {
        return FilterType::BLUR3;
    } else if (filter == "sharpen") {
        return FilterType::SHARPEN;
    } else {
        std::cerr << "   ILLEGAL ARGUMENT" << std::endl;
        return FilterType::NONE;
    }
}

NoiseType Interface::readNoiseType(std::string noise) {
    if (noise == "billow") {
        return NoiseType::BILLOW;
    } else if (noise == "checkerboard") {
        return NoiseType::CHECKERBOARD;
    } else if (noise == "cylinders") {
        return NoiseType::CYLINDERS;
    } else if (noise == "perlin") {
        return NoiseType::PERLIN;
    } else if (noise == "ridgedmulti") {
        return NoiseType::RIDGEDMULTI;
    } else if (noise == "spheres") {
        return NoiseType::SPHERES;
    } else if (noise == "voronoi") {
        return NoiseType::VORONOI;
    } else {
        std::cerr << "   ILLEGAL ARGUMENT" << std::endl;
        return NoiseType::NONE;
    }
}

Map* Interface::findName(std::string name, std::vector<Map*>& vect) {
    Map *area = NULL;
    for (Map *a : vect) {
        if (a->getName() == name) {
            area = a;
            break;
        }
    }
    if (area == NULL) {
        std::cerr << "   CANNOT FIND " << name << std::endl;
    }
    return area;
}

void Interface::openFile(std::string file) {
    std::ifstream inFile(file.c_str());
    std::string line;
    if (inFile.is_open()) {
        std::string text;
        while (getline(inFile, text)) {
            command(text);
        }
        inFile.close();
    } else {
        std::cerr << "    UNABLE TO OPEN FILE" << file << std::endl;
    }
}

void Interface::command(std::string text) {
    if (text == "") {
#ifdef _DEBUG_INFO
        std::cout << "EMPTY LINE" << std::endl;
#endif
        return;
    }
    stream_.clear();
    stream_.str(text);
    std::string name;
    stream_>>name;
    if (name.at(0) == '#') {
#ifdef _DEBUG_INFO
        std::cout << text << std::endl;
#endif
        return;
    }
    std::cout << ">> " << text << std::endl;
    if (name == "help") {
        help();
    } else if (name == "script") {
        script();
    } else if (name == "seed") {
        int seed;
        if (read(seed))
            srand(seed);
    } else if (name == "map") {
        map();
    } else if (name == "mask") {
        mask();
    } else if (name == "terrain") {
        terrain();
    } else if (name == "filter") {
        filter();
    } else {
        std::cerr << "   UNKNOWN COMMAND: " << name << std::endl;
    }
}

void Interface::help() {
    std::ifstream inFile("./help.txt");
    std::string line;
    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            std::cout << line << std::endl;
        }
        inFile.close();
    } else {
        std::cerr << "Unable to open help file" << std::endl;
    }
}

void Interface::script() {
    std::string name;
    if (read(name))
        openFile(name);
}

void Interface::map() {
    std::string name;
    std::string option;
    if (!read(name, option))
        return;
    if (option == "create") {
        map_create(name);
        return;
    }
    HeightMap *map = (HeightMap*) findName(name, heightMaps_);
    if (map == NULL)
        return;
    if (option == "reset") {
        map->reset();
    } else if (option == "invert") {
        map->invert();
    } else if (option == "scale") {
        map_scale(map);
    } else if (option == "min") {
        map_min(map);
    } else if (option == "max") {
        map_max(map);
    } else if (option == "merge") {
        map_merge(map);
    } else if (option == "name") {
        map_name(map);
    } else if (option == "copy") {
        map_copy(map);
    } else if (option == "topng") {
        map_topng(map);
    } else if (option == "noise") {
        map_noise(map);
    } else if (option == "raise") {
        map_raise(map);
    } else if (option == "stretch") {
        map_stretch(map);
    } else if (option == "lift") {
        map_lift(map);
    } else {
        std::cerr << "   ILLEGAL OPTION: " << option << std::endl;
    }
}

void Interface::map_create(std::string name) {
    int x;
    int y;
    if (read(x, y)) {
        HeightMap *map = new HeightMap(name, x, y);
        heightMaps_.push_back(map);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_scale(HeightMap *map) {
    std::string maskName;
    double factor;
    if (read(maskName, factor)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        map->scale(mask, factor);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_max(HeightMap *map) {
    std::string maskName;
    int value;
    if (read(maskName, value)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        map->scale(mask, value, map->getMin());
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_min(HeightMap *map) {
    std::string maskName;
    int value;
    if (read(value)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        map->scale(mask, map->getMax(), value);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_merge(HeightMap* map) {
    std::string name2;
    std::string merge;
    if (read(name2, merge)) {
        HeightMap *map2 = (HeightMap*) findName(name2, heightMaps_);
        if (map2 == NULL)
            return;
        MapMerge mode = readMapMerge(merge);
        if (mode == MapMerge::NONE)
            return;
        map->merge(mode, map2);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_name(HeightMap* map) {
    std::string name;
    if (read(name)) {
        map->setName(name);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_copy(HeightMap* map) {
    std::string name;
    if (read(name)) {
        HeightMap *m = new HeightMap(*map);
        m->setName(name);
        heightMaps_.push_back(m);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_topng(HeightMap* map) {
    std::string image;
    if (read(image)) {
        ImageIO *out = new ImageIO();
        out->writeGrayscalePng16Bit(map, image);
        delete out;
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_noise(HeightMap *map) {
    std::string type;
    std::string maskName;
    double scale;
    double frequency = 1.0;
    double persistence = 0.5;
    double lacunarity = 2.0;
    int octaveCount = 6;
    if (!read(type, maskName, scale)) {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
        return;
    }
    read(frequency, persistence, lacunarity, octaveCount);
    NoiseType t = readNoiseType(type);
    if (t == NoiseType::NONE)
        return;
    Mask *mask = (Mask*) findName(maskName, masks_);
    if (mask == NULL)
        return;
    map->noise(t, mask, scale, frequency, persistence, lacunarity, octaveCount);
}

void Interface::map_raise(HeightMap* map) {
    std::string maskName;
    double height = 0;
    double random = 0;
    if (read(maskName, height)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        read(random);
        map->raise(mask, height, random);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_stretch(HeightMap* map) {
    std::string maskName;
    double height;
    double random;
    if (read(maskName, height, random)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        map->stretch(mask, height, random);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::map_lift(HeightMap* map) {
    std::string maskName;
    double height;
    double random;
    if (read(maskName, height, random)) {
        Mask *mask = (Mask*) findName(maskName, masks_);
        if (mask == NULL)
            return;
        map->lift(mask, height, random);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask() {
    std::string name;
    std::string option;
    if (!read(name, option))
        return;
    if (option == "create") {
        mask_create(name);
        return;
    }
    Mask *mask = (Mask*) findName(name, masks_);
    if (mask == NULL)
        return;
    if (option == "reset") {
        mask->reset();
    } else if (option == "invert") {
        mask->invert();
    } else if (option == "high") {
        mask_high(mask);
    } else if (option == "low") {
        mask_low(mask);
    } else if (option == "edge") {
        mask_edge(mask);
    } else if (option == "middle") {
        mask_middle(mask);
    } else if (option == "random") {
        mask_random(mask);
    } else if (option == "dla") {
        mask_dla(mask);
    } else if (option == "dlaquick") {
        mask_dlaquick(mask);
    } else if (option == "growth") {
        mask_growth(mask); 
    } else if (option == "name") {
        mask_name(mask);
    } else if (option == "copy") {
        mask_copy(mask);
    } else if (option == "topng") {
        mask_topng(mask);
    } else {
        std::cerr << "   ILLEGAL OPTION: " << option << std::endl;
    }
}

void Interface::mask_create(std::string name) {
    std::string mapName;
    if (read(mapName)) {
        HeightMap *map = (HeightMap*) findName(mapName, heightMaps_);
        if (map == NULL)
            return;
        Mask *mask = new Mask(name, map);
        masks_.push_back(mask);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_high(Mask* mask) {
    std::string mapName;
    double height;
    std::string masking;
    if (read(mapName, height, masking)) {
        Map *map = (Map*) findName(mapName, heightMaps_);
        if (map == NULL)
            return;
        if (!readMaskType(masking))
            return;
        mask->maskLow(map, height, true, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_low(Mask* mask) {
    std::string mapName;
    double height;
    std::string masking;
    if (read(mapName, height, masking)) {
        Map *map = (Map*) findName(mapName, heightMaps_);
        if (map == NULL)
            return;
        if (!readMaskType(masking))
            return;
        mask->maskLow(map, height, false, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_edge(Mask* mask) {
    int distance;
    std::string masking;
    if (read(distance, masking)) {
        if (!readMaskType(masking))
            return;
        mask->maskBorder(distance, false, masking == "unmask");
    }
}

void Interface::mask_middle(Mask* mask) {
    int distance;
    std::string masking;
    if (read(distance, masking)) {
        if (!readMaskType(masking))
            return;
        mask->maskBorder(distance - 1, true, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_random(Mask* mask) {
    double frequency;
    std::string masking;
    if (read(frequency, masking)) {
        if (!readMaskType(masking))
            return;
        mask->random(frequency, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_dla(Mask* mask) {
    int count;
    double moveDist;
    double aggDist;
    std::string masking;
    if (read(count, moveDist, aggDist, masking)) {
        if (!readMaskType(masking))
            return;
        mask->dla(count, moveDist, aggDist, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_dlaquick(Mask* mask) {
    int count;
    double moveDist;
    double aggDist;
    std::string masking;
    if (read(count, moveDist, aggDist, masking)) {
        if (!readMaskType(masking))
            return;
        mask->dlaQuick(count, moveDist, aggDist, masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_growth(Mask* mask) {
    int count;
    std::string masking;
    if (read(count, masking)) {
        if (!readMaskType(masking))
            return;
        for(int i=0;i<count;i++)
            mask->growth(masking == "unmask");
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_name(Mask* mask) {
    std::string name;
    if (read(name)) {
        mask->setName(name);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::mask_copy(Mask* mask) {
    std::string name;
    if (read(name)) {
        Mask *m = new Mask(*mask);
        m->setName(name);
        masks_.push_back(m);
    }
}

void Interface::mask_topng(Mask* mask) {
    std::string image;
    if (read(image)) {
        ImageIO *out = new ImageIO();
        out->writeGrayscalePng8Bit((Map*) mask, image);
        delete out;
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
}

void Interface::terrain() {

}

void Interface::filter() {
    std::string option;
    if (!read(option))
        return;
    if (option == "mask") {
        filter_mask();
    } else if (option == "map") {
        filter_map();
    } else {
        std::cerr << "   ILLEGAL OPTION: " << option << std::endl;
    }
}

void Interface::filter_map() {
    Filter *filter = new Filter();
    std::string mapName;
    std::string maskName;
    if(!read(mapName,maskName))
        return;
    HeightMap *map = (HeightMap*)findName(mapName, heightMaps_);
    if (map == NULL)
        return;
    Mask *mask = (Mask*) findName(maskName, masks_);
    if (mask == NULL)
        return;
    std::string filt;
    if(read(filt)){
        FilterType ft=readFilterType(filt);
        if(ft==FilterType::NONE)
            return;
        filter->setType(ft);
        filter->apply(map,mask);
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
    delete filter;
}

void Interface::filter_mask() {
    Filter *filter = new Filter();
    std::string name;
    std::string type;
    if (read(name, type)) {
        Mask *mask = (Mask*) findName(name, masks_);
        if (mask == NULL)
            return;
        if (type == "grow1") {
            filter->setType(FilterType::BLUR1);
            filter->apply(mask, 1, false, false);
        } else if (type == "grow2") {
            filter->setType(FilterType::BLUR2);
            filter->apply(mask, 1, false, false);
        } else if (type == "grow3") {
            filter->setType(FilterType::BLUR3);
            filter->apply(mask, 1, false, false);
        } else if (type == "growsmooth") {
            filter->setType(FilterType::BLUR1);
            filter->apply(mask, 4, false, false);
        }
    } else {
        std::cerr << "   ILLEGAL OR MISSING ARGUMENTS" << std::endl;
    }
    delete filter;
}

//void Interface::deform(Map *map) {
//    Deformer *deform=new Deformer();
//    deform->perlin(map);
//    delete deform;
//}
