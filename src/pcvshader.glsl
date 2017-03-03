#version 150
in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

out vec4 color;
uniform vec4 lineColor;

uniform bool line;

uniform mat4 model_matrix,camera_matrix,proj_matrix;

uniform vec4 lightPos0;
uniform vec4 lightAmbient0, lightDiffuse0, lightSpecular0;
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

void main(){
    if(line) {
        color = lineColor;
    } else {
        vec3 pos = (camera_matrix*model_matrix*vec4(vPosition,1)).xyz;
        
        vec3 lightPosInCam = (camera_matrix*lightPos0).xyz;
        
        vec3 L = normalize(lightPosInCam.xyz-pos);
        float dist = 1.0;
        
        vec3 E = normalize(vec3(0,0,0)-pos);
        
        vec3 N = normalize(camera_matrix*model_matrix*vec4(vNormal,0)).xyz;
        vec3 H = normalize(L+E);
        
        vec4 ambient = lightAmbient0*matAmbient;
        
        float Kd = max(dot(L,N),0.0);
        vec4 diffuse = Kd*lightDiffuse0*matDiffuse;
        
        float Ks = pow(max(dot(N,H),0.0),matAlpha);
        vec4 specular = Ks*lightSpecular0*matSpecular;
        if(dot(L,N)<0.0)
            specular = vec4(0,0,0,1);
        
        color = (ambient+diffuse+specular)*(1/pow(dist,2));
        //color = diffuse;
        color.a = 1.0;
    }
    
    gl_Position = proj_matrix*camera_matrix*model_matrix*vec4(vPosition,1);
}
