#ifndef _SPHERE_H
#define _SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
private:
    int index = 0;
    float sqrt2 = (float)sqrt(2.0);
    float sqrt6 = (float)sqrt(6.0);
    float radius = 1.0;
    void divideTriangle(vec3 a, vec3 b, vec3 c, int count);
public:
    Sphere(vec3 center, float radius, int smoothness);
};

#endif
