//
//  SmoothMesh.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef SmoothMesh_hpp
#define SmoothMesh_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class SmoothMesh {
private:
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<unsigned int> indices;
    
public:
    SmoothMesh(vector<vec3> vertices, vector<unsigned int> indices);
    SmoothMesh(vector<vec3> vertices, vector<unsigned int> indices, vector<vec3> normals);
    
    void setVertices(vector<vec3> vertices);
    vector<vec3> getVertices();
    void setIndices(vector<unsigned int> indices);
    vector<unsigned int> getIndices();
    void setNormals(vector<vec3> normals);
    vector<vec3> getNormals();
    void calculateNormals();
    void print();
};

#endif /* SmoothMesh_hpp */
