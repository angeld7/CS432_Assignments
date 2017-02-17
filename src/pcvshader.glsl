#version 150
in vec3 vPosition;
in vec3 vColor;
flat out vec4 color;
uniform mat4 matrix;
uniform float brightness;
void main(){
    gl_Position = matrix * vec4(vPosition,1);
    color = vec4(vColor * brightness,brightness);
}
