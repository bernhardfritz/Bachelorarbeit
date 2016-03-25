#include "PARTICLE_SYSTEM.h"

unsigned int iteration = 0;
int scenario;

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
PARTICLE_SYSTEM::PARTICLE_SYSTEM() : 
_isGridVisible(false), surfaceThreshold(0.01f), gravityVector(0.0f,GRAVITY_ACCELERATION,0.0f), grid(NULL)
{
  loadScenario(INITIAL_SCENARIO);
}

void PARTICLE_SYSTEM::loadScenario(int newScenario) {
  
  scenario = newScenario;
  
  // remove all particles
  
  if (grid) {
    delete grid;
    
  }
    
  _walls.clear();
  
  
  // reset params
  
  PARTICLE::count = 0;
  
  iteration = 0;
    
  
  if (scenario == SCENARIO_DAM) {
    
    // create long grid
    
    boxSize.x = BOX_SIZE*2.0f;
    boxSize.y = BOX_SIZE;
    boxSize.z = BOX_SIZE/2.0f;
    
    int gridXRes = (int)ceil(boxSize.x/h);
    int gridYRes = (int)ceil(boxSize.y/h);
    int gridZRes = (int)ceil(boxSize.z/h);
    
    grid = new FIELD_3D(gridXRes, gridYRes, gridZRes);
    
    
    // add walls 
    
    _walls.push_back(WALL(vec3(0,0,1), vec3(0,0,-boxSize.z/2.0f))); // back
    _walls.push_back(WALL(vec3(0,0,-1), vec3(0,0,boxSize.z/2.0f))); // front
    _walls.push_back(WALL(vec3(1,0,0), vec3(-boxSize.x/2.0f,0,0)));     // left
    _walls.push_back(WALL(vec3(-1,0,0), vec3(boxSize.x/2.0f,0,0)));     // right
    _walls.push_back(WALL(vec3(0,1,0), vec3(0,-boxSize.y/2.0f,0))); // bottom
    
    vector<PARTICLE>& firstGridCell = (*grid)(0,0,0);
    
    // add particles
    
    for (float y = -boxSize.y/2.0f; y < boxSize.y/2.0f; y+= h/2.0f) {
      for (float x = -boxSize.x/2.0f; x < -boxSize.x/4.0f; x += h/2.0f) {
        for (float z = -boxSize.z/2.0f; z < boxSize.z/2.0f; z+= h/2.0f) {
          firstGridCell.push_back(PARTICLE(vec3(x, y,z)));
        }
      }
    }
    
    cout << "Loaded dam scenario" << endl;
    cout << "Grid size is " << (*grid).xRes() << "x" << (*grid).yRes() << "x" << (*grid).zRes() << endl;
    cout << "Simulating " << PARTICLE::count << " particles" << endl;
    
  }
  else if (scenario == SCENARIO_CUBE) {
    
    // create cubed grid
    
    boxSize.x = BOX_SIZE;
    boxSize.y = BOX_SIZE;
    boxSize.z = BOX_SIZE;
    
    int gridXRes = (int)ceil(boxSize.x/h);
    int gridYRes = (int)ceil(boxSize.y/h);
    int gridZRes = (int)ceil(boxSize.z/h);
    
    grid = new FIELD_3D(gridXRes, gridYRes, gridZRes);
    
    // add walls 
    
    _walls.push_back(WALL(vec3(0,0,1), vec3(0,0,-boxSize.z/2.0f))); // back
    _walls.push_back(WALL(vec3(0,0,-1), vec3(0,0,boxSize.z/2.0f))); // front
    _walls.push_back(WALL(vec3(1,0,0), vec3(-boxSize.x/2.0f,0,0))); // left
    _walls.push_back(WALL(vec3(-1,0,0), vec3(boxSize.x/2.0f,0,0))); // right
    _walls.push_back(WALL(vec3(0,1,0), vec3(0,-boxSize.y/2.0f,0))); // bottom
    
    vector<PARTICLE>& firstGridCell = (*grid)(0,0,0);
    
    // add particles
    
    for (float y = 0; y < boxSize.y; y+= h/2.0f) {
      for (float x = -boxSize.x/4.0f; x < boxSize.x/4.0f; x += h/2.0f) {
        for (float z = -boxSize.z/4.0f; z < boxSize.z/4.0f; z+= h/2.0f) {
          firstGridCell.push_back(PARTICLE(vec3(x,y,z)));
        }
      }
    }
    
    cout << "Loaded cube scenario" << endl;
    cout << "Grid size is " << (*grid).xRes() << "x" << (*grid).yRes() << "x" << (*grid).zRes() << endl;
    cout << "Simulating " << PARTICLE::count << " particles" << endl;
  }
  else if (scenario == SCENARIO_FAUCET) {
    
    // create cubed grid
    
    boxSize.x = BOX_SIZE;
    boxSize.y = BOX_SIZE;
    boxSize.z = BOX_SIZE;
    
    int gridXRes = (int)ceil(boxSize.x/h);
    int gridYRes = (int)ceil(boxSize.y/h);
    int gridZRes = (int)ceil(boxSize.z/h);
    
    grid = new FIELD_3D(gridXRes, gridYRes, gridZRes);
    
    // add walls 
    
    _walls.push_back(WALL(vec3(0,0,1), vec3(0,0,-boxSize.z/2.0f))); // back
    _walls.push_back(WALL(vec3(0,0,-1), vec3(0,0,boxSize.z/2.0f))); // front
    _walls.push_back(WALL(vec3(1,0,0), vec3(-boxSize.x/2.0f,0,0))); // left
    _walls.push_back(WALL(vec3(-1,0,0), vec3(boxSize.x/2.0f,0,0))); // right
    _walls.push_back(WALL(vec3(0,1,0), vec3(0,-boxSize.y/2.0f,0))); // bottom
    
    cout << "Loaded faucet scenario" << endl;
    cout << "Grid size is " << (*grid).xRes() << "x" << (*grid).yRes() << "x" << (*grid).zRes() << endl;
  }
  
  
  updateGrid();

  
  
}

void PARTICLE_SYSTEM::generateFaucetParticleSet() {
  
  vec3 initialVelocity(-1.8f,-1.8f,0);
  
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f,BOX_SIZE+h*0.6f, 0), initialVelocity);
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE, 0), initialVelocity);
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE+h*-0.6f, 0), initialVelocity);
  
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE+h*0.3f, h*0.6f), initialVelocity);
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE+h*0.3f, h*-0.6f), initialVelocity);
  
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE+h*-0.3f, h*0.6f), initialVelocity);
  addParticle(vec3(BOX_SIZE/2.0f-h/2.0f, BOX_SIZE+h*-0.3f, h*-0.6f), initialVelocity);


}

void PARTICLE_SYSTEM::addParticle(const vec3& position, const vec3& velocity) {
  
#ifdef BRUTE
  _particles.push_back(PARTICLE(position, velocity));
#else
  (*grid)(0,0,0).push_back(PARTICLE(position, velocity));
#endif
  
  //_particleCount++;
}

void PARTICLE_SYSTEM::addParticle(const vec3& position) {
  
  addParticle(position, vec3());
}

vector<PARTICLE> PARTICLE_SYSTEM::getParticles() {
    return _particles;
}

PARTICLE_SYSTEM::~PARTICLE_SYSTEM()
{
  if (grid) delete grid;
}

void PARTICLE_SYSTEM::toggleGridVisble() {
  _isGridVisible = !_isGridVisible;
}

void PARTICLE_SYSTEM::toggleSurfaceVisible() {
  PARTICLE::isSurfaceVisible = !PARTICLE::isSurfaceVisible;
}

void PARTICLE_SYSTEM::toggleTumble() {
  _tumble = !_tumble;
}

void PARTICLE_SYSTEM::toggleGravity() {
  if (gravityVector.length() > 0.0f) {
    gravityVector = vec3(0,0,0);
  }
  else {
    gravityVector = vec3(0,GRAVITY_ACCELERATION,0);
  }
}

void PARTICLE_SYSTEM::toggleArrows() {
  PARTICLE::showArrows = !PARTICLE::showArrows;
}

void PARTICLE_SYSTEM::setGravityVectorWithViewVector(vec3 viewVector) {
  
  if (_tumble)
    gravityVector = viewVector * GRAVITY_ACCELERATION;
  
}

// to update the grid cells particles are located in
// should be called right after particle positions are updated
void PARTICLE_SYSTEM::updateGrid() {
    
  for (unsigned int x = 0; x < (*grid).xRes(); x++) {
    for (unsigned int y = 0; y < (*grid).yRes(); y++) {
      for (unsigned int z = 0; z < (*grid).zRes(); z++) {
        
        vector<PARTICLE>& particles = (*grid)(x,y,z);
        
        //cout << particles.size() << "p's in this grid" << endl;
                
        for (int p = 0; p < particles.size(); p++) {
          
          PARTICLE& particle = particles[p];
          
          int newGridCellX = (int)floor((particle.position().x+BOX_SIZE/2.0f)/h);
          int newGridCellY = (int)floor((particle.position().y+BOX_SIZE/2.0f)/h);
          int newGridCellZ = (int)floor((particle.position().z+BOX_SIZE/2.0f)/h);
          
          //cout << "particle position: " << particle.position() << endl;
          //cout << "particle cell pos: " << newGridCellX << " " << newGridCellY << " " << newGridCellZ << endl;
        
          if (newGridCellX < 0)
            newGridCellX = 0;
          else if (newGridCellX >= (*grid).xRes())
            newGridCellX = (*grid).xRes() - 1;
          if (newGridCellY < 0)
            newGridCellY = 0;
          else if (newGridCellY >= (*grid).yRes())
            newGridCellY = (*grid).yRes() - 1;
          if (newGridCellZ < 0)
            newGridCellZ = 0;
          else if (newGridCellZ >= (*grid).zRes())
            newGridCellZ = (*grid).zRes() - 1;
          
          //cout << "particle cell pos: " << newGridCellX << " " << newGridCellY << " " << newGridCellZ << endl;

          
          // check if particle has moved
          
          if (x != newGridCellX || y != newGridCellY || z != newGridCellZ) {
            
            // move the particle to the new grid cell
            
            (*grid)(newGridCellX, newGridCellY, newGridCellZ).push_back(particle);
            
            // remove it from it's previous grid cell
            
            particles[p] = particles.back();
            particles.pop_back();
            p--; // important! make sure to redo this index, since a new particle will (probably) be there
          }
          
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// OGL drawing
///////////////////////////////////////////////////////////////////////////////
void PARTICLE_SYSTEM::draw() 
{ 
  static vec3 blackColor(0,0,0);
  static vec3 blueColor(0,0,1);
  static vec3 whiteColor(1,1,1);
  static vec3 greyColor(0.2f, 0.2f, 0.2f);
  static vec3 lightGreyColor(0.8f,0.8f,0.8f);
  //static VEC3F greenColor(34.0f / 255, 139.0f / 255, 34.0f / 255);
  static float shininess = 10.0f;

  // draw the particles
  
  
  //for (unsigned int x = 0; x < _particles.size(); x++)
  //  _particles[x].draw();
  
#ifdef BRUTE
  
  for (unsigned int x = 0; x < _particles.size(); x++)
    _particles[x].draw();
    
#else
  
  for (int gridCellIndex = 0; gridCellIndex < (*grid).cellCount(); gridCellIndex++) {
    
    vector<PARTICLE>& particles = (*grid).data()[gridCellIndex];
    
    for (int p = 0; p < particles.size(); p++) {
      
      PARTICLE& particle = particles[p];
      
      particle.draw();
      
    }
    
  }
    
  
  /*if (_isGridVisible) {
  
    // draw the grid
    
    
    //float offset = -BOX_SIZE/2.0f+h/2.0f;
    
    for (int x = 0; x < grid->xRes(); x++) {
      for (int y = 0; y < grid->yRes(); y++) {
        for (int z = 0; z < grid->zRes(); z++) {
          
          glTranslated(x*h-boxSize.x/2.0f+h/2.0f, y*h-boxSize.y/2.0f+h/2.0f, z*h-boxSize.z/2.0f+h/2.0f);
          glutWireCube(h);
          
        }
      }
    }
    
  }*/
   

#endif
}

///////////////////////////////////////////////////////////////////////////////
// Verlet integration
///////////////////////////////////////////////////////////////////////////////
void PARTICLE_SYSTEM::stepVerlet(float dt)
{
  
  calculateAcceleration();
  
  for (unsigned int gridCellIndex = 0; gridCellIndex < (*grid).cellCount(); gridCellIndex++) {
    
    vector<PARTICLE>& particles = (*grid).data()[gridCellIndex];
    
    for (unsigned int p = 0; p < particles.size(); p++) {
      
      PARTICLE& particle = particles[p];
      
      vec3 newPosition = particle.position() + particle.velocity()*dt + particle.acceleration()*dt*dt;
      vec3 newVelocity = (newPosition - particle.position()) / dt;
      
      particle.position() = newPosition;
      particle.velocity() = newVelocity;
    }
  }
  
  if (scenario == SCENARIO_FAUCET && PARTICLE::count < MAX_PARTICLES) {
    generateFaucetParticleSet();
  }
  
  updateGrid();
  
  iteration++;
}

///////////////////////////////////////////////////////////////////////////////
// Verlet integration
///////////////////////////////////////////////////////////////////////////////
void PARTICLE_SYSTEM::stepVerletBrute(float dt)
{
  if (PARTICLE::count < MAX_PARTICLES && iteration % 12 == 0) {
    generateFaucetParticleSet();
  }
  
  calculateAccelerationBrute();
  
  for (unsigned int i = 0; i < PARTICLE::count; i++) {
    
    PARTICLE& particle = _particles[i];
    
    vec3 newPosition = particle.position() + particle.velocity()*dt + particle.acceleration()*dt*dt;
    vec3 newVelocity = (newPosition - particle.position()) / dt;
    
    particle.position() = newPosition;
    particle.velocity() = newVelocity;
  }
  
  iteration++;
  
}

/*
 Calculate the acceleration of each particle using a grid optimized approach.
 For each particle, only particles in the same grid cell and the (26) neighboring grid cells must be considered,
 since any particle beyond a grid cell distance away contributes no force.
*/
void PARTICLE_SYSTEM::calculateAcceleration() {
  
  ///////////////////
  // STEP 1: UPDATE DENSITY & PRESSURE OF EACH PARTICLE
  
  for (int x = 0; x < (*grid).xRes(); x++) {
    for (int y = 0; y < (*grid).yRes(); y++) {
      for (int z = 0; z < (*grid).zRes(); z++) {
        
        vector<PARTICLE>& particles = (*grid)(x,y,z);
                
        for (int p = 0; p < particles.size(); p++) {
          
          PARTICLE& particle = particles[p];

          particle.density() = 0.0;
          
          // now iteratate through neighbors
          
          for (int offsetX = -1; offsetX <= 1; offsetX++) {
            if (x+offsetX < 0) continue;
            if (x+offsetX >= (*grid).xRes()) break;
            
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
              if (y+offsetY < 0) continue;
              if (y+offsetY >= (*grid).yRes()) break;
              
              for (int offsetZ = -1; offsetZ <= 1; offsetZ++) {
                if (z+offsetZ < 0) continue;
                if (z+offsetZ >= (*grid).zRes()) break;
                
                vector<PARTICLE>& neighborGridCellParticles = (*grid)(x+offsetX, y+offsetY, z+offsetZ);
              
                for (int i = 0; i < neighborGridCellParticles.size(); i++) {
                  
                  PARTICLE& neighborParticle = neighborGridCellParticles[i];
                  
                  vec3 diffPosition = particle.position() - neighborParticle.position();
                  
                  float radiusSquared = dot(diffPosition, diffPosition);
                  
                  if (radiusSquared <= h*h)
                    particle.density() += Wpoly6(radiusSquared);
                  
                }
              }
            }
          }
          
          particle.density() *= PARTICLE_MASS;
                        
          // p = k(density - density_rest)
          
          particle.pressure() = GAS_STIFFNESS * (particle.density() - REST_DENSITY);           
        }
      }
    }
  }
  
  ///////////////////
  // STEP 2: COMPUTE FORCES FOR ALL PARTICLES
  
  for (int x = 0; x < (*grid).xRes(); x++) {
    for (int y = 0; y < (*grid).yRes(); y++) {
      for (int z = 0; z < (*grid).zRes(); z++) {
        
        vector<PARTICLE>& particles = (*grid)(x,y,z);
        
        for (int p = 0; p < particles.size(); p++) {
          
          PARTICLE& particle = particles[p];
          
          //cout << "particle id: " << particle.id() << endl;
          
          vec3 f_pressure,
          f_viscosity, 
          f_surface, 
          f_gravity = gravityVector * particle.density(),
          colorFieldNormal;
          
          float colorFieldLaplacian;
                    
          // now iteratate through neighbors
          
          for (int offsetX = -1; offsetX <= 1; offsetX++) {
            if (x+offsetX < 0) continue;
            if (x+offsetX >= (*grid).xRes()) break;
            
            for (int offsetY = -1; offsetY <= 1; offsetY++) {
              if (y+offsetY < 0) continue;
              if (y+offsetY >= (*grid).yRes()) break;
              
              for (int offsetZ = -1; offsetZ <= 1; offsetZ++) {
                if (z+offsetZ < 0) continue;
                if (z+offsetZ >= (*grid).zRes()) break;
                
                vector<PARTICLE>& neighborGridCellParticles = (*grid)(x+offsetX, y+offsetY, z+offsetZ);
                
                for (int i = 0; i < neighborGridCellParticles.size(); i++) {
                  
                  PARTICLE& neighbor = neighborGridCellParticles[i];
                  
                  //if (particle.id() == neighbor.id()) continue; // SKIPPING COMPARISON OF THE SAME PARTICLE
                  
                  vec3 diffPosition = particle.position() - neighbor.position();
                  float radiusSquared = dot(diffPosition, diffPosition);
                  
                  if (radiusSquared <= h*h) {
                    
                    vec3 poly6Gradient, spikyGradient;
                    
                    Wpoly6Gradient(diffPosition, radiusSquared, poly6Gradient);
                    
                    WspikyGradient(diffPosition, radiusSquared, spikyGradient);
                    
                    if (particle.id() != neighbor.id()) {
                      
                      f_pressure += (float)(particle.pressure()/pow(particle.density(),2)+neighbor.pressure()/pow(neighbor.density(),2))*spikyGradient;
                      
                      f_viscosity += (neighbor.velocity() - particle.velocity()) * WviscosityLaplacian(radiusSquared) / neighbor.density();

                    }
                    
                                        
                    colorFieldNormal += poly6Gradient / neighbor.density();
                    
                    colorFieldLaplacian += Wpoly6Laplacian(radiusSquared) / neighbor.density();
                    
                  }
                }
              }
            }
          } // end of neighbor grid cell iteration
          
          f_pressure *= -PARTICLE_MASS * particle.density();
          
          f_viscosity *= VISCOSITY * PARTICLE_MASS;
          
          colorFieldNormal *= PARTICLE_MASS;
          
          
          particle.normal = -1.0f * colorFieldNormal;
          
          colorFieldLaplacian *= PARTICLE_MASS;
          
          
          // surface tension force
          
          float colorFieldNormalMagnitude = colorFieldNormal.length();
          
          if (colorFieldNormalMagnitude > SURFACE_THRESHOLD) {
            
            particle.flag() = true;
            f_surface = -SURFACE_TENSION * colorFieldNormal / colorFieldNormalMagnitude * colorFieldLaplacian;
            
          }
          else {
            particle.flag() = false;
          }
          
          // ADD IN SPH FORCES
          
          particle.acceleration() = (f_pressure + f_viscosity + f_surface + f_gravity) / particle.density();
          
          // EXTERNAL FORCES HERE (USER INTERACTION, SWIRL)
          
          vec3 f_collision;
          collisionForce(particle, f_collision);
          
        } 
      }
    }
  }
  
}


void PARTICLE_SYSTEM::collisionForce(PARTICLE& particle, vec3& f_collision) {
    
  for (unsigned int i = 0; i < _walls.size(); i++) {
    
    WALL& wall = _walls[i];
    
    float d = dot((wall.point() - particle.position()), wall.normal()) + 0.01f; // particle radius
    
    if (d > 0.0f) {
      // This is an alernate way of calculating collisions of particles against walls, but produces some jitter at boundaries
      //particle.position() += d * wall.normal();
      //particle.velocity() -= particle.velocity().dot(wall.normal()) * 1.9 * wall.normal();
      
      particle.acceleration() += WALL_K * wall.normal() * d;
      particle.acceleration() += WALL_DAMPING * dot(particle.velocity(), wall.normal()) * wall.normal();
    }
  }
}


float PARTICLE_SYSTEM::Wpoly6(float radiusSquared) {
    
  static float coefficient = 315.0f/(64.0f*M_PI*pow(h,9));
  static float hSquared = h*h;
  
  return coefficient * pow(hSquared-radiusSquared, 3);
}

void PARTICLE_SYSTEM::Wpoly6Gradient(vec3& diffPosition, float radiusSquared, vec3& gradient) {
    
  static float coefficient = -945.0f/(32.0f*M_PI*pow(h,9));
  static float hSquared = h*h;
    
  gradient = (float)(coefficient * pow(hSquared-radiusSquared, 2)) * diffPosition;
}

float PARTICLE_SYSTEM::Wpoly6Laplacian(float radiusSquared) {
    
  static float coefficient = -945.0f/(32.0f*M_PI*pow(h,9));
  static float hSquared = h*h;
    
  return coefficient * (hSquared-radiusSquared) * (3.0*hSquared - 7.0*radiusSquared);
}

void PARTICLE_SYSTEM::WspikyGradient(vec3& diffPosition, float radiusSquared, vec3& gradient) {  //
  
  static float coefficient = -45.0f/(M_PI*pow(h,6));
  
  float radius = sqrt(radiusSquared);
  
  gradient = (float)(coefficient * pow(h-radius, 2)) * diffPosition / radius;
}
 

float PARTICLE_SYSTEM::WviscosityLaplacian(float radiusSquared) {
  
  static float coefficient = 45.0/(M_PI*pow(h,6));
  
  float radius = sqrt(radiusSquared);
  
  return coefficient * (h - radius);    
}

/*
 Calculate the acceleration of every particle in a brute force manner (n^2).
 Used for debugging.
*/
void PARTICLE_SYSTEM::calculateAccelerationBrute() {
        
  ///////////////////
  // STEP 1: UPDATE DENSITY & PRESSURE OF EACH PARTICLE
  
  for (int i = 0; i < PARTICLE::count; i++) {
    
    // grab ith particle reference
    
    PARTICLE& particle = _particles[i];
    
    // now iteratate through neighbors
    
    particle.density() = 0.0;
            
    for (int j = 0; j < PARTICLE::count; j++) {
      
      PARTICLE& neighborParticle = _particles[j];
      
      vec3 diffPosition = particle.position() - neighborParticle.position();
      
      float radiusSquared = dot(diffPosition, diffPosition);
      
      if (radiusSquared <= h*h)
        particle.density() += Wpoly6(radiusSquared);
    }
    
    particle.density() *= PARTICLE_MASS;
    
    // p = k(density - density_rest)
    
    particle.pressure() = GAS_STIFFNESS * (particle.density() - REST_DENSITY);
  
    //totalDensity += particle.density();
  }
    
  ///////////////////
  // STEP 2: COMPUTE FORCES FOR ALL PARTICLES
  
  for (int i = 0; i < PARTICLE::count; i++) {
    
    PARTICLE& particle = _particles[i];
    
    //cout << "particle id: " << particle.id() << endl;
    
    vec3 f_pressure,
    f_viscosity, 
    f_surface, 
    f_gravity(0.0, particle.density() * -9.80665, 0.0),
    n, 
    colorFieldNormal,
    colorFieldLaplacian; // n is gradient of colorfield
    //float n_mag;
    
    for (int j = 0; j < PARTICLE::count; j++) {
      PARTICLE& neighbor = _particles[j];
      
      vec3 diffPosition = particle.position() - neighbor.position();
      vec3 diffPositionNormalized = normalize(diffPosition); // need?
      float radiusSquared = dot(diffPosition, diffPosition);
      
      if (radiusSquared <= h*h) {
        
                
        if (radiusSquared > 0.0) {
          
          //neighborsVisited++;
          //cout << neighborsVisited << endl;
          
          //cout << neighbor.id() << endl;
          
          vec3 gradient;
                    
          Wpoly6Gradient(diffPosition, radiusSquared, gradient);
                    
          f_pressure += (particle.pressure() + neighbor.pressure()) / (2.0f * neighbor.density()) * gradient;
          
          colorFieldNormal += gradient / neighbor.density();
        }
        
        f_viscosity += (neighbor.velocity() - particle.velocity()) * WviscosityLaplacian(radiusSquared) / neighbor.density();
        
        colorFieldLaplacian += Wpoly6Laplacian(radiusSquared) / neighbor.density();
      }
      
    }
    
    f_pressure *= -PARTICLE_MASS;
    
    //totalPressure += f_pressure;
    
    f_viscosity *= VISCOSITY * PARTICLE_MASS;
    
    colorFieldNormal *= PARTICLE_MASS;
    
    particle.normal = -1.0f * colorFieldNormal;
    
    colorFieldLaplacian *= PARTICLE_MASS;
    
    // surface tension force
    
    float colorFieldNormalMagnitude = colorFieldNormal.length();
    
    if (colorFieldNormalMagnitude > surfaceThreshold) {
      
      particle.flag() = true;
      f_surface = -SURFACE_TENSION * colorFieldLaplacian * colorFieldNormal / colorFieldNormalMagnitude;
      
    }
    else {
      particle.flag() = false;
    }
    
    // ADD IN SPH FORCES
    
    particle.acceleration() = (f_pressure + f_viscosity + f_surface + f_gravity) / particle.density();
    
    
    // EXTERNAL FORCES HERE (USER INTERACTION, SWIRL)
    
    vec3 f_collision;
    
    collisionForce(particle, f_collision);    

  }
}

