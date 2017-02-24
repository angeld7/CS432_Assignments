#include "Camera.h"

void Camera::calculateMatrix() {
    float width = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);
    if(orthograpic) {
        projectionMatrix = Ortho(-1, 1, -1, 1, 1.0, 3.0);
        
    } else {
        projectionMatrix = Perspective(65.0, width/height, 1, 100);
    }
    at = eye-n;
    cameraMatrix = LookAt(eye,at,v);
    viewMatrix = projectionMatrix * cameraMatrix;
}

Camera::Camera(vec4 location, vec4 up){
    eye = location;
    v = up;
    at = vec3(0,0,-10);
    n = normalize(eye-at);
    u = normalize(cross(v,n));
    v = normalize(cross(n,u));
    calculateMatrix();
}



void Camera::togglePerspective(){
    if(!allowMove) return;
    orthograpic = !orthograpic;
    calculateMatrix();
}

void Camera::moveFoward(){
    if(!allowMove) return;
    eye -= (n * FOWARD_INC);
    calculateMatrix();
}

void Camera::moveBackward(){
    if(!allowMove) return;
    eye += (n * FOWARD_INC);
    calculateMatrix();
}

void Camera::moveLeft(){
    if(!allowMove) return;
    eye -= (u * SIDE_INC);
    calculateMatrix();
}

void Camera::moveRight(){
    if(!allowMove) return;
    eye += (u * SIDE_INC);
    calculateMatrix();
}

void Camera::rollClockwise(){
    if(!allowMove) return;
    u = (cos(T_INC) * u) - (sin(T_INC) * v);
    v = (sin(T_INC) * u) + (cos(T_INC) * v);
    calculateMatrix();
}

void Camera::rollCounterClockwise(){
    if(!allowMove) return;
    u = (cos(-T_INC) * u) - (sin(-T_INC) * v);
    v = (sin(-T_INC) * u) + (cos(-T_INC) * v);
    calculateMatrix();
}

void Camera::pitchUp(){
    if(!allowMove) return;
    v = (cos(T_INC) * v) - (sin(T_INC) * n);
    n = (sin(T_INC) * v) + (cos(T_INC) * n);
    calculateMatrix();
}

void Camera::pitchDown(){
    if(!allowMove) return;
    v = (cos(-T_INC) * v) - (sin(-T_INC) * n);
    n = (sin(-T_INC) * v) + (cos(-T_INC) * n);
    calculateMatrix();
}

void Camera::yawCounterClockwise(){
    if(!allowMove) return;
    u = (cos(-T_INC) * u) - (sin(-T_INC) * n);
    n = (sin(-T_INC) * u) + (cos(-T_INC) * n);
    calculateMatrix();
}

void Camera::yawClockwise(){
    if(!allowMove) return;
    u = (cos(T_INC) * u) - (sin(T_INC) * n);
    n = (sin(T_INC) * u) + (cos(T_INC) * n);
    calculateMatrix();
}

vec3 Camera::getEye() {
    return vec3(eye.x,eye.y,eye.z);
}

