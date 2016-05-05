#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#include "PARTICLE.h"
#include "WALL.h"
#include <vector>
//#include <tr1/tuple>
//#include <map>
#include "FIELD_3D.h"

#include "HW1.h"

#include "ShaderManager.hpp"
#include "Icosphere.hpp"
#include "MetaMesh.hpp"
#include "DirectionalLight.hpp"
#include "Heightmap.hpp"

#define GRID_HEIGHT 0.015 //0.02 //0.0457 //0.02 //0.045

#define GAS_STIFFNESS 3.0 //3.0 //20.0 // 461.5  // Nm/kg is gas constant of water vapor
#define REST_DENSITY 998.29 // kg/m^3 is rest density of water particle
#define PARTICLE_MASS 0.02 // kg
#define VISCOSITY 3.5 //3.5 // 5.0 // 0.00089 // Ns/m^2 or Pa*s viscosity of water
#define SURFACE_TENSION 0.0728 // N/m
#define SURFACE_THRESHOLD 7.065
#define KERNEL_PARTICLES 20.0

#define GRAVITY_ACCELERATION -9.80665


#define WALL_K 10000.0 // wall spring constant
#define WALL_DAMPING -90.0 // wall damping constant

#define BOX_SIZE 0.4
#define MAX_PARTICLES 3000

#define INITIAL_SCENARIO MY_SCENARIO

using namespace std;

class PARTICLE_SYSTEM {
  
public:
  PARTICLE_SYSTEM();
    PARTICLE_SYSTEM(Heightmap* heightmap);
  ~PARTICLE_SYSTEM();
    
  void updateGrid();
  
  // draw to OGL
  void draw(const GLfloat *view, const GLfloat *proj, DirectionalLight& light);
  
  void addParticle(const VEC3D& position);
  
  void addParticle(const VEC3D& position, const VEC3D& velocity);
    
    vector<VEC3D*> getPositions();
    
    void updateMetaMesh();
  
  void stepVerlet(double dt);
  
  void stepVerletBrute(double dt);
    
  void calculateAcceleration();
  
  void calculateAccelerationBrute();
  
  void collisionForce(PARTICLE& particle, VEC3D& f_collision);
  
  void getNeighborParticles(vector<PARTICLE>& totalNeighborParticles, int x, int y, int z);
  
  double Wpoly6(double radiusSquared);
  
  void Wpoly6Gradient(VEC3D& diffPosition, double radiusSquared, VEC3D& gradient);
  
  double Wpoly6Laplacian(double radiusSquared); 
  
  void WspikyGradient(VEC3D& diffPosition, double radiusSquared, VEC3D& gradient);
  
  double WviscosityLaplacian(double radiusSquared);
  
  void toggleGridVisble();
  
  void toggleSurfaceVisible();
  
  void toggleGravity();
  
  void toggleArrows();
  
  void toggleTumble();
    
    void rain(int iterations = 1);
    
    void removeDeadParticles();
    
  void generateFaucetParticleSet();
  
  void setGravityVectorWithViewVector(VEC3D viewVector);
  
  
  void loadScenario(int scenario);
  
  
  
  
  
  //typedef std::tr1::tuple<int,int,int> gridKey;  
  //std::map<gridKey, std::vector<PARTICLE> > grid;
  
  
  FIELD_3D* grid;
  double surfaceThreshold;
  VEC3D gravityVector;

private:
  // list of particles, walls, and springs being simulated
  vector<PARTICLE> _particles;
  vector<WALL>     _walls;

  //unsigned int _particleCount;
  bool _isGridVisible;
  bool _tumble;
  
  VEC3D boxSize;
    
    ShaderManager shaderManager;
    Icosphere icosphere;
    MetaMesh metamesh;
    Heightmap* heightmap = NULL;
    
    float gauss[9] = {
        1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f,
        1.0f/8.0f , 1.0f/4.0f, 1.0f/8.0f ,
        1.0f/16.0f, 1.0f/8.0f, 1.0f/16.0f
    };

};

#endif
