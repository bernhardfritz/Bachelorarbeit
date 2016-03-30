//
//  Metaball.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 29/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Metaball_hpp
#define Metaball_hpp

#include <stdio.h>
#include "Mesh.hpp"

class Metaball : public Mesh {
private:
public:
    Metaball();
    void init();
    void update();
};

#endif /* Metaball_hpp */
