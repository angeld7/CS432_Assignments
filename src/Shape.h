#include "Angel.h"

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    vec2* points;
    vec4* colors;
    int numPoints;
public:
    Shape(std::string vShader, std::string fShader);
    void init();
    void setPoints(vec2* points, int numPoints);
    void setColor(vec4 color);
    void setColors(vec4* colors);
    void setRandomColors();
    void display();
    void deleteBuffer();
};
