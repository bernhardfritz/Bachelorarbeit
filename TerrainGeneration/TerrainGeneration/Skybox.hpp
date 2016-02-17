//
//  Skybox.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 17/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Skybox_hpp
#define Skybox_hpp
#include "Cube.hpp"

#include "gl_utils.h"

class Skybox {
private:
    GLuint cube_vao;
    GLuint cube_map_texture;
    GLuint cube_sp;
    int cube_M_location;
    int cube_V_location;
    int cube_P_location;
    
    GLuint make_big_cube();
    bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
    void create_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube);
public:
    Skybox();
    void draw(const GLfloat *M, const GLfloat *V, const GLfloat *P);
};
#endif /* Skybox_hpp */
