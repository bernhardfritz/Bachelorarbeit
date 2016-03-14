//
//  TemporaryHeightmap.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 13/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "TemporaryHeightmap.hpp"

TemporaryHeightmap::TemporaryHeightmap(int columns, int rows, float h) : columns(columns), rows(rows) {
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            heights.push_back(h);
        }
    }
}

void TemporaryHeightmap::setHeightAt(int column, int row, float height) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        heights[row * (columns + 1) + column] = height;
    }
}

float TemporaryHeightmap::getHeightAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return heights[row * (columns + 1) + column];
    }
    return 0.0f;
}