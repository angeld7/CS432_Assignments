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
    glUniform1f(brightnessLoc, brightness);
    
    GLuint lineColor = glGetUniformLocation(program, "lineColor");
    glUniform4fv(lineColor, 1, vec4(0,0,0,1));
    
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

void Shape::display(Camera camera) {
    glUseProgram( program );
    glBindVertexArray(VAO);
    
    mat4 m = camera.viewMatrix * modelMatrix;
    
    GLuint matrix = glGetUniformLocation(program, "matrix");
    glUniformMatrix4fv(matrix,1,GL_TRUE,m);
    
    glDrawArrays( GL_TRIANGLES, 0, numPoints );
    
    glLineWidth(5);
    GLuint drawline = glGetUniformLocation(program, "line");
    glUniform1i(drawline,1);
    glDrawArrays( GL_LINES, 0, numPoints );
    glUniform1i(drawline,0);
    
    //glDrawArrays( GL_LINES, 0, numPoints );
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
