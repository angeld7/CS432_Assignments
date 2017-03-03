#include "Polyhedron.h"

Polyhedron::Polyhedron(vec3 center, int sides, float length, float radius)
//: Shape("pcvshader.glsl","pcfshader.glsl"){
: Shape("vshader00_v150.glsl","fshader00_v150.glsl"){
    Shape::center = center;
    int num = sides;
    vec3* side1 = new vec3[num];
    vec3* side2 = new vec3[num];
    float inc = TWO_PI / num;
    float angle = M_PI / 4;
    for(int x = 0; x < num; x++) {
        side1[x] = vec3(radius * sin(angle), radius * cos(angle),length/2);
        side2[x] = vec3(radius * sin(angle), radius * cos(angle),-length/2);
        angle += inc;
    }
    
    numPoints = num * 12;
    points = new vec3[numPoints];
    normals = new vec3[numPoints];
    vec3 NF = vec3(0,0,1);
    vec3 NB = vec3(0,0,-1);
    angle = M_PI/8;
    int i = 0;
    for(int x = 0; x < num; x++) {
        int next = (x + 1) % num;
        angle += inc;
        vec3 N1 = normalize(vec3(sin(angle), cos(angle),0));
        
        normals[i] = N1;
        points[i++] = side1[x];
        normals[i] = N1;
        points[i++] = side1[next];
        normals[i] = N1;
        points[i++] = side2[x];
        
        normals[i] = NF;
        points[i++] = side1[x];
        normals[i] = NF;
        points[i++] = side1[next];
        normals[i] = NF;
        points[i++] = vec3(0, 0, length/2);
        
        normals[i] = N1;
        points[i++] = side2[x];
        normals[i] = N1;
        points[i++] = side2[next];
        normals[i] = N1;
        points[i++] = side1[next];
        
        normals[i] = NB;
        points[i++] = side2[x];
        normals[i] = NB;
        points[i++] = side2[next];
        normals[i] = NB;
        points[i++] = vec3(0, 0, -length/2);
    }
    
    modelMatrix = Translate(center.x, center.y, center.z);
}
