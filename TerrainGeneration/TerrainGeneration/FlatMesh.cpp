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
}

FlatMesh::FlatMesh(vector<vec3> vertices, vector<vec3> normals) {
    setVertices(vertices);
    setNormals(normals);
}

void FlatMesh::setVertices(vector<vec3> vertices) {
    this->vertices = vertices;
}

vector<vec3> FlatMesh::getVertices() {
    return vertices;
}

void FlatMesh::setNormals(vector<vec3> normals) {
    this->normals = normals;
}

vector<vec3> FlatMesh::getNormals() {
    return normals;
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

void FlatMesh::print() {
    cout << "vertices:" << endl;
    for(int i = 0; i < vertices.size(); i++) {
        cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
    }
    cout << "normals:" << endl;
    for(int i = 0; i < normals.size(); i++) {
        cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
    }
}