//
//  MetaMesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 30/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef MetaMesh_hpp
#define MetaMesh_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class MetaMesh {
private:
    vector<vec3> vertices;
    vector<vec3> normals;
    GLuint vbo;
    GLuint nbo;
    GLuint vao;
    
    void init();
public:
    MetaMesh();
    void clear();
    void addVertex(vec3 vertex);
    void addNormal(vec3 normal);
    void update();
    void draw();
};

#endif /* MetaMesh_hpp */
