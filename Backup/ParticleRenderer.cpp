//
//  ParticleRenderer.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 27/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "ParticleRenderer.hpp"
#include "ShaderManager.hpp"
#include "TextureLoader.hpp"

const GLfloat ParticleRenderer::g_vertex_buffer_data[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
};

ParticleRenderer::ParticleRenderer() : texture(tl.loadTexture("particle.png")) {
    g_particule_position_size_data = new GLfloat[MaxParticles * 4];
    g_particule_color_data = new GLubyte[MaxParticles * 4];
    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    
    texture.assignToSlot(8);
}

ParticleRenderer::~ParticleRenderer() {
    delete [] g_particule_position_size_data;
    delete [] g_particule_color_data;
    
    glDeleteBuffers(1, &billboard_vertex_buffer);
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &particles_color_buffer);
}

void ParticleRenderer::update(vector<PARTICLE>& particles) {
    static VEC3F cyan(0.0f, 0.5f, 1.0f);
    static VEC3F yellow(1.0f, 1.0f, 0.0f);
    for(int i = 0; i < particles.size(); i++) {
        PARTICLE& particle = particles[i];
        
        g_particule_position_size_data[4 * i + 0] = particle.position().x;
        g_particule_position_size_data[4 * i + 1] = particle.position().y;
        g_particule_position_size_data[4 * i + 2] = particle.position().z;
        
        g_particule_position_size_data[4 * i + 3] = PARTICLE_DRAW_RADIUS;
        
        g_particule_color_data[4 * i + 0] = particle.solid() ? yellow.x : cyan.x;
        g_particule_color_data[4 * i + 1] = particle.solid() ? yellow.y : cyan.y;
        g_particule_color_data[4 * i + 2] = particle.solid() ? yellow.z : cyan.z;
        g_particule_color_data[4 * i + 3] = 1.0f;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(GLfloat) * 4, g_particule_position_size_data);
    
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(GLubyte) * 4, g_particule_color_data);
}

void ParticleRenderer::draw(vector<PARTICLE>& particles, const GLfloat *view, const GLfloat *proj) {
    static ShaderManager shaderManager("particle_vs.glsl", "particle_fs.glsl");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shaderManager.getShaderProgram());
    
    GLint TextureID = glGetUniformLocation(shaderManager.getShaderProgram(), "myTextureSampler");
    glUniform1i(TextureID, 8);
    
    int viewLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "view_mat");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view);
    
    int projLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "projection_mat");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, proj);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
    
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (int)particles.size());
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    
    glDisable(GL_BLEND);
}

