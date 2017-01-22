#include "Shape.h"

class Circle:public Shape{
public:
    Circle(std::string vShader, std::string fShader, float xCenter, float yCenter, float radius);
};
