#version 150

in vec3 textureCoor;

uniform samplerCube skybox;

out vec4 FragColor;

void main() 
{
    FragColor = texture(skybox,textureCoor);
    FragColor.a = 1;
}

