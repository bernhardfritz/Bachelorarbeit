//
//  Fault.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/12/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "Fault.hpp"
#include <iostream>

void Fault::perform(Heightmap &heightmap, float roughness, int iterations) {
    for(int i = 0; i < iterations; i++) {
        vec3 v1(drand48() * heightmap.getColumns(), 0, drand48() * heightmap.getRows());
        vec3 v2(drand48() * heightmap.getColumns(), 0, drand48() * heightmap.getRows());
        vec3 line = v2 - v1;
        for(int row = 0; row <= heightmap.getRows(); row++) {
            for(int column = 0; column <= heightmap.getColumns(); column++) {
                vec3 v(column, 0, row);
                float sign = ((glm::cross(v - v1, line)).y >= 0.0f) ? 1.0f : -1.0f;
                heightmap.setHeightAt(column, row, heightmap.getHeightAt(column, row) + sign * roughness);
            }
        }
        roughness *= 1.0f - 1.0f / iterations;
    }
    heightmap.calculateNormals();
}