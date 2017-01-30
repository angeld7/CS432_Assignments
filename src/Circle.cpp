#include "Circle.h"

Circle::Circle(std::string vShader, std::string fShader, float xCenter, float yCenter, float radius)
: Shape(vShader, fShader)
{
    numPoints = 65;
    points = new vec2[numPoints];
    float inc = (2 * M_PI) / (numPoints - 2);
    points[0] = vec2(xCenter, yCenter);
    float angle = 0;
    for(int x = 1; x <= numPoints; x++) {
        points[x] = vec2(radius * sin(angle) + xCenter, radius * cos(angle) + yCenter);
        angle += inc;
    }
}
