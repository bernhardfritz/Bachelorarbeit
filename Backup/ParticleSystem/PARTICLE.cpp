#include "PARTICLE.h"

vec3 red(1,0,0);
vec3 blue(0,0,1);
vec3 black(0,0,0);
vec3 green(0,1,0);
vec3 lightBlueColor(0.01f,0.25f,1.0f);
vec3 purpleColor(0.88f,0.08f,0.88f);

int count = 0;

#define PARTICLE_DRAW_RADIUS 0.015f//0.01 //0.006

bool PARTICLE::isSurfaceVisible = false;
bool PARTICLE::showArrows = false;
unsigned int PARTICLE::count = 0;

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

PARTICLE::PARTICLE() : icosphere(Icosphere(PARTICLE_DRAW_RADIUS, 0))
{
}

PARTICLE::PARTICLE(const vec3& position) :
  _position(position), icosphere(Icosphere(PARTICLE_DRAW_RADIUS, 0))
{
  _id = count++;
}

PARTICLE::PARTICLE(const vec3& position, const vec3& velocity) :
_position(position), _velocity(velocity), icosphere(Icosphere(PARTICLE_DRAW_RADIUS, 0))
{
  _id = count++;
}

///////////////////////////////////////////////////////////////////////////////
// OGL drawing
///////////////////////////////////////////////////////////////////////////////
void PARTICLE::draw() 
{
  /*
  if (_flag) 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  else 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
  */
    icosphere.setPosition(_position[0], _position[1], _position[2]);
    icosphere.draw();
}

Icosphere* PARTICLE::getIcosphere() {
    return &icosphere;
}

void PARTICLE::clearParameters() {
  _position = vec3();
  _velocity = vec3();
  _acceleration = vec3();
  _density = 0.0f;
  _pressure = 0.0f;
}

