#version 150

in vec4 vPosition;
in vec2 vTexture;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

out vec2 textureCoor;

void main() 
{
    gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;
    textureCoor = vTexture;
}
