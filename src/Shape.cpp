//
//  Shape.cpp
//  CS432_Assignments
//
//  Created by Angel Delgado on 1/21/17.
//  Copyright © 2017 Drexel Uneversity. All rights reserved.
//

#include "Shape.h"
#include <stdlib.h>

Shape::Shape(std::string vShader, std::string fShader){
    program = InitShader( vShader.c_str(), fShader.c_str() );
}

void Shape::init(){
    // Create and initialize a buffer object
    glGenBuffers( 1, &VBO );
    
    size_t pointsSize = sizeof(vec3) * numPoints;
    size_t colorSize = sizeof(vec3) * numPoints;
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, pointsSize + colorSize, NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER,0,pointsSize,points);
    glBufferSubData(GL_ARRAY_BUFFER,pointsSize,colorSize, colors);
    
    // Load shaders and use the resulting shader program
    glUseProgram( program );
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    
    //Set up VAO
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize) );
    
    GLuint brightnessLoc = glGetUniformLocation(program, "brightness");
    glUniform1f(brightnessLoc, 1.0);
    
    GLuint lineColor = glGetUniformLocation(program, "lineColor");
    glUniform4fv(lineColor, 1, colors[0]);
    
    GLuint drawline = glGetUniformLocation(program, "line");
    glUniform1i(drawline,0);

    modelMatrix = mat4(1.0f);
}


void Shape::setPoints(Angel::vec3 *points, int numPoints) {
    Shape::points = points;
    Shape::numPoints = numPoints;
}



void Shape::setColor(Angel::vec3 color) {
    colors = new vec3[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = color;
    }
}

float rand255() {
    return (rand() % 255) / 255.0f;
}

void Shape::setColors(Angel::vec3 *colors) {
    Shape::colors = colors;
}

void Shape::setRandomColors() {
    srand((int)time(NULL));
    colors = new vec3[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = vec3(rand255(),rand255(),rand255());
    }
}

void Shape::display(Camera* camera) {
    glUseProgram( program );
    glBindVertexArray(VAO);
    
    mat4 m = camera->viewMatrix * modelMatrix;
    
    GLuint matrix = glGetUniformLocation(program, "matrix");
    glUniformMatrix4fv(matrix,1,GL_TRUE,m);
    
    glDrawArrays( GL_TRIANGLES, 0, numPoints );
    
//    glLineWidth(5);
//    GLuint drawline = glGetUniformLocation(program, "line");
//    glUniform1i(drawline,1);
//    glDrawArrays( GL_LINES, 0, numPoints );
//    glUniform1i(drawline,0);
}

void Shape::deleteBuffer(){
    glDeleteBuffers(1, &VBO);
}

void Shape::rotate(float theta){
    
    rotation += theta;
    
    mat4 t1 = Translate(center.x,center.y,center.z);
    
    mat4 rot = RotateY(rotation);
    
    mat4 t2 = Translate(-center.x,-center.y,-center.z);
    
    modelMatrix = t1 * rot * t2;
    
}

void Shape::increaseBrightness(float inc) {
    brightness += inc;
    if(brightness > 1) brightness = 1;
    glUseProgram(program);
    GLuint brightnessLoc = glGetUniformLocation(program, "brightness");
    glUniform1f(brightnessLoc, brightness);
}

vec3 Shape::normalize(vec3 vec){
    float len = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
    vec3 t;
    if (len > DivideByZeroTolerance) {
        t = vec / sqrt(len);
    }
    return t;
}

vec3 Shape::multVec4Mat(mat4 m, vec3 v) {
    vec4 v1 = m * vec4(v.x,v.y,v.z,1.0);
    return vec3(v1.x,v1.y,v1.z);
}

float Shape::checkCollisionPoly(vec4 ray, vec3 eye, vec3* collisionPoints){
    if (numPoints < 3) return false;
    vec3 D = normalize(vec3(ray.x, ray.y, ray.z));
    bool found = false;
    float t = INT_MAX;
    for(int i = 0; i < numPoints; i+=3){
        vec3* triangle = new vec3[3] {
            multVec4Mat(modelMatrix,points[i]),
            multVec4Mat(modelMatrix,points[i+1]),
            multVec4Mat(modelMatrix,points[i+2])
        };
        vec3 E = triangle[0];
        vec3 F = triangle[1];
        vec3 G = triangle[2];

        vec3 N = cross(F-E,G-E);
        
        N = normalize(N);
        
        float d = dot(-E,N);
        float t2 = -(dot(N,eye) + d) / dot(N,D);
        
        if(t < t2 || t2 < 0 || dot(N,D) == 0) continue;
        
        vec3 P = eye + t2 * D;
        bool hit = true;
        for(int x = 0; x < 3; x++) {
            vec3 v1 = triangle[(x+1)%3] - triangle[x];
            vec3 v2 = P - triangle[x];
            if(dot(N,cross(v1,v2)) < 0) {
                hit = false;
                break;
            }
        }
        if(hit) {
            collisionPoints[0] = points[i];
            collisionPoints[1] = points[i+1];
            collisionPoints[2] = points[i+2];
            t = t2;
            found = true;
        }
    }
    return t;
}

bool Shape::gluInvertMatrix(const double m[16], double invOut[16])
{
    double inv[16], det;
    int i;
    
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
    
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
    
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
    
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
    
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
    
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
    
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
    
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
    
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
    
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
    
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
    
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
    
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
    
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
    
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
    
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
    
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if (det == 0)
        return false;
    
    det = 1.0 / det;
    
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    
    return true;
}
