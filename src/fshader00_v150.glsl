#version 150

in vec2 textureCoor;

uniform sampler2D textureID;

out vec4 FragColor;

void main() 
{
    FragColor = texture(textureID,textureCoor);
    FragColor.a = 1;
}

