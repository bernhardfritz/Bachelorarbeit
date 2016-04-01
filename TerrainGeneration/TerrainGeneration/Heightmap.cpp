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

void Heightmap::changeHeightAt(int column, int row, float delta) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        setHeightAt(column, row, vertices[row * (columns + 1) + column].y + delta);
    }
}

float Heightmap::getHeightAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return vertices[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

#define EPSILON 0.000001

int triangle_intersection( const vec3   V1,  // Triangle vertices
                          const vec3   V2,
                          const vec3   V3,
                          const vec3    O,  //Ray origin
                          const vec3    D,  //Ray direction
                          float* out )
{
    vec3 e1, e2;  //Edge1, Edge2
    vec3 P, Q, T;
    float det, inv_det, u, v;
    float t;
    
    //Find vectors for two edges sharing V1
    e1 = V2 - V1;
    e2 = V3 - V1;
    //Begin calculating determinant - also used to calculate u parameter
    P = cross(D, e2);
    //if determinant is near zero, ray lies in plane of triangle
    det = dot(e1, P);
    //NOT CULLING
    if(det > -EPSILON && det < EPSILON) return 0;
    inv_det = 1.f / det;
    
    //calculate distance from V1 to ray origin
    T = O - V1;
    
    //Calculate u parameter and test bound
    u = dot(T, P) * inv_det;
    //The intersection lies outside of the triangle
    if(u < 0.f || u > 1.f) return 0;
    
    //Prepare to test v parameter
    Q = cross(T, e1);
    
    //Calculate V parameter and test bound
    v = dot(D, Q) * inv_det;
    //The intersection lies outside of the triangle
    if(v < 0.f || u + v  > 1.f) return 0;
    
    t = dot(e2, Q) * inv_det;
    
    if(t > EPSILON) { //ray intersection
        *out = t;
        return 1;
    }
    
    // No hit, no win
    return 0;
}

float Heightmap::getInterpolatedHeightAt(float x, float z) {
    if(0.0f <= x && x <= 1.0f && 0.0f <= z && z <= 1.0f) {
        int col1 = floor<int>(x * (float)(columns + 1));
        int col2 = col1 + 1;
        float dx = x * (float)(columns + 1) - (float)col1;
        int row1 = floor<int>(z * (float)(rows + 1));
        int row2 = row1 + 1;
        float dz = z * (float)(rows + 1) - (float)row1;
        float x1 = x - dx;
        float x2 = x1 + 1.0f;
        float z1 = z - dz;
        float z2 = z1 + 1.0f;
        float height = 0.0f;
        if(dx + dz < 1) {
            triangle_intersection(vec3(x1, getHeightAt(col1, row1), z1), vec3(x1, getHeightAt(col1, row2), z2), vec3(x2, getHeightAt(col2, row1), z1), vec3(x, 0, z), vec3(0, 1, 0), &height);
        } else {
            triangle_intersection(vec3(x2, getHeightAt(col2, row1), z1), vec3(x1, getHeightAt(col1, row2), z2), vec3(x2, getHeightAt(col2, row2), z2), vec3(x, 0, z), vec3(0, 1, 0), &height);
        }
        return height;
    }
    return 0.0f;
}

float Heightmap::getSlopeAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return normals[row * (columns + 1) + column].y;
    }
    return 0.0f;
}

vec3 Heightmap::getNormalAt(int column, int row) {
    if(0 <= column && column <= columns && 0 <= row && row <= rows) {
        return normals[row * (columns + 1) + column];
    }
    return vec3();
};

vec3 Heightmap::getInterpolatedNormalAt(float x, float z) {
    if(0.0f <= x && x <= 1.0f && 0.0f <= z && z <= 1.0f) {
        int col1 = floor<int>(x * (float)(columns + 1));
        int col2 = col1 + 1;
        int row1 = floor<int>(z * (float)(rows + 1));
        int row2 = row1 + 1;
        vec3 n1 = getNormalAt(col1, row1);
        vec3 n2 = getNormalAt(col2, row1);
        vec3 n3 = getNormalAt(col1, row2);
        vec3 n4 = getNormalAt(col2, row2);
        return normalize(n1 + n2 + n3 + n4);
    }
    return vec3();
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