//
//  Texture.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 16/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Texture.hpp"
#include "stb_image.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows

Texture::Texture(unsigned char* data, int width, int height, int channels) {
    this->data = data;
    this->width = width;
    this->height = height;
    this->channels = channels;
}

Texture::~Texture() {
    stbi_image_free(data);
}

unsigned char* Texture::getData() {
    return data;
}

int Texture::getWidth() {
    return width;
}

int Texture::getHeight() {
    return height;
}

int Texture::getChannels() {
    return channels;
}

void Texture::assignToSlot(int slot) {
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}