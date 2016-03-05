//
//  Framebuffer.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 04/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include <stdio.h>
#include "ShaderManager.hpp"
#include "Quad.hpp"

class Framebuffer {
private:
    int width, height;
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthTexture;
    Quad quad;
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    
    int getWidth();
    int getHeight();
    
    GLuint getColorTexture();
    GLuint getDepthTexture();
    
    void resize(int width, int height);
    void bind();
    void unbind();
    void draw();
};

#endif /* Framebuffer_hpp */
