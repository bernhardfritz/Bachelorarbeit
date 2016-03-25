#ifndef WALL_H
#define WALL_H

#include "PARTICLE.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class WALL {
public:
  WALL(const vec3& normal, const vec3& point);

  // draw to OGL
  void draw();

  // accessors
  vec3& normal() { return _normal; };
  vec3& point()  { return _point; };

private:  
  vec3 _normal;
  vec3 _point;
};

#endif
