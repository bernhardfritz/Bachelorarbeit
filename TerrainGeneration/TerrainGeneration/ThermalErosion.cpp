//
//  ThermalErosion.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 24/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "ThermalErosion.hpp"

void ThermalErosion::perform(Heightmap &heightmap, float talus, int iterations) {
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
                int max = -1;
                for(int bit = 0b0001, i = 0b0000; bit <= 0b1000 && i < 0b0100; bit <<= 1, i++) {
                    if(bit & bitmask) {
                        if(max == -1) max = i;
                        int x = column + ((i & 0b10) ? 1 : -1);
                        int y = row + ((i & 0b01) ? 1 : -1);
                        h_i[i] = heightmap.getHeightAt(x, y);
                        d_i[i] = h - h_i[i];
                        if(d_i[i] > d_i[max]) max = i;
                    }
                }
                if(max > -1) {
                    if(d_i[max] <= talus && h_i[max] < h) {
                        float factor = pow(heightmap.getMaxHeight()-h, 3.0f);
                        float delta = factor * d_i[max];
                        h -= delta;
                        h_i[max] += delta;
                        heightmap.setHeightAt(column, row, h);
                        int x = column + ((max & 0b10) ? 1 : -1);
                        int y = row + ((max & 0b01) ? 1 : -1);
                        heightmap.setHeightAt(x, y, h_i[max]);
                    }
                }
            }
        }
    }
    heightmap.calculateNormals();
}