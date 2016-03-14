//
//  HydraulicErosion.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 28/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "HydraulicErosion.hpp"
#include "TemporaryHeightmap.hpp"
#include "OpenSimplexNoise.hpp"

void HydraulicErosion::perform(Heightmap& heightmap, int iterations, float K_r, float K_s, float K_e, float K_c) {
    for(int n = 0; n < iterations; n++) {
        // Appearance of new water.
        TemporaryHeightmap watermap(heightmap.getColumns(), heightmap.getRows());
        OpenSimplexNoise osn;
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                watermap.setHeightAt(column, row, K_r * glm::max((double)K_r, osn.eval(column, row)));
            }
        }
        TemporaryHeightmap sedimentmap(heightmap.getColumns(), heightmap.getRows());
        
        // Water eroding the underlying terrain and capturing the dissolved material.
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                float h = heightmap.getHeightAt(column, row);
                float w = watermap.getHeightAt(column, row);
                float s = sedimentmap.getHeightAt(column, row);
                float factor = pow(1.0f - abs(heightmap.getSlopeAt(column, row)), 3.0f);
                h -= factor * K_s * w;
                heightmap.setHeightAt(column, row, h);
                s += factor * K_s * w;
                sedimentmap.setHeightAt(column, row, s);
            }
        }
        
        // Transportation of water and sediment.
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                float h = heightmap.getHeightAt(column, row);
                float w = watermap.getHeightAt(column, row);
                float s = sedimentmap.getHeightAt(column, row);
                float a = h + w;
                int bitmask = 0b0000;
                bitmask |= (column > 0 && row > 0) ? 0b0001 : 0b0000; // tl
                bitmask |= (column > 0 && row < heightmap.getRows()) ? 0b0010 : 0b0000; // bl
                bitmask |= (column < heightmap.getColumns() && row > 0) ? 0b0100 : 0b0000; // tr
                bitmask |= (column < heightmap.getColumns() && row < heightmap.getRows()) ? 0b1000 : 0b0000; // br
                float h_i[4];
                float w_i[4];
                float s_i[4];
                float a_i[4];
                float d_i[4];
                float d_total = 0.0f;
                float a_total = a;
                int cells = 1;
                for(int bit = 0b0001, i = 0b0000; bit <= 0b1000 && i < 0b0100; bit <<= 1, i++) {
                    if(bit & bitmask) {
                        int x = column + ((i & 0b10) ? 1 : -1);
                        int y = row + ((i & 0b01) ? 1 : -1);
                        h_i[i] = heightmap.getHeightAt(x, y);
                        w_i[i] = watermap.getHeightAt(x, y);
                        s_i[i] = sedimentmap.getHeightAt(x, y);
                        a_i[i] = h_i[i] + w_i[i];
                        d_i[i] = a - a_i[i];
                        if(d_i[i] > 0) {
                            d_total += d_i[i];
                            a_total += a_i[i];
                            cells += 1;
                        }
                    }
                }
                if(cells > 1) {
                    float a_avg = a_total / (float)cells;
                    float dw_i[4];
                    float ds_i[4];
                    for(int bit = 0b0001, i = 0b0000; bit <= 0b1000 && i < 0b0100; bit <<= 1, i++) {
                        if(bit & bitmask) {
                            int x = column + ((i & 0b10) ? 1 : -1);
                            int y = row + ((i & 0b01) ? 1 : -1);
                            if(d_i[i] > 0) {
                                dw_i[i] = glm::min(w, a - a_avg) * (d_i[i] / d_total);
                                ds_i[i] = s * dw_i[i] / w;
                                w -= dw_i[i];
                                watermap.setHeightAt(column, row, w);
                                w_i[i] += dw_i[i];
                                watermap.setHeightAt(x, y, w_i[i]);
                                s -= ds_i[i];
                                sedimentmap.setHeightAt(column, row, s);
                                s_i[i] += ds_i[i];
                                sedimentmap.setHeightAt(x, y, s_i[i]);
                            }
                        }
                    }
                }
                
                // Evaporation of water and deposition of sediment.
                w *= 1 - K_e;
                watermap.setHeightAt(column, row, w);
                float s_max = K_c * w;
                float ds = glm::max(0.0f, s - s_max);
                s -= ds;
                sedimentmap.setHeightAt(column, row, s);
                h += ds;
                heightmap.setHeightAt(column, row, h);
            }
        }
        
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                float h = heightmap.getHeightAt(column, row);
                float s = sedimentmap.getHeightAt(column, row);
                h += s;
                heightmap.setHeightAt(column, row, h);
            }
        }
    }
    heightmap.calculateNormals();
}