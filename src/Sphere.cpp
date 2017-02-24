#include "Sphere.h"

Sphere::Sphere(vec3 center, float radius, int smoothness)
: Shape("pcvshader.glsl","pcfshader.glsl"){
    Shape::center = center;
    Sphere::radius = radius;
    numPoints = pow(4,smoothness+1) * 3;
    points = new vec3[numPoints];
    vec3 v[4] = {
        vec3(0,0,1),
        vec3(0,2 * sqrt2 / 3, -1.0f / 3.0f),
        vec3(-sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f),
        vec3(sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f /3.0f)
    };
    
    divideTriangle(v[0], v[1], v[2], smoothness);
    divideTriangle(v[3], v[2], v[1], smoothness);
    divideTriangle(v[1], v[3], v[0], smoothness);
    divideTriangle(v[2], v[0], v[3], smoothness);
}

void Sphere::divideTriangle(vec3 a, vec3 b, vec3 c, int count){
    if (count > 0) {
        vec3 v1 = normalize(a + b);
        vec3 v2 = normalize(a + c);
        vec3 v3 = normalize(b + c);
        
        divideTriangle(a, v1, v2, count - 1);
        divideTriangle(c, v2, v3, count - 1);
        divideTriangle(b, v3, v1, count - 1);
        divideTriangle(v1, v3, v2, count - 1);
    } else {
        points[index++] = a * radius + center;
        points[index++] = b * radius + center;
        points[index++] = c * radius + center;
    }
}
