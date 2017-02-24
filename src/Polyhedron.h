#ifndef _POLYHEDRON_H
#define _POLYHEDRON_H

#include "Shape.h"

class Polyhedron:public Shape{
public:
    Polyhedron(vec3 center, int sides, float length, float radius);
};

#endif
