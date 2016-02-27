//
//  Camera.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 28/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <stdio.h>

using namespace glm;

class Camera {
private:
    float x;
    float y;
    float z;
    float pitch;
    float yaw;
    float roll;
    float speed;
    vec3 eye;
    vec3 center;
    vec3 up;
    vec3 u;
    vec3 v;
    vec3 w;
    void updateEye();
    void updateCenter();
    void updateUp();
    void updateUVW();
public:
    Camera(float x, float y, float z, float pitch, float yaw, float speed);
    void setX(float x);
    float getX();
    void setY(float y);
    float getY();
    void setZ(float z);
    float getZ();
    void setPitch(float pitch);
    float getPitch();
    void incPitch(float deltaPitch);
    void setYaw(float yaw);
    float getYaw();
    void incYaw(float deltaYaw);
    void setSpeed(float speed);
    float getSpeed();
    void setEye(vec3 eye);
    vec3 getEye();
    vec3 getCenter();
    vec3 getUp();
    vec3 getU();
    vec3 getV();
    vec3 getW();
    void moveForward(float elapsedSeconds);
    void moveBackward(float elapsedSeconds);
    void moveLeft(float elapsedSeconds);
    void moveRight(float elapsedSeconds);
    void moveUp(float elapsedSeconds);
    void moveDown(float elapsedSeconds);
    void print();
};

#endif /* Camera_hpp */
