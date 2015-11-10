//
//  Heightmap.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 09/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <string>
#include <iostream>
#include "Heightmap.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Heightmap::Heightmap(int columns, int rows) {
    setColumns(columns);
    setRows(rows);
    
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

void Heightmap::setColumns(int columns) {
    this->columns = columns;
}

int Heightmap::getColumns() {
    return columns;
}

void Heightmap::setRows(int rows) {
    this->rows = rows;
}

int Heightmap::getRows() {
    return rows;
}

void Heightmap::setHeightAt(int column, int row, float height) {
    if(column <= columns && row <= rows) {
        vertices[row * (columns + 1) + column].y = height;
    }
}

void Heightmap::loadHeightmap(string filename, int maxHeight) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 1); // force 1 channel
    
    if(width < columns || height < rows) {
        cout << "The provided file " << filename << " (" << width << "x" << height << ") is too small and cannot be used for this heightmap (" << columns << "x" << rows << ")" << endl;
        stbi_image_free(data);
        return;
    }
    
    int dx = width/columns;
    int dy = height/rows;
    
    for(int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            setHeightAt(column, row, (data[row * width * dy + column * dx] / 255.0f) * maxHeight);
        }
    }
    
    stbi_image_free(data);
    
    init();
}