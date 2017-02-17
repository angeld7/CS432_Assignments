#ifndef _CAMERA_H
#define _CAMERA_H

#include "Angel.h"

const float SIDE_INC = 0.1f;
const float FOWARD_INC = 0.1f;
const float T_INC = 2 * M_PI / 60;

class Camera {
private:
    bool orthograpic = false;
    vec4 eye, u, v, n, t;
public:
    mat4 viewMatrix;
    void calculateMatrix();
    Camera();
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
};

#endif

