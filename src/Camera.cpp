#include "Camera.h"

void Camera::calculateMatrix() {
    float width = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);
    mat4 projection;
    if(orthograpic) {
        projection = Ortho(-1, 1, -1, 1, 1.0, 3.0);
        
    } else {
        projection = Perspective(65.0, width/height, 1, 100);
    }
    mat4 c = mat4(u, v, n, t);
    mat4 view = c * Translate( -eye );
    viewMatrix = projection * view * mat4(1.0);
}

Camera::Camera(){
    eye = vec3(0,0,.1);
    n = normalize(eye - vec3(0,0,0));
    vec3 uu = normalize(cross(vec3(0,1,0), n));
    u = vec4(uu.x, uu.y, uu.z, 0.0);
    vec3 vv = normalize(cross(n,u));
    v = vec4(vv.x, vv.y, vv.z, 0.0);
    t = vec4(0.0, 0.0, 0.0, 1.0);
    calculateMatrix();
}


void Camera::togglePerspective(){
    orthograpic = !orthograpic;
    calculateMatrix();
}

void Camera::moveFoward(){
    eye -= (n * FOWARD_INC);
    calculateMatrix();
}

void Camera::moveBackward(){
    eye += (n * FOWARD_INC);
    calculateMatrix();
}

void Camera::moveLeft(){
    eye -= (u * SIDE_INC);
    calculateMatrix();
}

void Camera::moveRight(){
    eye += (u * SIDE_INC);
    calculateMatrix();
}

void Camera::rollClockwise(){
    u = (cos(T_INC) * u) - (sin(T_INC) * v);
    v = (sin(T_INC) * u) + (cos(T_INC) * v);
    calculateMatrix();
}

void Camera::rollCounterClockwise(){
    u = (cos(-T_INC) * u) - (sin(-T_INC) * v);
    v = (sin(-T_INC) * u) + (cos(-T_INC) * v);
    calculateMatrix();
}

void Camera::pitchUp(){
    v = (cos(T_INC) * v) - (sin(T_INC) * n);
    n = (sin(T_INC) * v) + (cos(T_INC) * n);
    calculateMatrix();
}

void Camera::pitchDown(){
    v = (cos(-T_INC) * v) - (sin(-T_INC) * n);
    n = (sin(-T_INC) * v) + (cos(-T_INC) * n);
    calculateMatrix();
}

void Camera::yawCounterClockwise(){
    u = (cos(-T_INC) * u) - (sin(-T_INC) * n);
    n = (sin(-T_INC) * u) + (cos(-T_INC) * n);
    calculateMatrix();
}

void Camera::yawClockwise(){
    u = (cos(T_INC) * u) - (sin(T_INC) * n);
    n = (sin(T_INC) * u) + (cos(T_INC) * n);
    calculateMatrix();
}
