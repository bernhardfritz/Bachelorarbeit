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
#include "TextureLoader.hpp"

Heightmap::Heightmap(int columns, int rows) {
    setColumns(columns);
    setRows(rows);
    
    vector<vec3> vertices;
    vector<unsigned int> indices;
    vector<vec2> texcoords;
    for(int z = 0; z <= rows; z++) {
        for(int x = 0; x <= columns; x++) {
            vertices.push_back(vec3(x, 0, z));
            texcoords.push_back(vec2(x, z));
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
    setTexcoords(texcoords);
    setMaterial(new Material());
    getMaterial()->setSpecularReflectance(0.05f);
    getMaterial()->setShininess(1.0f);
    getMaterial()->setAmbientReflectance(0.5);
    init();
    calculateNormals();
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
        if(height < minHeight) minHeight = height;
        if(height > maxHeight) maxHeight = height;
    }
}

float Heightmap::getHeightAt(int column, int row) {
    if(column <= columns && row <= rows) {
        return vertices[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

float Heightmap::getSlopeAt(int column, int row) {
    if(column <= columns && row <= rows) {
        return normals[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

void Heightmap::loadHeightmap(string filename, int strength) {
    TextureLoader tl;
    Texture t = tl.loadTexture(filename, 1);
    
    if(t.getWidth() < columns || t.getHeight() < rows) {
        cout << "The provided file " << filename << " (" << t.getWidth() << "x" << t.getHeight() << ") is too small and cannot be used for this heightmap (" << columns << "x" << rows << ")" << endl;
        return;
    }
    
    int dx = t.getWidth()/columns;
    int dy = t.getHeight()/rows;
    
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            setHeightAt(column, row, (t.getData()[row * t.getWidth() * dy + column * dx] / 255.0f) * strength);
        }
    }
    
    calculateNormals();
}

float Heightmap::getMinHeight() {
    return minHeight;
}

float Heightmap::getMaxHeight() {
    return maxHeight;
}