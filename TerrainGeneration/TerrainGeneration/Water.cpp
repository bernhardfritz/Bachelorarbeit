//
//  Water.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Water.hpp"

Water::Water(int columns, int rows, float waveLevel, float waveWidth, float waveHeight) : Heightmap(columns, rows) {
    setWaveLevel(waveLevel);
    setWaveWidth(waveWidth);
    setWaveHeight(waveHeight);
    setWaveTime(0.0f);
    getMaterial()->setSpecularReflectance(1.0f);
    getMaterial()->setShininess(1000.0f);
    getMaterial()->setAmbientReflectance(0.67f);
    step(0.0f);
}

void Water::setWaveLevel(float waveLevel) {
    this->waveLevel = waveLevel;
}

float Water::getWaveLevel() {
    return waveLevel;
}

void Water::setWaveWidth(float waveWidth) {
    this->waveWidth = waveWidth;
}

float Water::getWaveWidth() {
    return waveWidth;
}

void Water::setWaveHeight(float waveHeight) {
    this->waveHeight = waveHeight;
}

float Water::getWaveHeight() {
    return waveHeight;
}

void Water::setWaveTime(float waveTime) {
    this->waveTime = waveTime;
}

float Water::getWaveTime() {
    return waveTime;
}

void Water::step(float delta) {
    waveTime += delta;
    for(int row = 0; row <= rows; row++) {
        for(int column = 0; column <= columns; column++) {
            float height = sin(waveWidth * column + waveTime) * cos(waveWidth * row + waveTime) * waveHeight;
            setHeightAt(column, row, waveLevel + height);
        }
    }
    calculateNormals();
}