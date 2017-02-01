#version 150
in vec2 vPosition;
in vec3 vColor;
out vec4 color;
uniform mat3 matrix;
uniform float brightness;
void main(){
    gl_Position = vec4((matrix * vec3(vPosition,1)),1);
    color = vec4(vColor * brightness,brightness);
}
