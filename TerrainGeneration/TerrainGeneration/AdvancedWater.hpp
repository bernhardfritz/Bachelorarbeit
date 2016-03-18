//
//  AdvancedWater.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 18/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef AdvancedWater_hpp
#define AdvancedWater_hpp

#include "Heightmap.hpp"

typedef struct {
    float t;
    float b;
    float l;
    float r;
} Direction;

class AdvancedWater : public Heightmap {
private:
    Heightmap& heightmap;
    float g = 9.81f;
    float dx;
    float dy;
    float L;
    float A;
    Direction* f;
    float* d;
public:
    AdvancedWater(Heightmap& heightmap);
    ~AdvancedWater();
    
    void step(float dt);
};

#endif /* AdvancedWater_hpp */
