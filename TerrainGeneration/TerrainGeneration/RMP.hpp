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
};

#endif /* RMP_hpp */
