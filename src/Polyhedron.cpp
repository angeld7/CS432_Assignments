#include "Polyhedron.h"

Polyhedron::Polyhedron()
: Shape("pcvshader.glsl","pcfshader.glsl"){
    center = vec3(0,0,-2);
    int num = 7;
    float radius = 0.5f;
    vec3* side1 = new vec3[num];
    vec3* side2 = new vec3[num];
    float inc = TWO_PI / num;
    float angle = 0;
    for(int x = 0; x < num; x++) {
        side1[x] = vec3(radius * sin(angle) + center.x, radius * cos(angle) + center.y, center.z + 1);
        side2[x] = vec3(radius * sin(angle) + center.x, radius * cos(angle) + center.y, center.z - 1);
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
        points[i++] = vec3(center.x, center.y, center.z + 1);
        
        points[i++] = side2[x];
        points[i++] = side2[next];
        points[i++] = side1[next];
        
        points[i++] = side2[x];
        points[i++] = side2[next];
        points[i++] = vec3(center.x, center.y, center.z - 1);
    }
    
    increaseBrightness(1.0);
    setRandomColors();
}
