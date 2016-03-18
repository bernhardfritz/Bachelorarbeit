//
//  Solver.hpp
//  TerrainGeneration
//
//  Solver written by Alex Darcy https://github.com/alexDarcy/shallow-water
//  Fixed and modified by Bernhard Fritz on 14/03/16.
//

#include <iostream>
#include <glm/glm.hpp>

using namespace std;

class Solver
{
  private:
    float g;
    int nbX;
    int nbZ;
    int nbPoints;
    int nbQuads;
    float Lx;
    float Lz;
    float dx;
    float dz;
    float dt;
    float* F1;
    float* F2;
    float* F3;
    float* G1;
    float* G2;
    float* G3;
    int damLimit;
    int damWidth;
    int holeWidth;
    int holeLimit;
    float h1;
    float h0 ;
    bool isDamBreak;
    bool isWaterDrop;
    bool isTsunami;
    float xC1;
    float zC1;
    float xC2;
    float zC2;
    float radius;
    float up;
    int time;
    void boundary();
    void addDrops(float t);
    float phi(float lambda);
    glm::vec3 riemannX(glm::vec3& qL,glm::vec3& qR);
    glm::vec3 riemannY(glm::vec3& qL,glm::vec3& qR);

  public:
    Solver();
    Solver(float Lx,float dx, float Lz,float dz);
    void run(float t);
        ~Solver();

    float* q1;
    float* q2;
    float* q3;
};
