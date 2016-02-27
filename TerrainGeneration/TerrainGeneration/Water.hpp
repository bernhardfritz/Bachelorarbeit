//
//  Water.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Water_hpp
#define Water_hpp

#include <stdio.h>
#include "Heightmap.hpp"
#include "OpenSimplexNoise.hpp"

class Water : public Heightmap {
private:
    float waveLevel;
    float waveWidth;
    float waveHeight;
    float waveTime;
    OpenSimplexNoise osm;
public:
    Water(int columns, int rows, float waveLevel, float waveWidth, float waveHeight);
    
    void setWaveLevel(float waveLevel);
    float getWaveLevel();
    void setWaveWidth(float waveWidth);
    float getWaveWidth();
    void setWaveHeight(float waveHeight);
    float getWaveHeight();
    void setWaveTime(float waveTime);
    float getWaveTime();
    
    void step(float delta);
};

#endif /* Water_hpp */
