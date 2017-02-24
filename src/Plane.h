#ifndef _PLANE_H
#define _PLANE_H

#include "Shape.h"

class Plane: public Shape{
public:
    Plane(vec3 center, float width, float height);
};

#endif
