//
//  DiamondSquare.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 19/12/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include "DiamondSquare.hpp"
void DiamondSquare::diamondStep(Heightmap& heightmap, float roughness, int left, int right, int top, int bottom) {
    int midpointColumn = left + (right - left) / 2;
    int midpointRow = top + (bottom - top) / 2;
    float avg = average({
        heightmap.getHeightAt(left, top),
        heightmap.getHeightAt(right, top),
        heightmap.getHeightAt(left, bottom),
        heightmap.getHeightAt(right, bottom)
    });
    float r = roughness * (2 * drand48() - 1); // r is between -roughness and roughness;
    heightmap.setHeightAt(midpointColumn, midpointRow, avg + r);
}

void DiamondSquare::squareStep(Heightmap& heightmap, float roughness, int left, int right, int top, int bottom) {
    int midpointColumn = left + (right - left) / 2;
    int midpointRow = top + (bottom - top) / 2;
    
    vector<float> vecTop;
    if(top > 0) vecTop.push_back(heightmap.getHeightAt(midpointColumn, top - (bottom - top) / 2));
    vecTop.push_back(heightmap.getHeightAt(left, top));
    vecTop.push_back(heightmap.getHeightAt(right, top));
    vecTop.push_back(heightmap.getHeightAt(midpointColumn, midpointRow));
    float avgTop = average(vecTop);
    float r = roughness * (2 * drand48() - 1); // r is between -roughness and roughness;
    heightmap.setHeightAt(midpointColumn, top, avgTop + r);
    
    vector<float> vecLeft;
    vecLeft.push_back(heightmap.getHeightAt(left, top));
    if(left > 0) vecLeft.push_back(heightmap.getHeightAt(left - (right - left) / 2, midpointRow));
    vecLeft.push_back(heightmap.getHeightAt(midpointColumn, midpointRow));
    vecLeft.push_back(heightmap.getHeightAt(left, bottom));
    float avgLeft = average(vecLeft);
    r = roughness * (2 * drand48() - 1); // r is between -roughness and roughness;
    heightmap.setHeightAt(left, midpointRow, avgLeft + r);
    
    vector<float> vecRight;
    vecRight.push_back(heightmap.getHeightAt(right, top));
    vecRight.push_back(heightmap.getHeightAt(midpointColumn, midpointRow));
    if(right < heightmap.getColumns()) vecRight.push_back(heightmap.getHeightAt(right + (right - left) / 2, midpointRow));
    vecRight.push_back(heightmap.getHeightAt(right, bottom));
    float avgRight = average(vecRight);
    r = roughness * (2 * drand48() - 1); // r is between -roughness and roughness;
    heightmap.setHeightAt(right, midpointRow, avgRight + r);
    
    vector<float> vecBottom;
    vecBottom.push_back(heightmap.getHeightAt(midpointColumn, midpointRow));
    vecBottom.push_back(heightmap.getHeightAt(left, bottom));
    vecBottom.push_back(heightmap.getHeightAt(right, bottom));
    if(bottom < heightmap.getRows()) vecBottom.push_back(heightmap.getHeightAt(midpointColumn, bottom + (bottom - top) / 2 ));
    float avgBottom = average(vecBottom);
    r = roughness * (2 * drand48() - 1); // r is between -roughness and roughness;
    heightmap.setHeightAt(midpointColumn, bottom, avgBottom + r);
}

void DiamondSquare::diamondSquare(Heightmap &heightmap, float roughness, int left, int right, int top, int bottom) {
    DiamondSquare::diamondStep(heightmap, roughness, left, right, top, bottom);
    DiamondSquare::squareStep(heightmap, roughness, left, right, top, bottom);
}

/**/

float DiamondSquare::average(vector<float> values) {
    float sum = 0.0f;
    for (int i = 0; i < values.size(); i++) {
        sum += values[i];
    }
    return sum / values.size();
}

void DiamondSquare::perform(Heightmap& heightmap, float roughness) {
    heightmap.setHeightAt(0, 0, roughness);
    heightmap.setHeightAt(heightmap.getColumns(), 0, roughness);
    heightmap.setHeightAt(0, heightmap.getRows(), roughness);
    heightmap.setHeightAt(heightmap.getColumns(), heightmap.getRows(), roughness);

    int columns = heightmap.getColumns();
    int rows = heightmap.getRows();
    
    while(columns > 1) {
        for(int row = 0; row < heightmap.getRows(); row += rows) {
            for(int column = 0; column < heightmap.getColumns(); column += columns) {
                DiamondSquare::diamondSquare(heightmap, roughness, column, column + columns, row, row + rows);
            }
        }
        roughness/=2.0f;
        columns/=2.0f;
        rows/=2.0f;
    }

    heightmap.init();
}
