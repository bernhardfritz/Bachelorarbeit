//
//  Skybox.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 17/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Skybox.hpp"
#include "stb_image.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>

#define CUBE_VERT_FILE "skybox_vs.glsl"
#define CUBE_FRAG_FILE "skybox_fs.glsl"
#define FRONT "front4.png"
#define BACK "back4.png"
#define TOP "top4.png"
#define BOTTOM "bottom4.png"
#define LEFT "left4.png"
#define RIGHT "right4.png"

/* big cube. returns Vertex Array Object */
GLuint Skybox::make_big_cube() {
    float points[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -10.0f,
        -1.0f, -1.0f,  10.0f,
         1.0f, -1.0f, -10.0f,
         1.0f, -1.0f, -10.0f,
        -1.0f, -1.0f,  10.0f,
         1.0f, -1.0f,  10.0f
    };
    
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
                  GL_ARRAY_BUFFER, 3 * 36 * sizeof (GLfloat), &points, GL_STATIC_DRAW);
    
    GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    return vao;
}

/* use stb_image to load an image file into memory, and then into one side of
 a cube-map texture. */
bool Skybox::load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name) {
    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
    
    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load (
                                            file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        fprintf (stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        fprintf (
                 stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
                 );
    }
    
    // copy image data into 'target' side of cube map
    glTexImage2D (
                  side_target,
                  0,
                  GL_RGBA,
                  x,
                  y,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  image_data
                  );
    stbi_image_free(image_data);
    return true;
}

/* load all 6 sides of the cube-map from images, then apply formatting to the
 final texture */
void Skybox::create_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube) {
    // generate a cube-map texture to hold all the sides
    glActiveTexture (GL_TEXTURE0);
    glGenTextures (1, tex_cube);
    
    // load each image and copy into a side of the cube-map texture
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
    // format cube map texture
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Skybox::Skybox() : shaderManager(ShaderManager(CUBE_VERT_FILE, CUBE_FRAG_FILE)) {
    cube_vao = make_big_cube();
    create_cube_map (FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT, &cube_map_texture);
    cube_M_location = glGetUniformLocation (shaderManager.getShaderProgram(), "M");
    cube_V_location = glGetUniformLocation (shaderManager.getShaderProgram(), "V");
    cube_P_location = glGetUniformLocation (shaderManager.getShaderProgram(), "P");
}

GLuint Skybox::get_cube_map_texture() {
    return cube_map_texture;
}

void Skybox::draw(const GLfloat *M, const GLfloat *V, const GLfloat *P) {
    glUseProgram (shaderManager.getShaderProgram());
    glUniformMatrix4fv (cube_M_location, 1, GL_FALSE, M);
    glUniformMatrix4fv (cube_V_location, 1, GL_FALSE, V);
    glUniformMatrix4fv (cube_P_location, 1, GL_FALSE, P);
    
    glDepthMask (GL_FALSE);
    glUseProgram (shaderManager.getShaderProgram());
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, cube_map_texture);
    glBindVertexArray (cube_vao);
    glDrawArrays (GL_TRIANGLES, 0, 36);
    glDepthMask (GL_TRUE);
}