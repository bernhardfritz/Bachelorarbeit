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

using namespace std;
using namespace glm;

class Mesh {
private:
    vector<vec3> vertices;
    vector<unsigned int> indices;
    vector<vec3> normals;
    GLuint vao;
    GLuint vbo;
    GLuint nbo;
    GLuint ibo;
    vec3 ambientReflectance;
    vec3 diffuseReflectance;
    vec3 specularReflectance;
    float shininess;
    
public:
    Mesh(vector<vec3> vertices, vector<unsigned int> indices);
    Mesh(vector<vec3> vertices, vector<unsigned int> indices, vector<vec3> normals);
    
    void setVertices(vector<vec3> vertices);
    vector<vec3> getVertices();
    void setNormals(vector<vec3> normals);
    vector<vec3> getNormals();
    void setIndices(vector<unsigned int> indices);
    vector<unsigned int> getIndices();
    void setAmbientReflectance(vec3 ambientReflectance);
    vec3 getAmbientReflectance();
    void setDiffuseReflectance(vec3 diffuseReflectance);
    vec3 getDiffuseReflectance();
    void setSpecularReflectance(vec3 specularReflectance);
    vec3 getSpecularReflectance();
    void setShininess(float shininess);
    float getShininess();
    void setDefaultMaterial();
    void calculateNormals();
    void init();
    void draw();
    void print();
};

#endif /* Mesh_hpp */
