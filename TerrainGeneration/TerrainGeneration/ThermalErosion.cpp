//
//  ThermalErosion.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 24/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "ThermalErosion.hpp"

void ThermalErosion::perform(Heightmap &heightmap, int iterations, float talus) {
    for(int n = 0; n < iterations; n++) {
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                float h = heightmap.getHeightAt(column, row);
                int bitmask = 0b0000;
                bitmask |= (column > 0 && row > 0) ? 0b0001 : 0b0000; // tl
                bitmask |= (column > 0 && row < heightmap.getRows()) ? 0b0010 : 0b0000; // bl
                bitmask |= (column < heightmap.getColumns() && row > 0) ? 0b0100 : 0b0000; // tr
                bitmask |= (column < heightmap.getColumns() && row < heightmap.getRows()) ? 0b1000 : 0b0000; // br
                float h_i[4];
                float d_i[4];
                float d_total = 0.0f;
                float d_max = 0.0f;
                for(int bit = 0b0001, i = 0b0000; bit <= 0b1000 && i < 0b0100; bit <<= 1, i++) {
                    if(bit & bitmask) {
                        int x = column + ((i & 0b10) ? 1 : -1);
                        int y = row + ((i & 0b01) ? 1 : -1);
                        h_i[i] = heightmap.getHeightAt(x, y);
                        d_i[i] = h - h_i[i];
                        if(d_i[i] > talus) {
                            d_total += d_i[i];
                            if(d_i[i] > d_max) {
                                d_max = d_i[i];
                            }
                        }
                    }
                }
                for(int bit = 0b0001, i = 0b0000; bit <= 0b1000 && i < 0b0100; bit <<= 1, i++) {
                    if(bit & bitmask) {
                        int x = column + ((i & 0b10) ? 1 : -1);
                        int y = row + ((i & 0b01) ? 1 : -1);
                        if(d_i[i] > talus) {
                            float factor = pow(heightmap.getMaxHeight() - h, 3.0f);
                            float dh = factor * (d_max - talus) * (d_i[i]/d_total);
                            h -= dh;
                            heightmap.setHeightAt(column, row, h);
                            h_i[i] += dh;
                            heightmap.setHeightAt(x, y, h_i[i]);
                        }
                    }
                }
            }
        }
    }
    heightmap.calculateNormals();
}