#version 150

in vec3 vPosition;

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

out vec3 textureCoor;

void main() 
{
    gl_Position = proj_matrix*view_matrix*vec4(vPosition,1.0);
    textureCoor = normalize(vPosition);
}
