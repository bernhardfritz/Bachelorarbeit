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
    class TemporaryHeightmap {
    private:
        int columns;
        int rows;
        vector<float> heights;
        
    public:
        TemporaryHeightmap(int columns, int rows, float h = 0.0f) : columns(columns), rows(rows) {
            for(int row = 0; row <= rows; row++) {
                for(int column = 0; column <= columns; column++) {
                    heights.push_back(h);
                }
            }
        }
        
        void setHeightAt(int column, int row, float height) {
            if(column <= columns && row <= rows) {
                heights[row * (columns + 1) + column] = height;
            }
        }
        
        float getHeightAt(int column, int row) {
            if(column <= columns && row <= rows) {
                return heights[row * (columns + 1) + column];
            }
            return 0.0f;
        }
    };
public:
    // K_r ... Amount of water to be added to each cell per iteration
    // K_s ... Solubility constant
    // K_e ... Evaporation coefficient
    // K_c ... Sediment capacity coefficient
    static void perform(Heightmap& heightmap, int iterations, float K_r = 0.01f, float K_s = 0.01f, float K_e = 0.5f, float K_c = 0.01f);
};

#endif /* HydraulicErosion_hpp */
