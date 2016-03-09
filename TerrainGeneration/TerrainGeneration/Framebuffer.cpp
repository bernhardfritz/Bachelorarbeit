//
//  Framebuffer.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 04/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(int width, int height) {
    this->width = width;
    this->height = height;
    
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    fbo = 0;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // The texture we're going to render to
    glGenTextures(1, &colorTexture);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture, 0);
    
    // The depth buffer
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer is not ok!\n");
}

int Framebuffer::getWidth() {
    return width;
}

int Framebuffer::getHeight() {
    return height;
}

Framebuffer::~Framebuffer() {
    glDeleteTextures(1, &colorTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteFramebuffers(1, &fbo);
}

GLuint Framebuffer::getColorTexture() {
    return colorTexture;
}

GLuint Framebuffer::getDepthTexture() {
    return depthTexture;
}

void Framebuffer::resize(int width, int height) {
    this->width = width;
    this->height = height;
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}

void Framebuffer::bind() {
    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::draw() {
    // Render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Create and compile our GLSL program from the shaders
    static ShaderManager shaderManager("brightpass_vs.glsl","brightpass_fs.glsl");
    glUseProgram (shaderManager.getShaderProgram());
    
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    GLuint texID = glGetUniformLocation(shaderManager.getShaderProgram(), "colorTexture");
    glUniform1i(texID, 11);
    
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    GLuint depID = glGetUniformLocation(shaderManager.getShaderProgram(), "depthTexture");
    glUniform1i(depID, 12);
    
    quad.draw();
}