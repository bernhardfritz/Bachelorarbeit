//
//  ThermalErosion.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 24/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef ThermalErosion_hpp
#define ThermalErosion_hpp

#include <stdio.h>
#include "Heightmap.hpp"

class ThermalErosion {
private:
public:
    static void perform(Heightmap& heightmap, float talus, int iterations);
};

#endif /* ThermalErosion_hpp */
