//
//  Quad.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 05/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Quad_hpp
#define Quad_hpp

#include <stdio.h>
#include <GL/glew.h>

class Quad {
private:
    GLuint vao;
public:
    Quad();
    void draw();
};

#endif /* Quad_hpp */
