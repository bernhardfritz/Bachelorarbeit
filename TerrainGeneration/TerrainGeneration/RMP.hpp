//
//  RMP.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/01/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef RMP_hpp
#define RMP_hpp

#include <stdio.h>
#include "Heightmap.hpp"

class RMP {
private:
public:
    static void perform(Heightmap& heightmap, int n, int l, int r);
    static void perform(Heightmap& heightmap, int n = 1);
    static void perform(Heightmap& heightmap, int x, int z, int spread, float delta, int iterations = 1);
    static void perform(Heightmap& heightmap, int x0, int z0, int x1, int z1, int spread, float delta, int iterations = 1);
};

#endif /* RMP_hpp */
