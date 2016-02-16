//
//  TextureLoader.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 16/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "TextureLoader.hpp"
#include "stb_image.h"

Texture TextureLoader::loadTexture(string filename, int forceChannels) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, forceChannels);
    
    if(!data) {
        fprintf(stderr, "ERROR: could not load %s\n", filename.c_str());
    }
    
    if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0) {
        fprintf(stderr, "WARNING: texture %s is not power-of-2 dimensions\n", filename.c_str());
    }
    
    // flip upside-down start
    int widthInBytes = width * channels;
    unsigned char *top = NULL;
    unsigned char *bottom = NULL;
    unsigned char temp = 0;
    int halfHeight = height / 2;
    
    for (int row = 0; row < halfHeight; row++) {
        top = data + row * widthInBytes;
        bottom = data + (height - row - 1) * widthInBytes;
        for (int col = 0; col < widthInBytes; col++) {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }
    // flip upside-down end
    
    return Texture(data, width, height, channels);
}