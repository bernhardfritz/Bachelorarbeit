//
//  Heightmap.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 09/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "Heightmap.hpp"

Heightmap::Heightmap(int columns, int rows) {
    vector<vec3> vertices;
    vector<unsigned int> indices;
    for(int z = 0; z <= rows; z++) {
        for(int x = 0; x <= columns; x++) {
            vertices.push_back(vec3(x, 0, z));
        }
    }
    
    for(int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            indices.push_back(column + row * (columns + 1));
            indices.push_back(column + (row + 1) * (columns + 1));
            indices.push_back(column + 1 + row * (columns + 1));
            indices.push_back(column + 1 + row * (columns + 1));
            indices.push_back(column + (row + 1) * (columns + 1));
            indices.push_back(column + 1 + (row + 1) * (columns + 1));
        }
    }
    
    setVertices(vertices);
    setIndices(indices);
    setMaterial(new Material());
    calculateNormals();
    init();
}