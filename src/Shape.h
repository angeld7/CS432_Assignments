#ifndef _SHAPE_H
#define _SHAPE_H

#include "Angel.h"
#include "Camera.h"
#include <math.h>

const float TWO_PI = 2 * M_PI;
const std::string DEFAULT_V_SHADER = "pcvshader.glsl";
const std::string DEFAULT_F_SHADER = "pcfshader.glsl";

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    vec3* points;
    vec3* colors;
    vec3 multVec4Mat(mat4 m, vec3 v);
    int numPoints;
    float rotation = 0;
    float brightness = 0;
public:
    mat4 modelMatrix;
    vec3 center;
    Shape(std::string vShader = DEFAULT_V_SHADER, std::string fShader = DEFAULT_F_SHADER);
    virtual void init();
    virtual void setPoints(vec3* points, int numPoints);
    virtual void setColor(vec3 color);
    virtual void setColors(vec3* colors);
    virtual void setRandomColors();
    virtual void display(Camera* camera);
    virtual void deleteBuffer();
    virtual void rotate(float theta);
    virtual void increaseBrightness(float brightness);
    virtual float checkCollisionPoly(vec4 ray, vec3 eye, vec3* collisionPoints);
    static vec3 normalize(vec3 vec);
    static bool gluInvertMatrix(const double m[16], double invOut[16]);
};

#endif
