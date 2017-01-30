#version 150
in vec3 vPosition;
in vec4 vColor;
out vec4 color;
uniform mat3 rot_matrix;
void main(){
    
    mat3 t1;
    t1[0] = vec3(1,0,vPosition.x);
    t1[1] = vec3(0,1,vPosition.y);
    t1[2] = vec3(0,0,1);
    
    mat3 t2;
    t2[0] = vec3(1,0,-vPosition.x);
    t2[1] = vec3(0,1,-vPosition.y);
    t2[2] = vec3(0,0,1);
    
    vec3 pos = t1 * rot_matrix * t2 * vPosition;
    
    gl_Position = vec4(pos.xy,0,1);
    color = vColor;
}
