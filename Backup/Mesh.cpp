//
//  Mesh.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 07/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "Mesh.hpp"

void Mesh::setVertices(vector<vec3> vertices) {
    this->vertices = vertices;
}

vector<vec3> Mesh::getVertices() {
    return vertices;
}

void Mesh::setNormals(vector<vec3> normals) {
    this->normals = normals;
}

vector<vec3> Mesh::getNormals() {
    return normals;
}

void Mesh::init() {
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    nbo = 0;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Mesh::print() {
    cout << "vertices:" << endl;
    for(int i = 0; i < vertices.size(); i++) {
        cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
    }
    cout << "normals:" << endl;
    for(int i = 0; i < normals.size(); i++) {
        cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << endl;
    }
}