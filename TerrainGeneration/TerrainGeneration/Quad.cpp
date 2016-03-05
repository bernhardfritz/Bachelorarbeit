//
//  Quad.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Quad.hpp"

Quad::Quad() {
    static const GLfloat points[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };
    
    GLuint vbo = 0;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
                  GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Quad::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}