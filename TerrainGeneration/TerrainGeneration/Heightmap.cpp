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
            vertices.push_back(vec3(x*(1.0f/columns), 0, z*(1.0f/rows)));
            texcoords.push_back(vec2(x*0.5f, z*0.5f));
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
    material.setSpecularReflectance(0.05f);
    material.setShininess(1.0f);
    material.setAmbientReflectance(0.5f);
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
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        vertices[row * (columns + 1) + column].y = height;
        if(height < minHeight) minHeight = height;
        if(height > maxHeight) maxHeight = height;
    }
}

float Heightmap::getHeightAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return vertices[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

float Heightmap::getSlopeAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return normals[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

void Heightmap::loadHeightmap(string filename, float strength) {
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
            //assert(0.0f <= getHeightAt(column, row) && getHeightAt(column, row) <= strength);
        }
    }
    
    calculateNormals();
}

void Heightmap::calculateNormals() {
    float sum = 0.0f;
    for(vec3 vertex : vertices) {
        sum += vertex.y;
    }
    averageHeight = sum / vertices.size();
    Mesh::calculateNormals();
}

float Heightmap::getMinHeight() {
    return minHeight;
}

float Heightmap::getMaxHeight() {
    return maxHeight;
}

float Heightmap::getAverageHeight() {
    return averageHeight;
}