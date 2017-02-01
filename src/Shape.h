#include "Angel.h"
#include <math.h>

const float TWO_PI = 2 * M_PI;

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    vec2* points;
    vec3* colors;
    int numPoints;
    float rotation = 0;
    float brightness = 0;
public:
    vec2 center;
    Shape(std::string vShader, std::string fShader);
    void init();
    void setPoints(vec2* points, int numPoints);
    void setColor(vec3 color);
    void setColors(vec3* colors);
    void setRandomColors();
    void display();
    void deleteBuffer();
    void rotate(float theta);
    void increaseBrightness(float brightness);
};
