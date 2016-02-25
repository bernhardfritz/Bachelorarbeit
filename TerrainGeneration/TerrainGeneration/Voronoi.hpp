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
#include "ShaderManager.hpp"

class Voronoi {
private:
    vector<Cone> cones;
    unsigned char* buffer = NULL;
    bool once = true;
public:
    Voronoi(int n);
    ~Voronoi();
    void draw();
    bool isPixelInRegion(int px, int py, int rx, int ry);
    bool isPositionInRegion(int pcolumn, int prow, int rcolumn, int rrow, int columns, int rows);
};

#endif /* Voronoi_hpp */