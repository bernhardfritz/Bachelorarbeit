//
//  Mesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include "Material.hpp"

using namespace std;
using namespace glm;

class Mesh {
protected:
    vector<vec3> vertices;
    vector<unsigned int> indices;
    vector<vec3> normals;
    vector<vec2> texcoords;
    
private:
    GLuint vao;
    GLuint vbo;
    GLuint nbo;
    GLuint tbo;
    GLuint ibo;
    Material* material;
    
public:
    Mesh();
    Mesh(vector<vec3> vertices, vector<unsigned int> indices);
    Mesh(vector<vec3> vertices, vector<unsigned int> indices, vector<vec3> normals);
    
    void setVertices(vector<vec3> vertices);
    vector<vec3> getVertices();
    void setNormals(vector<vec3> normals);
    vector<vec3> getNormals();
    void setTexcoords(vector<vec2> texcoords);
    vector<vec2> getTexcoords();
    bool isTextured();
    void setIndices(vector<unsigned int> indices);
    vector<unsigned int> getIndices();
    void setMaterial(Material* material);
    Material* getMaterial();
    GLuint getVAO();
    void calculateNormals();
    void init();
    void translate(float x, float y, float z);
    void draw();
    void print();
};

#endif /* Mesh_hpp */
