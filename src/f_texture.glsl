#version 150

in vec3 fN0;
in vec3 fL0;
in vec3 fE0;

in vec3 fN1;
in vec3 fL1;
in vec3 fE1;

in vec2 textureCoor;

uniform bool lightOn0;
uniform vec4 lightAmbient0, lightDiffuse0, lightSpecular0;

uniform bool lightOn1;
uniform vec4 lightDir1;
uniform vec4 lightAmbient1, lightDiffuse1, lightSpecular1;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

uniform sampler2D textureID;

out vec4 FragColor;

void main() 
{
    if(lightOn0) {
        vec3 N = normalize(fN0);
        vec3 E = normalize(fE0);
        vec3 L = normalize(fL0);
        
        vec3 H = normalize(L+E);
        
        vec4 ambient = lightAmbient0*matAmbient;
        
        float Kd = max(dot(L,N),0.0);
        vec4 diffuse = Kd*lightDiffuse0*matDiffuse;
        
        float Ks = pow(max(dot(N,H),0.0),matAlpha);
        vec4 spec = Ks*lightSpecular0*matSpecular;
        
        if(dot(L,N)<0.0)
            spec = vec4(0,0,0,1);
        
        FragColor = ambient + diffuse + spec;
    } else {
        FragColor = lightAmbient0*matAmbient;
    }
    if(lightOn1) {
        vec3 N = normalize(fN1);
        vec3 E = normalize(fE1);
        vec3 L = normalize(fL1);
        
        vec3 dir = normalize(lightDir1).xyz;
        
        float u = abs(acos(dot(L, dir)));
        
        if(u < radians(25)){
            vec3 H = normalize(L+E);
            
            vec4 ambient = lightAmbient1*matAmbient;
            
            float Kd = max(dot(L,N),0.0);
            vec4 diffuse = Kd*lightDiffuse1*matDiffuse;
            
            float Ks = pow(max(dot(N,H),0.0),matAlpha);
            vec4 spec = Ks*lightSpecular1*matSpecular;
            
            if(dot(L,N)<0.0)
                spec = vec4(0,0,0,1);
            
            FragColor = FragColor + ambient + diffuse + spec;
        }
    }
    FragColor = FragColor * texture(textureID,textureCoor);
    FragColor.a = 1.0;
}

