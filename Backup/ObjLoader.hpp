//
//  ObjLoader.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 30/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "FlatMesh.hpp"
#include "SmoothMesh.hpp"

using namespace std;

class ObjLoader {
private:
    string readFile(string filename);
    vector<string> split(string s, char c);
public:
    FlatMesh loadFlatMesh(string filename);
    SmoothMesh loadSmoothMesh(string filename);
};

#endif /* ObjLoader_hpp */
