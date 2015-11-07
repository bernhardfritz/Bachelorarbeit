//
//  FlatMesh.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 31/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "FlatMesh.hpp"

FlatMesh::FlatMesh(vector<vec3> vertices) {
    setVertices(vertices);
    calculateNormals();
    init();
}

FlatMesh::FlatMesh(vector<vec3> vertices, vector<vec3> normals) {
    setVertices(vertices);
    setNormals(normals);
    init();
}

void FlatMesh::calculateNormals() {
    vector<vec3> temporaryNormals;
    for(int i = 0; i < vertices.size(); i += 3) {
        vec3 v0 = vertices[i] - vertices[i+2];
        vec3 v1 = vertices[i+1] - vertices[i+2];
        vec3 normal = normalize(cross(v0 , v1));
        temporaryNormals.push_back(normal);
        temporaryNormals.push_back(normal);
        temporaryNormals.push_back(normal);
    }
    normals = temporaryNormals;
}

void FlatMesh::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, (unsigned int)vertices.size());
}