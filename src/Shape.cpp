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
    
    size_t pointsSize = sizeof(vec2) * numPoints;
    size_t colorSize = sizeof(vec4) * numPoints;
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
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize) );
}


void Shape::setPoints(Angel::vec2 *points, int numPoints) {
    Shape::points = points;
    Shape::numPoints = numPoints;
    mat3 m(1.0,0,0,0,1.0,0,0,0,1);
    GLuint rotMatrixLoc = glGetUniformLocation(program, "rot_matrix");
    glUniformMatrix3fv(rotMatrixLoc,1, GL_TRUE,m);
}

void Shape::setColor(Angel::vec4 color) {
    colors = new vec4[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = color;
    }
}

float rand255() {
    return (rand() % 255) / 255.0f;
}

void Shape::setColors(Angel::vec4 *colors) {
    Shape::colors = colors;
}

void Shape::setRandomColors() {
    srand(time(NULL));
    colors = new vec4[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = vec4(rand255(),rand255(),rand255(),1.0);
    }
}

void Shape::display() {
    glUseProgram( program );
    glBindVertexArray(VAO);
    glDrawArrays( GL_TRIANGLE_FAN, 0, numPoints );
}

void Shape::deleteBuffer(){
    glDeleteBuffers(1, &VBO);
}

void Shape::rotate(float theta){
    mat3 r(cos(theta),-sin(theta),0,sin(theta),cos(theta),0,0,0,1);
    glUseProgram( program );
    GLuint rotMatrixLoc = glGetUniformLocation(program, "rot_matrix");
    glUniformMatrix3fv(rotMatrixLoc,1, GL_TRUE,r);
    
}
