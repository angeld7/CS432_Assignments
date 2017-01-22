#include "Angel.h"

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    GLuint color_loc;
    vec2* points;
    int numPoints;
    vec4 color;
public:
    Shape(std::string vShader, std::string fShader);
    void init();
    void setPoints(vec2* points, int numPoints);
    void setColor(vec4 color);
    void display();
    void deleteBuffer();
};
