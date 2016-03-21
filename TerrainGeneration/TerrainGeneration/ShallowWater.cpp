//
//  ShallowWater.cpp
//  TerrainGeneration
//
//  ShallowWater written in Javascript by Trevor Dixon http://jsfiddle.net/trevordixon/DYVfa/
//  Translated to C++ and modified by Bernhard Fritz on 19/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "ShallowWater.hpp"

ShallowWater::ShallowWater(Heightmap& heightmap) : Heightmap(heightmap.getColumns(), heightmap.getRows()), heightmap(heightmap) {
    _h = new float[(columns + 1) * (rows + 1)];
    _u = new float[(columns + 1) * (rows + 1)];
    _v = new float[(columns + 1) * (rows + 1)];
    
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            _h[row * (columns + 1) + column] = H;
            _u[row * (columns + 1) + column] = 0.0f;
            _v[row * (columns + 1) + column] = 0.0f;
            setHeightAt(column, row, H);
        }
    }
    
    Mesh::calculateNormals();
    
    material.setSpecularReflectance(1.0f);
    material.setShininess(1000.0f);
    material.setAmbientReflectance(0.67f);
}

ShallowWater::~ShallowWater() {
    delete [] _h;
    delete [] _u;
    delete [] _v;
}

// returns random int in interval [a,b)
int getRandomInt(int a, int b) {
    return a+(b-a)*(double)rand()/((double)RAND_MAX+1.0);
}

void ShallowWater::rain(float weight, int radius) {
    int row = getRandomInt(radius, rows - radius + 1);
    int column = getRandomInt(radius, columns - radius + 1);
    for(int r = row - radius; r <= row + radius; r++) {
        for(int c = column - radius; c <= column + radius; c++) {
            bool edge = c == 0 || c == columns || r == 0 || r == rows;
            if (!edge) _h[r * (columns + 1) + c] = H + weight;
        }
    }
}

void ShallowWater::step() {
    for(int row = 1; row <= rows - 1; row++) {
        for(int column = 1; column <= columns - 1; column++) {
            float* h = &_h[row * (columns + 1) + column];
            float* u = &_u[row * (columns + 1) + column];
            float* v = &_v[row * (columns + 1) + column];
            
            float hl = _h[row * (columns + 1) + column - 1];
            float hr = _h[row * (columns + 1) + column + 1];
            float ht = _h[(row - 1) * (columns + 1) + column];
            float hb = _h[(row + 1) * (columns + 1) + column];
            float ul = _u[row * (columns + 1) + column - 1];
            float ur = _u[row * (columns + 1) + column + 1];
            float vt = _v[(row - 1) * (columns + 1) + column];
            float vb = _v[(row + 1) * (columns + 1) + column];
            
            (*u) += TIMESTEP * (G * (hl - hr) - (DAMP * (*u)));
            (*v) += TIMESTEP * (G * (ht - hb) - (DAMP * (*v)));
            (*h) += TIMESTEP * (ul * (H + hl) - ur * (H + hr) + vt * (H + ht) - vb * (H + hb));
        }
    }
    
    for(int row = 1; row <= rows - 1; row++) {
        for(int column = 1; column <= columns - 1; column++) {
            float* h = &_h[row * (columns + 1) + column];
            
            float sum = 0.0f;
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    sum += _h[(row+i) * (columns + 1) + (column + j)] * gauss[(i+1) * 3 + (j+1)];
                }
            }
            
            (*h) = sum;
            setHeightAt(column, row, (*h));
        }
    }
    
    Mesh::calculateNormals();
}