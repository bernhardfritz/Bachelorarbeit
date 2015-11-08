//
//  SmoothMesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef SmoothMesh_hpp
#define SmoothMesh_hpp

#include "Mesh.hpp"

using namespace std;
using namespace glm;

class SmoothMesh : public Mesh{
private:
    vector<unsigned int> indices;
    GLuint ibo;
    
public:
    SmoothMesh(vector<vec3> vertices, vector<unsigned int> indices);
    SmoothMesh(vector<vec3> vertices, vector<unsigned int> indices, vector<vec3> normals);
    
    void setIndices(vector<unsigned int> indices);
    vector<unsigned int> getIndices();
    void calculateNormals();
    void init();
    void draw();
    void print();
};

#endif /* SmoothMesh_hpp */
