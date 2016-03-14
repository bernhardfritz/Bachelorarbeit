//
//  TemporaryHeightmap.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 13/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef TemporaryHeightmap_hpp
#define TemporaryHeightmap_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class TemporaryHeightmap {
private:
    int columns;
    int rows;
    vector<float> heights;
    
public:
    TemporaryHeightmap(int columns, int rows, float h = 0.0f);
    
    void setHeightAt(int column, int row, float height);
    float getHeightAt(int column, int row);
};

#endif /* TemporaryHeightmap_hpp */
