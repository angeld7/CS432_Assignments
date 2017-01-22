#include "Circle.h"
#include "Math.h"

Circle::Circle(std::string vShader, std::string fShader, float xCenter, float yCenter, float radius)
: Shape(vShader, fShader)
{
    numPoints = 256;
    points = new vec2[numPoints];
    float end = 2 * M_PI;
    float inc = end / numPoints;
    int x = 0;
    for(float angle = 0.0; angle < end; angle += inc) {
        points[x++] = vec2(radius * sin(angle) + xCenter, radius * cos(angle) + yCenter);
    }
}
