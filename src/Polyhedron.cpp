#include "Polyhedron.h"

Polyhedron::Polyhedron(vec3 center, int sides, float length, float radius)
: Shape("pcvshader.glsl","pcfshader.glsl"){
    Shape::center = center;
    int num = sides;
    vec3* side1 = new vec3[num];
    vec3* side2 = new vec3[num];
    float inc = TWO_PI / num;
    float angle = 0;
    for(int x = 0; x < num; x++) {
        side1[x] = vec3(radius * sin(angle) + center.x, radius * cos(angle) + center.y, center.z + length/2);
        side2[x] = vec3(radius * sin(angle) + center.x, radius * cos(angle) + center.y, center.z - length/2);
        angle += inc;
    }
    
    numPoints = num * 12;
    points = new vec3[numPoints];
    
    int i = 0;
    for(int x = 0; x < num; x++) {
        int next = (x + 1) % num;
        
        points[i++] = side1[x];
        points[i++] = side1[next];
        points[i++] = side2[x];
        
        points[i++] = side1[x];
        points[i++] = side1[next];
        points[i++] = vec3(center.x, center.y, center.z + length/2);
        
        points[i++] = side2[x];
        points[i++] = side2[next];
        points[i++] = side1[next];
        
        points[i++] = side2[x];
        points[i++] = side2[next];
        points[i++] = vec3(center.x, center.y, center.z - length/2);
    }
    
    setRandomColors();
}
