#ifndef _CAMERA_H
#define _CAMERA_H

#include "Angel.h"

const float SIDE_INC = 0.1f;
const float FOWARD_INC = 0.1f;
const float T_INC = 2 * M_PI / 60;

class Camera {
private:
    bool orthograpic = false;
public:
    vec4 eye, u, v, n, t, at;
    bool allowMove = true;
    bool allowPick = false;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 cameraMatrix;
    void calculateMatrix();
    Camera(vec4 location = vec4(0,0,0,1), vec4 up = vec4(0,1,0,1));
    void togglePerspective();
    void moveFoward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void rollClockwise();
    void rollCounterClockwise();
    void pitchUp();
    void pitchDown();
    void yawCounterClockwise();
    void yawClockwise();
    vec3 getEye();
};

#endif

