#ifndef WALL_H
#define WALL_H

#include "PARTICLE.h"
#include "VEC3D.h"

using namespace std;

class WALL {
public:
  WALL(const VEC3D& normal, const VEC3D& point);

  // draw to OGL
  void draw();

  // accessors
  VEC3D& normal() { return _normal; };
  VEC3D& point()  { return _point; };

private:  
  VEC3D _normal;
  VEC3D _point;
};

#endif
