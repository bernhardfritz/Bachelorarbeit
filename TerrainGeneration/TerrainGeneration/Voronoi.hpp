//
//  Voronoi.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 18/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Voronoi_hpp
#define Voronoi_hpp

#include "Cone.hpp"
#include "Camera.hpp"
#include "ShaderManager.hpp"

class Voronoi {
private:
    //vector<Cone*> cones;
    vector<vec3> positions;
    unsigned char* buffer = NULL;
    Camera camera;
    mat4 view;
public:
    Voronoi(int n);
    ~Voronoi();
    void draw();
    bool isPixelInRegion(int px, int py, int rx, int ry);
    bool isPositionInRegion(int pcolumn, int prow, int rcolumn, int rrow, int columns, int rows);
    vec3 getColorAtPixel(int x, int y);
    vec3 getColorAtPosition(int column, int row, int columns, int rows);
};

#endif /* Voronoi_hpp */
