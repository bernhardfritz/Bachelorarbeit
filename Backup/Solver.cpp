//
//  Solver.cpp
//  TerrainGeneration
//
//  Solver written by Alex Darcy https://github.com/alexDarcy/shallow-water
//  Fixed and modified by Bernhard Fritz on 14/03/16.
//

#include "Solver.hpp"

Solver::Solver():g(9.81f){}

Solver::Solver(float LX, float nX, float LZ, float nZ):g(9.81f) {
    int choice = 1;
    isDamBreak = false;
    isWaterDrop = false;
    isTsunami = false;

    switch(choice) {
        case 1:
            isWaterDrop= true;
            break;
        case 2:
            isDamBreak = true;
            break;
        case 3:
            isTsunami= true;
            break;
    }
    dx = LX / (nX-1);
    dz = LZ / (nZ-1);

    /* Ghost cells */
    nbX = nX + 2;
    nbZ = nZ + 2;
    Lx = LX + dx;
    Lz = LZ + dz;
    nbPoints = nbX * nbZ;
    nbQuads = (nbX - 1) * (nbZ - 1);

    if(isWaterDrop) h1 = 0.5f;
    else h1 = 0.3f;

    h0 = 0.05f;
    xC1 = LX / 3.0f;
    zC1 = Lz / 3.0f;
    xC2 = 2.0f * LX / 3.0f;
    zC2 = 2.0f * Lz / 3.0f;
    radius = LX / 10.0f;
    float dist1;
    float dist2;
    int i = 0;
    dt = 0.007f;
    F1 = new float[nbPoints];
    F2 = new float[nbPoints];
    F3 = new float[nbPoints];
    G1 = new float[nbPoints];
    G2 = new float[nbPoints];
    G3 = new float[nbPoints];
    /* Dam properties */
    float damL = Lx / 4.0f;
    float damW = 2.0f * dx;
    float holeL = Lz / 2.0f;
    float holeW = 5.0f * dz;
    damLimit = damL / dx;
    damWidth = damW / dx;

    holeLimit = holeL / dz;
    holeWidth = holeW / dz;

    /* Tsunami properties */
    up = 1.0f;
    bool cond;

    q1 = new float[nbPoints];
    q2 = new float[nbPoints];
    q3 = new float[nbPoints];

    for(float z = -dz ; z <= Lz ; z += dz) {
        for(float x = -dx; x <= Lx ; x += dx) {
            if(isDamBreak) cond = (i % nbX) <= damLimit + damWidth;
            else if(isWaterDrop) {
                dist1 = (x - xC1) * (x - xC1) + (z - zC1) * (z - zC1);
                dist1 = glm::sqrt(dist1);
                dist2 = (x - xC2) * (x - xC2) + (z - zC2) * (z - zC2);
                dist2 = glm::sqrt(dist2);

                cond = dist1 <= radius;// || dist2 <= radius ;
            } else cond = false;

            if (cond) q1[i] = h1;
            else q1[i] = h0;

            q2[i] = 0;
            q3[i] = 0;
            
            i++;
        }
    }
}

Solver::~Solver() {
    delete[] q1;
    delete[] q2;
    delete[] q3;
}

/* Compute flux and update Q */
void Solver::run(float t) {
    time++;
    glm::vec3 qL(0.0f);
    glm::vec3 qRX(0.0f);
    glm::vec3 qRY(0.0f);
    for(int j = 0; j < nbZ - 1; j++) {
        for(int i = 0; i < nbX - 1; i++) {
            qL.x = q1[i + j * nbX];
            qL.y = q2[i + j * nbX];
            qL.z = q3[i + j * nbX];
            qRX.x = q1[i + 1 + j * nbX];
            qRX.y = q2[i + 1 + j * nbX];
            qRX.z = q3[i + 1 + j * nbX];
            qRY.x = q1[i + j * nbX + nbX];
            qRY.y = q2[i + j * nbX + nbX];
            qRY.z = q3[i + j * nbX + nbX];

            glm::vec3 FTmp = riemannX(qL, qRX);
            glm::vec3 GTmp = riemannY(qL, qRY);

            F1[i + j * nbX] = FTmp.x;
            F2[i + j * nbX] = FTmp.y;
            F3[i + j * nbX] = FTmp.z;

            G1[i + j * nbX] = GTmp.x;
            G2[i + j * nbX] = GTmp.y;
            G3[i + j * nbX] = GTmp.z;
        }
    }

    bool cond;
    bool cond1;
    bool cond2;
    for(int j = 0; j < nbZ - 1; j++) {
        for(int i = 0; i < nbX - 1; i++) {
            // dam break
            if (isDamBreak) {
                cond = (glm::abs(j - holeLimit) <= holeWidth * 0.5f);
                cond1 = i > damLimit + damWidth + 1 ; // bottom of the dam
                cond2 = i < damLimit - damWidth;
                cond = cond || cond1 || cond2;
            } else cond = true;
            
            if (cond){
                if (i > 0) {
                    q1[i + j * nbX] = q1[i + j * nbX] - dt / dx * (F1[i + j * nbX] - F1[i - 1 + j * nbX]);
                    q2[i + j * nbX] = q2[i + j * nbX] - dt / dx * (F2[i + j * nbX] - F2[i - 1 + j * nbX]);
                    q3[i + j * nbX] = q3[i + j * nbX] - dt / dx * (F3[i + j * nbX] - F3[i - 1 + j * nbX]);
                }

                if (j > 0) {
                    q1[i + j * nbX] = q1[i + j * nbX] - dt / dz * (G1[i + j * nbX] - G1[i + (j - 1) * nbX]);
                    q2[i + j * nbX] = q2[i + j * nbX] - dt / dz * (G2[i + j * nbX] - G2[i + (j - 1) * nbX]);
                    q3[i + j * nbX] = q3[i + j * nbX] - dt / dz * (G3[i + j * nbX] - G3[i + (j - 1) * nbX]);
                }
            }
        }
    }

    boundary();
    if (isWaterDrop) addDrops(t);
}

// Set boundary conditions
void Solver::boundary() {
    int j0;
    int i0;

    if(isTsunami) { // && time % 10 == 0)
        float mean = 0.0f;
        float dh = (h1 - h0) / 20.0f;

        for(int j = 0; j < nbZ; j++) {
            mean += q1[j*nbX];
        }
        mean /= nbZ;

        if (mean > h1) up = -1.0f;
        else if (mean < h0) up = 1.0f;

        for(int j = 0; j < nbZ; j++) {
            q1[j * nbX] += dh * up;
        }
        j0 = 0;
        for(int i = 0; i < nbZ; i++) {
            q1[i + j0 * nbX] = q1[i + (j0 + 1) * nbX];
            q2[i + j0 * nbX] = q2[i + (j0 + 1) * nbX];
            q3[i + j0 * nbX] = q3[i + (j0 + 1) * nbX];
        }
        j0 = nbZ - 1;
        for(int i = 0; i < nbZ; i++) {
            q1[i + j0 * nbX] = q1[i + (j0 - 1) * nbX];
            q2[i + j0 * nbX] = q2[i + (j0 - 1) * nbX];
            q3[i + j0 * nbX] = q3[i + (j0 - 1) * nbX];
        }
    } else if(isDamBreak) {
        i0 = nbX - 1;
        for(int j = 0; j < nbZ; j++) {
            q1[i0 + j * nbX] = q1[i0 + j * nbX - 1];
            q2[i0 + j * nbX] = q2[i0 + j * nbX - 1];
            q3[i0 + j * nbX] = q3[i0 + j * nbX - 1];
        }
    } else if (isWaterDrop) {
        /* Reflection on the boundary */
        for (int j = 1; j < nbZ - 1; j++)
        {
            i0 = 1;
            q2[i0+j*nbX] = -q2[i0+1+j*nbX];
            q3[i0+j*nbX] = -q3[i0+1+j*nbX];
            i0 = nbX-2;
            q2[i0+j*nbX] = -q2[i0-1+j*nbX];
            q3[i0+j*nbX] = -q3[i0-1+j*nbX];
        }
        for (int i = 1; i < nbX - 1; i++)
        {
            j0 = 1;
            q2[i+j0*nbX] = -q2[i+(j0+1)*nbX];
            q3[i+j0*nbX] = -q3[i+(j0+1)*nbX];
            j0 = nbZ-2;
            q2[i+j0*nbX] = -q2[i+(j0-1)*nbX];
            q3[i+j0*nbX] = -q3[i+(j0-1)*nbX];
        }
    }
}

void Solver::addDrops(float t) {
    //float latency = 1.f;
    float precision = 1e-2f;
    float x;
    float z;
    float dist;
    /* random */
    float tmp = t - (int)t;
    if(tmp < precision) {
        int iC = xC1 / dx;
        int jC = zC1 / dz;
        int iWidth = radius / dx;
        int jWidth = radius / dz;
        for(int i = iC - iWidth; i <= iC + iWidth; i++) {
            for(int j = jC - jWidth; j <= jC + jWidth; j++) {
                x = i * dx;
                z = j * dz;
                dist = (x - xC1) * (x - xC1) + (z - zC1) * (z - zC1);
                dist = glm::sqrt(dist);

                if (dist <= radius) {
                    q1[i + j * nbX] = h1;
                    q2[i + j * nbX] = 0.0f;
                    q3[i + j * nbX] = 0.0f;
                }
            }
        }
    }
}

/* Solve the riemann problem on X */
glm::vec3 Solver::riemannX(glm::vec3& qL, glm::vec3& qR) {
    float hL = qL.x;
    float uL = qL.y / qL.x;
    float vL = qL.z / qL.x;
    float hR = qR.x;
    float uR = qR.y / qR.x;
    float vR = qR.z / qR.x;

    float hBar = 0.5f * (hL + hR);
    float uTilde = (glm::sqrt(hL) * uL + glm::sqrt(hR) * uR) / (glm::sqrt(hL) + glm::sqrt(hR));
    float vTilde = (glm::sqrt(hL) * vL + glm::sqrt(hR) * vR) / (glm::sqrt(hL) + glm::sqrt(hR));
    float cTilde = glm::sqrt(g * hBar);

    glm::vec3 r1(1.0f, uTilde - cTilde, vTilde);
    glm::vec3 r2(0.0f, 0.0f, 1.0f);
    glm::vec3 r3(1.0f, uTilde + cTilde, vTilde);

    glm::vec3 alpha(0.0f);
    glm::vec3 lambda(0.0f);
    glm::vec3 delta = qR;
    delta -= qL;
    glm::vec3 w(0.0f);
    alpha.x = ((uTilde + cTilde) * delta.x - delta.y) / (2 * cTilde);
    alpha.y = -vTilde * delta.x + delta.z;
    alpha.z = (-(uTilde - cTilde) * delta.x + delta.y) / (2 * cTilde);
    lambda.x = uTilde - cTilde;
    lambda.y = uTilde;
    lambda.z = uTilde + cTilde;
    w = r1 * phi(lambda.x) * alpha.x;
    w += r2 * phi(lambda.y) * alpha.y;
    w += r3 * phi(lambda.z) * alpha.z;
    w *= 0.5f;

    glm::vec3 F;
    F.x = 0.5f * (qL.y + qR.y);
    F.y = 0.5f * (qL.y * qL.y / qL.x + 0.5f * g * qL.x * qL.x + qR.y * qR.y / qR.x + 0.5f * g * qR.x * qR.x);
    F.z = 0.5f * (qL.y * qL.z / qL.x + qR.y * qR.z / qR.x);
    F = F - w;
    //lambdaMax = max(lambda);
    return F;
}

/* Solve the riemann problem on Y */
glm::vec3 Solver::riemannY(glm::vec3& qL,glm::vec3& qR) {
    float hL = qL.x;
    float uL = qL.y / qL.x;
    float vL = qL.z / qL.x;
    float hR = qR.x;
    float uR = qR.y / qR.x;
    float vR = qR.z / qR.x;
    float hBar = 0.5f * (hL + hR);
    float uTilde = (glm::sqrt(hL) * uL + glm::sqrt(hR) * uR) / (glm::sqrt(hL) + glm::sqrt(hR));
    float vTilde = (glm::sqrt(hL) * vL + glm::sqrt(hR) * vR) / (glm::sqrt(hL) + glm::sqrt(hR));

    float cTilde = glm::sqrt(g * hBar);

    glm::vec3 r1(1.0f, uTilde, vTilde - cTilde);
    glm::vec3 r2(0.0f, -1.0f, 0.0f);
    glm::vec3 r3(1.0f, uTilde, vTilde + cTilde);

    glm::vec3 delta = qR;
    delta -= qL;

    glm::vec3 alpha(0.0f);
    glm::vec3 lambda(0.0f);
    glm::vec3 w(0.0f);
    alpha.x = ((vTilde + cTilde) * delta.x - delta.z) / (2.0f * cTilde);
    alpha.y = uTilde * delta.x - delta.y;
    alpha.z = (-(vTilde - cTilde) * delta.x + delta.z) / (2.0f * cTilde);
    lambda.x = vTilde - cTilde;
    lambda.y = vTilde;
    lambda.z = vTilde + cTilde;
    w = r1 * phi(lambda.x) * alpha.x;
    w += r2 * phi(lambda.y) * alpha.y;
    w += r3 * phi(lambda.z) * alpha.z;
    w *= 0.5f;



    glm::vec3 G(0.0f);
    G.x = 0.5f * (qL.z + qR.z);
    G.y = 0.5f * (qL.y * qL.z / qL.x + qR.y * qR.z / qR.x) ;
    G.z = 0.5f * (qL.z * qL.z / qL.x + 0.5f * g * qL.x * qL.x + qR.z * qR.z / qR.x + 0.5f * g * qR.x * qR.x);

    G = G - w;
    return G;

    //lambdaMax = max(lambda);
}

// Harten entropy fix
float Solver::phi(float lambda) {
    // empirical value
    float epsilon = 2.0f;
    if(glm::abs(lambda) >= epsilon) return glm::abs(lambda);
    else return(lambda * lambda + epsilon * epsilon) / (2.0f * epsilon);
}
