//
//  DiamondSquare.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 19/12/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef DiamondSquare_hpp
#define DiamondSquare_hpp

#include <stdio.h>
#include <cstdarg>
#include "Heightmap.hpp"

class DiamondSquare {
private:
    static void diamondStep(Heightmap& heightmap, float roughness, int left, int right, int top, int bottom);
    static void squareStep(Heightmap& heightmap, float roughness, int left, int right, int top, int bottom);
    static void diamondSquare(Heightmap& heightmap, float roughness, int left, int right, int top, int bottom);
    static float average(vector<float> values);
    
public:
    DiamondSquare();
    
    static void perform(Heightmap& heightmap, float roughness);
};

#endif /* DiamondSquare_hpp */
