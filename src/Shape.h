#ifndef _SHAPE_H
#define _SHAPE_H

#include "Angel.h"
#include "Camera.h"
#include "Light.h"

#include <math.h>
#include <vector>

const float TWO_PI = 2 * M_PI;
const std::string DEFAULT_V_SHADER = "pcvshader.glsl";
const std::string DEFAULT_F_SHADER = "pcfshader.glsl";

class Shape {
protected:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    vec4 diffuse, spec, ambient;
    vec3* points;
    vec3* normals;
    vec3* colors;
    vec2* texturePoints;
    vec3 multVec4Mat(mat4 m, vec3 v);
    int numPoints;
    float rotation = 0;
    float shine;
    std::vector<GLuint> textures;
    void assignGouradVerticies();
    unsigned char* ppmRead(char* filename, int* width, int* height);
    bool textured = false;
    int curTexture = 0, numTextures = 0;
public:
    bool hide = false;
    bool dPoints = false;
    mat4 modelMatrix = mat4(1.0f);
    vec3 center;
    Shape(std::string vShader = DEFAULT_V_SHADER, std::string fShader = DEFAULT_F_SHADER);
    virtual void init();
    virtual void setPoints(vec3* points, int numPoints);
    virtual void setColor(vec3 color);
    virtual void setColors(vec3* colors);
    virtual void setRandomColors();
    virtual void display(Camera* camera, std::vector<Light> lights);
    virtual void deleteBuffer();
    virtual void rotate(float theta);
    virtual float checkCollisionPoly(vec4 ray, vec3 eye, vec3* collisionPoints);
    void setMaterial(vec4 diffuseLoc, vec4 specLoc, vec4 ambientLoc, float shine);
    static vec3 normalize(vec3 vec);
    static bool gluInvertMatrix(const double m[16], double invOut[16]);
    void toggleTexture();
    void addTexture(char* filename, int width, int height);
};

#endif
