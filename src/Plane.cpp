#include "Plane.h"


Plane::Plane(vec3 center, float width, float height)
: Shape("v_texture.glsl","f_texture.glsl"){
    numPoints = 6;
    Shape::center = center;
    
    vec3 p1 = vec3(width/2, 0, height/2);
    vec3 p2 = vec3(width/2, 0, -height/2);
    vec3 p3 = vec3(-width/2, 0, -height/2);
    vec3 p4 = vec3(-width/2, 0, height/2);
    
    points = new vec3[numPoints] {
        p1,p2,p3,p3,p4,p1
    };
    
    vec3 N1 = normalize(cross(p2-p1,p3-p1));
    vec3 N2 = normalize(cross(p4-p3,p1-p3));
    
    normals = new vec3[numPoints] {
        N1,N1,N1,N2,N2,N2
    };
    
    texturePoints = new vec2[numPoints]{
        vec2(0,0),vec2(0,1),vec2(1,1),vec2(1,1),vec2(1,0),vec2(0,0)
    };
    
    textured = true;
    
    modelMatrix = Translate(center.x, center.y, center.z);
}
