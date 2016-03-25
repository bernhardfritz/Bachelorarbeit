#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#include "PARTICLE.h"
#include "WALL.h"
#include <vector>
//#include <tr1/tuple>
//#include <map>
#include "FIELD_3D.h"

#define h 0.0457f //0.02 //0.045

#define GAS_STIFFNESS 3.0f //20.0 // 461.5  // Nm/kg is gas constant of water vapor
#define REST_DENSITY 998.29f // kg/m^3 is rest density of water particle
#define PARTICLE_MASS 0.02f // kg
#define VISCOSITY 3.5f // 5.0 // 0.00089 // Ns/m^2 or Pa*s viscosity of water
#define SURFACE_TENSION 0.0728f // N/m
#define SURFACE_THRESHOLD 7.065f
#define KERNEL_PARTICLES 20.0f

#define GRAVITY_ACCELERATION -9.80665f

#define BRUTE1

#define SCENARIO_DAM      0
#define SCENARIO_FAUCET   1
#define SCENARIO_CUBE     2

#define WALL_K 10000.0f // wall spring constant
#define WALL_DAMPING -0.9f // wall damping constant

#define BOX_SIZE 0.4f
#define MAX_PARTICLES 3000

#define INITIAL_SCENARIO SCENARIO_DAM

using namespace std;

class PARTICLE_SYSTEM {
  
public:
  PARTICLE_SYSTEM();
  ~PARTICLE_SYSTEM();

  void updateGrid();
  
  // draw to OGL
  void draw();
  
  void addParticle(const vec3& position);
  
  void addParticle(const vec3& position, const vec3& velocity);
    
    vector<PARTICLE> getParticles();
  
  void stepVerlet(float dt);
  
  void stepVerletBrute(float dt);
    
  void calculateAcceleration();
  
  void calculateAccelerationBrute();
  
  void collisionForce(PARTICLE& particle, vec3& f_collision);
  
  void getNeighborParticles(vector<PARTICLE>& totalNeighborParticles, int x, int y, int z);
  
  float Wpoly6(float radiusSquared);
  
  void Wpoly6Gradient(vec3& diffPosition, float radiusSquared, vec3& gradient);
  
  float Wpoly6Laplacian(float radiusSquared);
  
  void WspikyGradient(vec3& diffPosition, float radiusSquared, vec3& gradient);
  
  float WviscosityLaplacian(float radiusSquared);
  
  void toggleGridVisble();
  
  void toggleSurfaceVisible();
  
  void toggleGravity();
  
  void toggleArrows();
  
  void toggleTumble();
  
  void generateFaucetParticleSet();
  
  void setGravityVectorWithViewVector(vec3 viewVector);
  
  
  void loadScenario(int scenario);
  
  
  
  
  
  //typedef std::tr1::tuple<int,int,int> gridKey;  
  //std::map<gridKey, std::vector<PARTICLE> > grid;
  
  
  FIELD_3D* grid;
  float surfaceThreshold;
  vec3 gravityVector;

private:
  // list of particles, walls, and springs being simulated
  vector<PARTICLE> _particles;
  vector<WALL>     _walls;

  //unsigned int _particleCount;
  bool _isGridVisible;
  bool _tumble;
  
  vec3 boxSize;

};

#endif
