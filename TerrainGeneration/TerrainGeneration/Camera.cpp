//
//  Camera.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 28/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "Camera.hpp"

Camera::Camera(float x, float y, float z, float pitch, float yaw, float speed) {
    setX(x);
    setY(y);
    setZ(z);
    setPitch(pitch);
    setYaw(yaw);
    updateUp();
    setSpeed(speed);
}

void Camera::setX(float x) {
    this->x = x;
    updateEye();
}

float Camera::getX() {
    return x;
}

void Camera::setY(float y) {
    this->y = y;
    updateEye();
}

float Camera::getY() {
    return y;
}

void Camera::setZ(float z) {
    this->z = z;
    updateEye();
}

float Camera::getZ() {
    return z;
}

void Camera::setPitch(float pitch) {
    if(fabs(pitch) > half_pi<float>()-two_pi<float>()/360.0f) {
        pitch = sign(pitch)*(half_pi<float>()-two_pi<float>()/360.0f);
    }
    this->pitch = pitch;
    updateCenter();
}

float Camera::getPitch() {
    return pitch;
}

void Camera::incPitch(float deltaPitch) {
    setPitch(pitch += deltaPitch);
}

void Camera::setYaw(float yaw) {
    this->yaw = yaw;
    updateCenter();
}

float Camera::getYaw() {
    return yaw;
}

void Camera::incYaw(float deltaYaw) {
    setYaw(yaw += deltaYaw);
}

void Camera::setSpeed(float speed) {
    this->speed = speed;
}

float Camera::getSpeed() {
    return speed;
}

void Camera::updateEye() {
    eye = vec3(x, y, z);
    updateCenter();
}

void Camera::updateCenter() {
    center = eye + vec3(cos(pitch)*cos(yaw), sin(pitch), cos(pitch)*sin(yaw));
    updateUVW();
}

void Camera::updateUp() {
    up = vec3(0.0f, 1.0f, 0.0f);
    updateUVW();
}

void Camera::updateUVW() {
    w = normalize(eye - center);
    u = normalize(cross(up, w));
    v = cross(w, u);
}

void Camera::setEye(vec3 eye) {
    this->eye = eye;
    x = eye.x;
    y = eye.y;
    z = eye.z;
    updateEye();
}

vec3 Camera::getEye() {
    return eye;
}

vec3 Camera::getCenter() {
    return center;
}

vec3 Camera::getUp() {
    return up;
}

vec3 Camera::getU() {
    return u;
}

vec3 Camera::getV() {
    return v;
}

vec3 Camera::getW() {
    return w;
}

void Camera::moveForward(float elapsedSeconds) {
    setEye(eye - w * speed * elapsedSeconds);
}

void Camera::moveBackward(float elapsedSeconds) {
    setEye(eye + w * speed * elapsedSeconds);
}

void Camera::moveLeft(float elapsedSeconds) {
    setEye(eye - u * speed * elapsedSeconds);
}

void Camera::moveRight(float elapsedSeconds) {
    setEye(eye + u * speed * elapsedSeconds);
}

void Camera::moveUp(float elapsedSeconds) {
    setEye(eye + v * speed * elapsedSeconds);
}

void Camera::moveDown(float elapsedSeconds) {
    setEye(eye - v * speed * elapsedSeconds);
}

void Camera::print() {
    printf("x:%.2f y:%.2f z:%.2f pitch:%.2f yaw:%.2f\n", getX(), getY(), getZ(), getPitch(), getYaw());
}