//
//  ObjLoader.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 30/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <glm/glm.hpp>

#include "ObjLoader.hpp"

using namespace std;
using namespace glm;

string ObjLoader::readFile(string filename) {
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

vector<string> ObjLoader::split(string s, char c) {
    stringstream ss(s);
    string sub;
    vector<string> v;
    while(getline(ss, sub, c)) {
        v.push_back(sub);
    }
    return v;
}

FlatMesh ObjLoader::loadFlatMesh(string filename) {
    string str = readFile(filename);
    stringstream ss(str);
    regex v("^v\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)");
    regex vn("^vn\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)");
    regex f("^f\\s+(\\d+)\\/(\\d*)\\/(\\d+)\\s+(\\d+)\\/(\\d*)\\/(\\d+)\\s+(\\d+)\\/(\\d*)\\/(\\d+)");
    smatch match;
    vector<string> lines = split(str, '\n');
    vector<vec3> indexedVertices;
    vector<vec3> indexedNormals;
    vector<unsigned int> vertexIndices;
    vector<unsigned int> normalIndices;
    for(int i = 0; i < lines.size(); i++) {
        if(regex_search(lines[i], match, v)) {
            indexedVertices.push_back(vec3(stof(match[1].str()), stof(match[2].str()), stof(match[3].str())));
        } else if(regex_search(lines[i], match, vn)) {
            indexedNormals.push_back(vec3(stof(match[1].str()), stof(match[2].str()), stof(match[3].str())));
        } else if(regex_search(lines[i], match, f)) {
            vertexIndices.push_back(stoi(match[1].str())-1);
            vertexIndices.push_back(stoi(match[4].str())-1);
            vertexIndices.push_back(stoi(match[7].str())-1);
            normalIndices.push_back(stoi(match[3].str())-1);
            normalIndices.push_back(stoi(match[6].str())-1);
            normalIndices.push_back(stoi(match[9].str())-1);
        } else {
            cout << "no match: " << lines[i] << endl;
        }
    }
    vector<vec3> vertices;
    vector<vec3> normals;
    for(int i = 0; i < vertexIndices.size(); i++) {
        vertices.push_back(indexedVertices[vertexIndices[i]]);
    }
    for(int i = 0; i < normalIndices.size(); i++) {
        normals.push_back(indexedNormals[normalIndices[i]]);
    }
    FlatMesh mesh(vertices, normals);
    return mesh;
}

SmoothMesh ObjLoader::loadSmoothMesh(string filename) {
    string str = readFile(filename);
    stringstream ss(str);
    regex v("^v\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)");
    regex vn("^vn\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)");
    regex f("^f\\s+(\\d+)\\/(\\d*)\\/(\\d+)\\s+(\\d+)\\/(\\d*)\\/(\\d+)\\s+(\\d+)\\/(\\d*)\\/(\\d+)");
    smatch match;
    vector<string> lines = split(str, '\n');
    vector<vec3> indexedVertices;
    vector<vec3> indexedNormals;
    vector<unsigned int> vertexIndices;
    vector<unsigned int> normalIndices;
    for(int i = 0; i < lines.size(); i++) {
        if(regex_search(lines[i], match, v)) {
            indexedVertices.push_back(vec3(stof(match[1].str()), stof(match[2].str()), stof(match[3].str())));
        } else if(regex_search(lines[i], match, vn)) {
            indexedNormals.push_back(vec3(stof(match[1].str()), stof(match[2].str()), stof(match[3].str())));
        } else if(regex_search(lines[i], match, f)) {
            vertexIndices.push_back(stoi(match[1].str())-1);
            vertexIndices.push_back(stoi(match[4].str())-1);
            vertexIndices.push_back(stoi(match[7].str())-1);
            normalIndices.push_back(stoi(match[3].str())-1);
            normalIndices.push_back(stoi(match[6].str())-1);
            normalIndices.push_back(stoi(match[9].str())-1);
        } else {
            cout << "no match: " << lines[i] << endl;
        }
    }
    vector<vec3> vertices = indexedVertices;
    vector<unsigned int> indices = vertexIndices;
    // indexedNormals.size() can be smaller than indexedVertices.size() because some vertices might share the same normal
    // opengl only supports one index. this is why i have to expand the normals vector to the same size as the vertices vector by duplicating some normals
    vector<vec3> normals;
    for(int i = 0; i < vertices.size(); i++) {
        normals.push_back(vec3(0.0, 0.0, 0.0));
    }
    for(int i = 0; i < indices.size(); i++) {
        cout << "normal[" << indices[i] << "] = " << indexedNormals[normalIndices[i]].x << " " << indexedNormals[normalIndices[i]].y << " " << indexedNormals[normalIndices[i]].z << endl;
        normals[indices[i]] = indexedNormals[normalIndices[i]];
    }
    SmoothMesh mesh(vertices, indices, normals);
    return mesh;
}