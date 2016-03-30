//
//  Heightmap.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 09/11/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Heightmap_hpp
#define Heightmap_hpp

#include <vector>
#include "Mesh.hpp"
#include "Texture.hpp"

using namespace std;

class Heightmap : public Mesh{
protected:
    int columns;
    int rows;
    float minHeight = 0.0f;
    float maxHeight = 0.0f;
    float averageHeight = 0.0f;
public:
    Heightmap() {};
    Heightmap(int columns, int rows);
    
    void setColumns(int columns);
    int getColumns();
    void setRows(int rows);
    int getRows();
    void setHeightAt(int column, int row, float height);
    float getHeightAt(int column, int row);
    float getInterpolatedHeightAt(float x, float z);
    float getSlopeAt(int column, int row);
    vec3 getNormalAt(int column, int row);
    vec3 getInterpolatedNormalAt(float x, float z);
    void loadHeightmap(string filename, float strength);
    void calculateNormals();
    float getMinHeight();
    float getMaxHeight();
    float getAverageHeight();
};

#endif /* Heightmap_hpp */
