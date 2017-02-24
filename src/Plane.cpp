#include "Plane.h"


Plane::Plane(vec3 center, float width, float height)
: Shape("pcvshader.glsl","pcfshader.glsl"){
    numPoints = 6;
    
    vec3 p1 = vec3(center.x + width/2, center.y, center.z + height/2);
    vec3 p2 = vec3(center.x + width/2, center.y, center.z - height/2);
    vec3 p3 = vec3(center.x - width/2, center.y, center.z - height/2);
    vec3 p4 = vec3(center.x - width/2, center.y, center.z + height/2);
    
    points = new vec3[numPoints] {
        p1,p2,p3,p3,p4,p1
    };
}
