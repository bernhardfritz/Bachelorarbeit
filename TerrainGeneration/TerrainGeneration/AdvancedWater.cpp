//
//  AdvancedWater.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 18/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "AdvancedWater.hpp"
#include "TemporaryHeightmap.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

AdvancedWater::AdvancedWater(Heightmap& heightmap) : Heightmap(heightmap.getColumns(), heightmap.getRows()), heightmap(heightmap) {
    dx = 1.0f / columns;
    dy = 1.0f / rows;
    L = (dx + dy) / 2.0f;
    A = pow(L, 2) * pi<float>();
    f = new Direction[(columns + 1) * (rows + 1)];
    d = new float[(columns + 1) * (rows + 1)];
    
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            float* d_xy = &d[row*(columns+1)+column];
            (*d_xy) = 0.001f;
            setHeightAt(column, row, heightmap.getHeightAt(column, row) + (*d_xy) - 0.001f);
            Direction* f_xy = &f[row * (columns+1) + column];
            f_xy->t = 0.0f;
            f_xy->b = 0.0f;
            f_xy->l = 0.0f;
            f_xy->r = 0.0f;
        }
    }
    
    material.setSpecularReflectance(1.0f);
    material.setShininess(1000.0f);
    material.setAmbientReflectance(0.67f);
}

AdvancedWater::~AdvancedWater() {
    delete [] f;
    delete [] d;
}

void AdvancedWater::step(float dt) {
    for(int i = 0; i < 10; i++) {
        Direction dh[(columns+1) * (rows+1)];
        for(int row = 1; row <= rows-1; row++) {
            for(int column = 1; column <= columns-1; column++) {
                float h_xy = heightmap.getHeightAt(column, row);
                float* d_xy = &d[row*(columns+1)+column];
                
                Direction* dh_xy = &dh[(row-1)*(columns+1)+column];
                dh_xy->t = h_xy + (*d_xy) - heightmap.getHeightAt(column, row - 1) - d[(row - 1) * (columns + 1) + column];
                dh_xy->b = h_xy + (*d_xy) - heightmap.getHeightAt(column, row + 1) - d[(row + 1) * (columns + 1) + column];
                dh_xy->l = h_xy + (*d_xy) - heightmap.getHeightAt(column - 1, row) - d[row * (columns + 1) + column - 1];
                dh_xy->r = h_xy + (*d_xy) - heightmap.getHeightAt(column + 1, row) - d[row * (columns + 1) + column + 1];
                
                Direction* f_xy = &f[row * (columns+1) + column];
                f_xy->t = glm::max(0.0f, f_xy->t + dt * A * ((g * dh_xy->t)/L));
                f_xy->b = glm::max(0.0f, f_xy->b + dt * A * ((g * dh_xy->b)/L));
                f_xy->l = glm::max(0.0f, f_xy->l + dt * A * ((g * dh_xy->l))/L);
                f_xy->r = glm::max(0.0f, f_xy->r + dt * A * ((g * dh_xy->r))/L);
                
                float K = glm::min(1.0f, ((*d_xy) * dx * dy) / (f_xy->t + f_xy->b + f_xy->l + f_xy->r) * dt);
                
                f_xy->t *= K;
                f_xy->b *= K;
                f_xy->l *= K;
                f_xy->r *= K;
            }
        }
        for(int row = 1; row <= rows-1; row++) {
            for(int column = 1; column <= columns-1; column++) {
                float fin = f[(row-1)*(columns+1)+column].b + f[(row+1)*(columns+1)+column].t + f[row*(columns+1)+column-1].r + f[row*(columns+1)+column+1].l;
                Direction* f_xy = &f[row * (columns+1) + column];
                float fout = f_xy->t + f_xy->b + f_xy->l + f_xy->r;
                float dV_xy = dt * (fin - fout);
                
                float* d_xy = &d[row*(columns+1)+column];
                (*d_xy) += (dV_xy / (dx * dy));
                
                setHeightAt(column, row, heightmap.getHeightAt(column, row) + (*d_xy) - 0.001f);
            }
        }
    }
    Mesh::calculateNormals();
}