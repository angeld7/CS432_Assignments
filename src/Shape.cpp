//
//  Shape.cpp
//  CS432_Assignments
//
//  Created by Angel Delgado on 1/21/17.
//  Copyright © 2017 Drexel Uneversity. All rights reserved.
//

#include "Shape.h"


Shape::Shape(std::string vShader, std::string fShader){
    program = InitShader( vShader.c_str(), fShader.c_str() );
}

void Shape::init(){
    // Create and initialize a buffer object
    glGenBuffers( 1, &VBO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec2) * numPoints, Shape::points, GL_STATIC_DRAW );
    
    // Load shaders and use the resulting shader program
    glUseProgram( program );
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    
    //Set up VAO
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    color_loc = glGetUniformLocation(program, "color");
    
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void Shape::setPoints(Angel::vec2 *points, int numPoints) {
    Shape::points = points;
    Shape::numPoints = numPoints;
}

void Shape::setColor(Angel::vec4 color) {
    Shape::color = color;
}

void Shape::display() {
    glUseProgram( program );
    glBindVertexArray(VAO);
    
    glUniform4fv(color_loc, 1, color);
    glDrawArrays( GL_TRIANGLE_FAN, 0, numPoints );
}

void Shape::deleteBuffer(){
    glDeleteBuffers(1, &VBO);
}
