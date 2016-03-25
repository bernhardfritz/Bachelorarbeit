#include "PARTICLE.h"

VEC3F red(1,0,0);
VEC3F blue(0,0,1); 
VEC3F black(0,0,0);
VEC3F green(0,1,0);
VEC3F lightBlueColor(0.01,0.25,1.0);
VEC3F purpleColor(0.88,0.08,0.88);

int count = 0;

bool PARTICLE::isSurfaceVisible = false;
bool PARTICLE::showArrows = false;
unsigned int PARTICLE::count = 0;

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

PARTICLE::PARTICLE()
{
}

PARTICLE::PARTICLE(const VEC3D& position) :
  _position(position)
{
  _id = count++;
}

PARTICLE::PARTICLE(const VEC3D& position, const VEC3D& velocity) :
_position(position), _velocity(velocity)
{
  _id = count++;
}

///////////////////////////////////////////////////////////////////////////////
// OGL drawing
///////////////////////////////////////////////////////////////////////////////
void PARTICLE::draw()
{
    // PARTICLE_SYSTEM is drawing
}

void PARTICLE::clearParameters() {
  _position = VEC3D();
  _velocity = VEC3D();
  _acceleration = VEC3D();
  _density = 0.0;
  _pressure = 0.0;
}

