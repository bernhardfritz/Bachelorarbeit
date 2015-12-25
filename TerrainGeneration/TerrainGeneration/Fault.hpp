//
//  Fault.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 25/12/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Fault_hpp
#define Fault_hpp

#include <stdio.h>
#include "Heightmap.hpp"

class Fault {
private:
    
public:
    Fault();
    
    static void perform(Heightmap& heightmap, float roughness, int iterations);
};

#endif /* Fault_hpp */
