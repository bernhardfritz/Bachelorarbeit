#ifndef PARTICLE_H
#define PARTICLE_H

#include "VEC3F.h"
#include "VEC3D.h"
#include <vector>

#define PARTICLE_DRAW_RADIUS 0.007//0.015//0.01 //0.006
#define PARTICLE_DEFAULT_LIFESPAN 500
#define PARTICLE_DEFAULT_SEDIMENT_CAPACITY 250
#define PARTICLE_ACIDITY 0.0001

using namespace std;



class PARTICLE {
  
  

  
public:
  
  static bool isSurfaceVisible;
  static bool showArrows;
  
  //static unsigned int count;
  PARTICLE();
  PARTICLE(const VEC3D& position);
  PARTICLE(const VEC3D& position, const VEC3D& velocity);
  //~PARTICLE();
  
  // draw to OGL
  void draw();

  // clear all previous accumulated forces
  void clearForce() { _force *= 0; };

  // accumulate forces
  void addForce(VEC3D newForce) { _force += newForce; };
  
  void calculateAcceleration();

  // accessors
  VEC3D& position() { return _position; };
  VEC3D& velocity() { return _velocity; };
  VEC3D& acceleration() { return _acceleration; }
  VEC3D& force()    { return _force; };
  double& density()  { return _density; };
  double& pressure() { return _pressure; };
  bool& flag() { return _flag; };
  int& id() { return _id; };
  VEC3D normal;
    
    int& lifespan() { return _lifespan; };
    int& sedimentCapacity() { return _sedimentCapacity; };
  
  void clearParameters();
  
  static unsigned int count;
  
private:  
  VEC3D _position;
  VEC3D _velocity;
  VEC3D _force;
  VEC3D _acceleration;
  double _density;
  double _pressure;
  bool _flag;
  int _id;
    
    int _lifespan = PARTICLE_DEFAULT_LIFESPAN;
    int _sedimentCapacity = PARTICLE_DEFAULT_SEDIMENT_CAPACITY;
};


#endif
