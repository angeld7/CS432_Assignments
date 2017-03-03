#version 150

in vec4 vPosition;
in vec3 vNormal;

out vec3 fN0;
out vec3 fE0;
out vec3 fL0;

out vec3 fN1;
out vec3 fE1;
out vec3 fL1;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

uniform bool lightOn0;
uniform vec4 lightPos0;

uniform bool lightOn1;
uniform vec4 lightPos1;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;


uniform vec4 lightPos;

void main() 
{
    vec3 pos = (camera_matrix*model_matrix*vPosition).xyz;
    
    vec3 lightPosInCam = (camera_matrix*lightPos0).xyz;
    
    fN0 = normalize(camera_matrix*model_matrix*vec4(vNormal,0)).xyz;
    
    fE0 = normalize(vec3(0,0,0)-pos);
    
    fL0 = normalize(lightPosInCam.xyz-pos);
    
    lightPosInCam = (camera_matrix*lightPos1).xyz;
    
    fN1 = normalize(camera_matrix*model_matrix*vec4(vNormal,0)).xyz;
    
    fE1 = normalize(vec3(0,0,0)-pos);
    
    fL1 = fE1;
    
    gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;
}
