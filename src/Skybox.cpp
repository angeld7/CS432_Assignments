#include "Skybox.h"

Skybox::Skybox()
:Polyhedron(vec3(0,0,0), 4, 10, 10){
    textured = true;
    texturePoints = new vec2[numPoints];
    vec2* sides = new vec2[4] {
        vec2(0,1), vec2(1,1), vec2(1,0), vec2(0,0)
    };
    vec2 mid = vec2(.5,.5);
    int c = 0;
    for(int i=0;i<numPoints;) {
        int n = (c + 1) % 4;
        texturePoints[i++] = sides[0];
        texturePoints[i++] = sides[3];
        texturePoints[i++] = sides[1];
        
        texturePoints[i++] = sides[c];
        texturePoints[i++] = sides[n];
        texturePoints[i++] = mid;
        
        texturePoints[i++] = sides[1];
        texturePoints[i++] = sides[2];
        texturePoints[i++] = sides[3];
        
        texturePoints[i++] = sides[c];
        texturePoints[i++] = sides[n];
        texturePoints[i++] = mid;
        
        c++;
    }
}
