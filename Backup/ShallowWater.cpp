//
//  ShallowWater.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 12/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "ShallowWater.hpp"

ShallowWater::ShallowWater(Heightmap& heightmap) : Heightmap(heightmap.getColumns(), heightmap.getRows()), heightmap(heightmap), watermap(TemporaryHeightmap(heightmap.getColumns(), heightmap.getRows())) {
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            float b = heightmap.getHeightAt(column, row);
            float d = 0.002f;
            watermap.setHeightAt(column, row, d);
            setHeightAt(column, row, b + d - 0.001f);
            Flux tmp;
            tmp.fL = 0.0f;
            tmp.fR = 0.0f;
            tmp.fT = 0.0f;
            tmp.fB = 0.0f;
            flux.push_back(tmp);
        }
    }
    material.setSpecularReflectance(1.0f);
    material.setShininess(1000.0f);
    material.setAmbientReflectance(0.67f);
}

void ShallowWater::flow(float dt, int iterations) {
    for(int i = 0; i < iterations; i++) {
        float lX = 1.0f / columns;
        float lY = 1.0f / rows;
        float A = lX * lY;
        float g = 9.81f;
        float l = (lX + lY) / 2.0f;
        for(int row = 0; row <= rows; row++) {
            for(int column = 0; column <= columns; column++) {
                float b = heightmap.getHeightAt(column, row);
                float d = watermap.getHeightAt(column, row);
                float dhL = b + d - heightmap.getHeightAt(column - 1, row) - watermap.getHeightAt(column - 1, row);
                float dhR = b + d - heightmap.getHeightAt(column + 1, row) - watermap.getHeightAt(column + 1, row);
                float dhT = b + d - heightmap.getHeightAt(column, row - 1) - watermap.getHeightAt(column, row - 1);
                float dhB = b + d - heightmap.getHeightAt(column, row + 1) - watermap.getHeightAt(column, row + 1);
                Flux* tmp = &flux[row * columns + column];
                tmp->fL = glm::max(0.0f, tmp->fL + dt * A * ((g * dhL) / l));
                tmp->fR = glm::max(0.0f, tmp->fR + dt * A * ((g * dhR) / l));
                tmp->fT = glm::max(0.0f, tmp->fT + dt * A * ((g * dhT) / l));
                tmp->fB = glm::max(0.0f, tmp->fB + dt * A * ((g * dhB) / l));
                float K = glm::min(1.0f, (d * lX * lY) / (tmp->fL + tmp->fR + tmp->fT + tmp->fB) * dt);
                tmp->fL *= K;
                tmp->fR *= K;
                tmp->fT *= K;
                tmp->fB *= K;
            }
        }
        for(int row = 0; row <= rows; row++) {
            for(int column = 0; column <= columns; column++) {
                float b = heightmap.getHeightAt(column, row);
                float d = watermap.getHeightAt(column, row);
                float finL = 0.0f;
                float finR = 0.0f;
                float finT = 0.0f;
                float finB = 0.0f;
                if(column == 0) {
                    finL = 0.0f;
                    finR = flux[row * columns + column + 1].fL;
                } else if(0 < column && column < columns) {
                    finL = flux[row * columns + column - 1].fR;
                    finR = flux[row * columns + column + 1].fL;
                } else if(column == columns){
                    finL = flux[row * columns + column - 1].fR;
                    finR = 0.0f;
                }
                if(row == 0) {
                    finT = 0.0f;
                    finB = flux[(row + 1) * columns + column].fT;
                } else if(0 < row && row < rows) {
                    finT = flux[(row - 1) * columns + column].fB;
                    finB = flux[(row + 1) * columns + column].fT;
                } else if(row == rows) {
                    finT = flux[(row - 1) * columns + column].fB;
                    finB = 0.0f;
                }
                float fin = finL + finR + finT + finB;
                Flux* tmp = &flux[row * columns + column];
                float fout = tmp->fL + tmp->fR + tmp->fT + tmp->fB;
                float dV = dt * (fin - fout);
                d += dV / (lX * lY);
                if(0.0f <= d && d <= watermap.getHeightAt(column, row) * 2.0f) {
                    watermap.setHeightAt(column, row, d);
                    setHeightAt(column, row, b + d - 0.001f);
                }
            }
        }
    }
    Mesh::calculateNormals();
}

