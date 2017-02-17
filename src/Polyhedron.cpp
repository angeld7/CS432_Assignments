#include "Polyhedron.h"

void Polyhedron::init(){
    
}

void Polyhedron::rotate(float theta){
    rotation += theta;
    
    mat4 t1(vec4(1.0,0,0,center.x),
            vec4(0,1.0,0,center.y),
            vec4(0,0,1.0,center.z),
            vec4(0,0,0,1.0));
    
    mat4 rot(vec4(cos(rotation),0,sin(rotation),0),
             vec4(0,1.0,0,0),
             vec4(-sin(rotation),0,cos(rotation),0),
             vec4(0,0,0,1.0));
    
    mat4 t2(vec4(1.0,0,0,-center.x),
            vec4(0,1.0,0,-center.y),
            vec4(0,0,1.0,-center.z),
            vec4(0,0,0,1.0));
    
    mat4 m = t1 * rot * t2;
    
    glUseProgram(program);
    GLuint matrix = glGetUniformLocation(program, "matrix");
    glUniformMatrix3fv(matrix,1,GL_TRUE,m);
}
