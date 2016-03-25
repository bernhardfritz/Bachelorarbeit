#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <glm/glm.hpp>
#include "Icosphere.hpp"

using namespace std;
using namespace glm;



class PARTICLE {
public:
  static bool isSurfaceVisible;
  static bool showArrows;
  
  //static unsigned int count;
  PARTICLE();
  PARTICLE(const vec3& position);
  PARTICLE(const vec3& position, const vec3& velocity);
  //~PARTICLE();
  
  // draw to OGL
  void draw();

  // clear all previous accumulated forces
  void clearForce() { _force *= 0; };

  // accumulate forces
  void addForce(vec3 newForce) { _force += newForce; };
  
  void calculateAcceleration();

  // accessors
  vec3& position() { return _position; };
  vec3& velocity() { return _velocity; };
  vec3& acceleration() { return _acceleration; }
  vec3& force()    { return _force; };
  float& density()  { return _density; };
  float& pressure() { return _pressure; };
  bool& flag() { return _flag; };
  int& id() { return _id; };
  vec3 normal;
    
    Icosphere* getIcosphere();
  
  void clearParameters();
  
  static unsigned int count;
  
private:  
  vec3 _position;
  vec3 _velocity;
  vec3 _force;
  vec3 _acceleration;
  float _density;
  float _pressure;
  bool _flag;
  int _id;
    Icosphere icosphere;
};


#endif
