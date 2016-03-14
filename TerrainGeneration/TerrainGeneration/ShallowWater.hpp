//
//  ShallowWater.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 12/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef ShallowWater_hpp
#define ShallowWater_hpp

#include <stdio.h>
#include "Heightmap.hpp"
#include "TemporaryHeightmap.hpp"

typedef struct {
    float fL;
    float fR;
    float fT;
    float fB;
} Flux;

class ShallowWater : public Heightmap {
private:
    Heightmap& heightmap;
    TemporaryHeightmap watermap;
    vector<Flux> flux;
public:
    ShallowWater(Heightmap& heightmap);
    void flow(float dt, int iterations);
};

#endif /* ShallowWater_hpp */
