#ifndef _TEXTUREDCUBE_H
#define _TEXTUREDCUBE_H

#include "Polyhedron.h"

class TexturedCube:public Polyhedron{
public:
    TexturedCube(vec3 center, float length, float radius);
};

#endif
