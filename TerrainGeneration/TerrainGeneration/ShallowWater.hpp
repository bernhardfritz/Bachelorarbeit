//
//  ShallowWater.hpp
//  TerrainGeneration
//
//  ShallowWater written in Javascript by Trevor Dixon http://jsfiddle.net/trevordixon/DYVfa/
//  Translated to C++ and modified by Bernhard Fritz on 19/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef ShallowWater_hpp
#define ShallowWater_hpp

#include "Heightmap.hpp"

class ShallowWater : public Heightmap {
private:
    Heightmap& heightmap;
    float G = 9.81f;
    float H = 0.05f;
    float DAMP = 0.05f;
    float TIMESTEP = 0.1f;
    
    float* _h;
    float* _u;
    float* _v;
    
    float gauss[9] = {
        1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f,
        1.0f/8.0f , 1.0f/4.0f, 1.0f/8.0f ,
        1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f
    };
    
public:
    ShallowWater(Heightmap& heightmap);
    ~ShallowWater();
    
    void rain(float weight, int radius);
    void step();
};

#endif /* ShallowWater_hpp */
