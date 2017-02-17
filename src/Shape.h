#ifndef _SHAPE_H
#define _SHAPE_H

#include "Angel.h"
#include "Camera.h"
#include <math.h>

const float TWO_PI = 2 * M_PI;

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    vec3* points;
    vec3* colors;
    mat4 modelMatrix;
    int numPoints;
    float rotation = 0;
    float brightness = 0;
public:
    vec3 center;
    Shape(std::string vShader, std::string fShader);
    virtual void init();
    virtual void setPoints(vec3* points, int numPoints);
    virtual void setColor(vec3 color);
    virtual void setColors(vec3* colors);
    virtual void setRandomColors();
    virtual void display(Camera camera);
    virtual void deleteBuffer();
    virtual void rotate(float theta);
    virtual void increaseBrightness(float brightness);
};

#endif
