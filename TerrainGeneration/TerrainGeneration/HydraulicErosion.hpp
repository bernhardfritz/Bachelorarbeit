//
//  HydraulicErosion.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 28/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef HydraulicErosion_hpp
#define HydraulicErosion_hpp

#include <stdio.h>
#include "Heightmap.hpp"

class HydraulicErosion {
private:
public:
    // K_r ... Amount of water to be added to each cell per iteration
    // K_s ... Solubility constant
    // K_e ... Evaporation coefficient
    // K_c ... Sediment capacity coefficient
    static void perform(Heightmap& heightmap, int iterations, float K_r = 0.01f, float K_s = 0.01f, float K_e = 0.5f, float K_c = 0.01f);
};

#endif /* HydraulicErosion_hpp */
