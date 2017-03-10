#include "Skybox.h"

Skybox::Skybox()
: Shape("vshader00_v150.glsl","fshader00_v150.glsl"){
    numPoints = 36;
    points = new vec3[numPoints]{
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3( 1.0f, -1.0f, -1.0f),
        vec3(1.0f,  1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f, -1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f, -1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f, -1.0f,  1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(1.0f,  1.0f, -1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f,  1.0f)
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) * numPoints, points, GL_STATIC_DRAW);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    glGenTextures(1, &textureID);
    
    int width = 512, height = 512;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    GLubyte *skyTop = ppmRead("dispair-ridge_up.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTop);
    GLubyte *skyBottom = ppmRead("dispair-ridge_dn.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBottom);
    GLubyte *skyRight = ppmRead("dispair-ridge_rt.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyRight);
    GLubyte *skyLeft = ppmRead("dispair-ridge_lf.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyLeft);
    GLubyte *skyFront = ppmRead("dispair-ridge_ft.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyFront);
    GLubyte *skyBack = ppmRead("dispair-ridge_bk.ppm", &width, &height);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBack);
    
    
}

void Skybox::init(){
    
}

void Skybox::display(Camera* camera) {
    glUseProgram( program );
    glBindVertexArray(VAO);
    
    GLuint view_matrix = glGetUniformLocation(program, "view_matrix");
    vec4 skyboxEye = vec4(0,0,0,1);
    glUniformMatrix4fv(view_matrix,1,GL_TRUE,LookAt(skyboxEye, skyboxEye - camera->n, camera->v));
    
    GLuint proj_matrix = glGetUniformLocation(program, "proj_matrix");
    glUniformMatrix4fv(proj_matrix,1,GL_TRUE,camera->projectionMatrix);
    
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glUniform1i(glGetUniformLocation(program, "skybox"),0);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    glDrawArrays( GL_TRIANGLES, 0, numPoints );
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
}
