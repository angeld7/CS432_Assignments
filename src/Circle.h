#ifndef _CIRCLE_H
#define _CIRCLE_H

#include "Shape.h"

class Circle:public Shape{
public:
    Circle(std::string vShader, std::string fShader, float xCenter, float yCenter, float radius);
};

#endif
