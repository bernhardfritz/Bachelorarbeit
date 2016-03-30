//
//  ParticleRenderer.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 27/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef ParticleRenderer_hpp
#define ParticleRenderer_hpp

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include "PARTICLE.h"
#include "TextureLoader.hpp"
#include "Texture.hpp"

class ParticleRenderer {
private:
    GLfloat* g_particule_position_size_data;
    GLubyte* g_particule_color_data;
    
    static const GLfloat g_vertex_buffer_data[];
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;
    
    const int MaxParticles = 100000;
    
    TextureLoader tl;
    Texture texture;
public:
    ParticleRenderer();
    ~ParticleRenderer();
    void update(vector<PARTICLE>& particles);
    void draw(vector<PARTICLE>& particles, const GLfloat *view, const GLfloat *proj);
};

#endif /* ParticleRenderer_hpp */
