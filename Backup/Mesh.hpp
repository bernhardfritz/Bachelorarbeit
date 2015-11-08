//
//  Mesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 07/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h> // include GLEW and new version of GL on Windows

using namespace std;
using namespace glm;

class Mesh {
protected:
    vector<vec3> vertices;
    vector<vec3> normals;
    GLuint vao;
    GLuint vbo;
    GLuint nbo;
    
public:
    void setVertices(vector<vec3> vertices);
    vector<vec3> getVertices();
    void setNormals(vector<vec3> normals);
    vector<vec3> getNormals();
    virtual void calculateNormals() = 0;
    virtual void init();
    virtual void draw() = 0;
    virtual void print();
    virtual ~Mesh() {}
};

#endif /* Mesh_hpp */
