//
//  Shape.cpp
//  CS432_Assignments
//
//  Created by Angel Delgado on 1/21/17.
//  Copyright © 2017 Drexel Uneversity. All rights reserved.
//

#include "Shape.h"
#include <stdlib.h>

Shape::Shape(std::string vShader, std::string fShader){
    program = InitShader( vShader.c_str(), fShader.c_str() );
}

void Shape::init(){
    // Create and initialize a buffer object
    glGenBuffers( 1, &VBO );
    
    size_t pointsSize = sizeof(vec3) * numPoints;
    size_t normalSize = sizeof(vec3) * numPoints;
    size_t colorSize = sizeof(vec3) * numPoints;
    size_t textureSize = sizeof(vec2) * numPoints;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pointsSize + colorSize + normalSize + textureSize, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,pointsSize,points);
    glBufferSubData(GL_ARRAY_BUFFER,pointsSize,colorSize, colors);
    glBufferSubData(GL_ARRAY_BUFFER,pointsSize+colorSize,normalSize, normals);
    if(textured) glBufferSubData(GL_ARRAY_BUFFER,pointsSize+colorSize+normalSize,textureSize, texturePoints);
    
    // Load shaders and use the resulting shader program
    glUseProgram(program);
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint nPosition = glGetAttribLocation(program, "vNormal");
    GLuint vColor = glGetAttribLocation(program, "vColor");
    
    //Set up VAO
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(nPosition);
    glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize+colorSize));
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize));
    if(textured) {
        GLuint vTexture = glGetAttribLocation(program, "vTexture");
        glEnableVertexAttribArray(vTexture);
        glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(pointsSize+colorSize+normalSize));
    }
    
    GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
    glUniform4fv(diffuse_loc, 1, diffuse);
    
    GLuint spec_loc = glGetUniformLocation(program, "matSpecular");
    glUniform4fv(spec_loc, 1, spec);
    
    GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
    glUniform4fv(ambient_loc, 1, ambient);
    
    GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
    glUniform1f(alpha_loc, shine);
    
    GLuint brightnessLoc = glGetUniformLocation(program, "brightness");
    glUniform1f(brightnessLoc, 1.0);
    
    GLuint lineColor = glGetUniformLocation(program, "lineColor");
    glUniform4fv(lineColor, 1, colors[0]);
    
    GLuint drawline = glGetUniformLocation(program, "line");
    glUniform1i(drawline,0);
}


void Shape::setPoints(Angel::vec3 *points, int numPoints) {
    Shape::points = points;
    Shape::numPoints = numPoints;
}



void Shape::setColor(Angel::vec3 color) {
    colors = new vec3[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = color;
    }
}

float rand255() {
    return (rand() % 255) / 255.0f;
}

void Shape::setColors(Angel::vec3 *colors) {
    Shape::colors = colors;
}

void Shape::setRandomColors() {
    srand((int)time(NULL));
    colors = new vec3[numPoints];
    for(int x = 0; x < numPoints; x++) {
        colors[x] = vec3(rand255(),rand255(),rand255());
    }
}

void Shape::display(Camera* camera, std::vector<Light> lights) {
    if(!hide) {
        glUseProgram( program );
        glBindVertexArray(VAO);
        
        GLuint model_matrix = glGetUniformLocation(program, "model_matrix");
        glUniformMatrix4fv(model_matrix,1,GL_TRUE,modelMatrix);
        GLuint camera_matrix = glGetUniformLocation(program, "camera_matrix");
        glUniformMatrix4fv(camera_matrix,1,GL_TRUE,camera->cameraMatrix);
        GLuint proj_matrix = glGetUniformLocation(program, "proj_matrix");
        glUniformMatrix4fv(proj_matrix,1,GL_TRUE,camera->projectionMatrix);
        
        
        for(int i = 0; i < lights.size(); i++){
            GLuint on_loc = glGetUniformLocation(program, ("lightOn" + std::to_string(i)).c_str());
            glUniform1i(on_loc, lights[i].on);
            GLuint light_loc = glGetUniformLocation(program, ("lightPos" + std::to_string(i)).c_str());
            glUniform4fv(light_loc, 1, lights[i].position);
            GLuint light_dir = glGetUniformLocation(program, ("lightDir" + std::to_string(i)).c_str());
            glUniform4fv(light_dir, 1, lights[i].dir);
            GLuint ambient_loc = glGetUniformLocation(program, ("lightAmbient" + std::to_string(i)).c_str());
            glUniform4fv(ambient_loc, 1, lights[i].ambient);
            GLuint diffuse_loc = glGetUniformLocation(program, ("lightDiffuse" + std::to_string(i)).c_str());
            glUniform4fv(diffuse_loc, 1, lights[i].diffuse);
            GLuint spec_loc = glGetUniformLocation(program, ("lightSpecular" + std::to_string(i)).c_str());
            glUniform4fv(spec_loc, 1, lights[i].specular);
            
        }
        if(dPoints) {
            glPointSize(3.0);
            glDrawArrays( GL_POINTS, 0, numPoints );
        } else {
            if(textured) {
                glEnable(GL_TEXTURE_2D);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[curTexture]);
                glUniform1i(glGetUniformLocation(program, "textureID"), 0);
            }
            
            glDrawArrays( GL_TRIANGLES, 0, numPoints );
        }
        
    //    glLineWidth(5);
    //    GLuint drawline = glGetUniformLocation(program, "line");
    //    glUniform1i(drawline,1);
    //    glDrawArrays( GL_LINES, 0, numPoints );
    //    glUniform1i(drawline,0);
    }
}

void Shape::assignGouradVerticies() {
    normals = new vec3[numPoints];
    vec3* normalSum = new vec3[numPoints];
    int* counts = new int[numPoints];
    
    for(int i=0;i<numPoints;i++) {
        normalSum[i] = vec3(0,0,0);
        counts[i] = 0;
    }
    
    for(int i=0;i<numPoints;i++) {
        int count = 0;
        for(int j=0;j<numPoints;j++) {
            if((points[i].x==points[j].x) &&
               (points[i].y==points[j].y) &&
               (points[i].z==points[j].z)) {
                count++;
                normalSum[i]+=normals[j];
            }
        }
        counts[i]=count;
    }
    
    for(int i=0;i<numPoints;i++) {
        normals[i]=normalSum[i]/counts[i];
    }
}

void Shape::setMaterial(vec4 diffuse, vec4 spec, vec4 ambient, float shine) {
    Shape::diffuse = diffuse;
    Shape::spec = spec;
    Shape::ambient = ambient;
    Shape::shine = shine;
}

void Shape::deleteBuffer(){
    glDeleteBuffers(1, &VBO);
}

void Shape::rotate(float theta){
    
    rotation += theta;
    
    mat4 t1 = Translate(center.x,center.y,center.z);
    
    mat4 rot = RotateY(rotation);
    
    mat4 t2 = Translate(-center.x,-center.y,-center.z);
    
    modelMatrix = t1 * rot * t2;
    
}

vec3 Shape::normalize(vec3 vec){
    float len = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
    vec3 t;
    if (len > DivideByZeroTolerance) {
        t = vec / sqrt(len);
    }
    return t;
}

vec3 Shape::multVec4Mat(mat4 m, vec3 v) {
    vec4 v1 = m * vec4(v.x,v.y,v.z,1.0);
    return vec3(v1.x,v1.y,v1.z);
}

float Shape::checkCollisionPoly(vec4 ray, vec3 eye, vec3* collisionPoints){
    if (numPoints < 3) return false;
    vec3 D = normalize(vec3(ray.x, ray.y, ray.z));
    bool found = false;
    float t = INT_MAX;
    for(int i = 0; i < numPoints; i+=3){
        vec3* triangle = new vec3[3] {
            multVec4Mat(modelMatrix,points[i]),
            multVec4Mat(modelMatrix,points[i+1]),
            multVec4Mat(modelMatrix,points[i+2])
        };
        vec3 E = triangle[0];
        vec3 F = triangle[1];
        vec3 G = triangle[2];

        vec3 N = cross(F-E,G-E);
        
        N = normalize(N);
        
        float d = dot(-E,N);
        float t2 = -(dot(N,eye) + d) / dot(N,D);
        
        if(t < t2 || t2 < 0 || dot(N,D) == 0) continue;
        
        vec3 P = eye + t2 * D;
        bool hit = true;
        for(int x = 0; x < 3; x++) {
            vec3 v1 = triangle[(x+1)%3] - triangle[x];
            vec3 v2 = P - triangle[x];
            if(dot(N,cross(v1,v2)) < 0) {
                hit = false;
                break;
            }
        }
        if(hit) {
            collisionPoints[0] = points[i];
            collisionPoints[1] = points[i+1];
            collisionPoints[2] = points[i+2];
            t = t2;
            found = true;
        }
    }
    return t;
}

bool Shape::gluInvertMatrix(const double m[16], double invOut[16])
{
    double inv[16], det;
    int i;
    
    inv[0] = m[5]  * m[10] * m[15] -
    m[5]  * m[11] * m[14] -
    m[9]  * m[6]  * m[15] +
    m[9]  * m[7]  * m[14] +
    m[13] * m[6]  * m[11] -
    m[13] * m[7]  * m[10];
    
    inv[4] = -m[4]  * m[10] * m[15] +
    m[4]  * m[11] * m[14] +
    m[8]  * m[6]  * m[15] -
    m[8]  * m[7]  * m[14] -
    m[12] * m[6]  * m[11] +
    m[12] * m[7]  * m[10];
    
    inv[8] = m[4]  * m[9] * m[15] -
    m[4]  * m[11] * m[13] -
    m[8]  * m[5] * m[15] +
    m[8]  * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];
    
    inv[12] = -m[4]  * m[9] * m[14] +
    m[4]  * m[10] * m[13] +
    m[8]  * m[5] * m[14] -
    m[8]  * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];
    
    inv[1] = -m[1]  * m[10] * m[15] +
    m[1]  * m[11] * m[14] +
    m[9]  * m[2] * m[15] -
    m[9]  * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];
    
    inv[5] = m[0]  * m[10] * m[15] -
    m[0]  * m[11] * m[14] -
    m[8]  * m[2] * m[15] +
    m[8]  * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];
    
    inv[9] = -m[0]  * m[9] * m[15] +
    m[0]  * m[11] * m[13] +
    m[8]  * m[1] * m[15] -
    m[8]  * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];
    
    inv[13] = m[0]  * m[9] * m[14] -
    m[0]  * m[10] * m[13] -
    m[8]  * m[1] * m[14] +
    m[8]  * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];
    
    inv[2] = m[1]  * m[6] * m[15] -
    m[1]  * m[7] * m[14] -
    m[5]  * m[2] * m[15] +
    m[5]  * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];
    
    inv[6] = -m[0]  * m[6] * m[15] +
    m[0]  * m[7] * m[14] +
    m[4]  * m[2] * m[15] -
    m[4]  * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];
    
    inv[10] = m[0]  * m[5] * m[15] -
    m[0]  * m[7] * m[13] -
    m[4]  * m[1] * m[15] +
    m[4]  * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];
    
    inv[14] = -m[0]  * m[5] * m[14] +
    m[0]  * m[6] * m[13] +
    m[4]  * m[1] * m[14] -
    m[4]  * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];
    
    inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];
    
    inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];
    
    inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];
    
    inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];
    
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if (det == 0)
        return false;
    
    det = 1.0 / det;
    
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    
    return true;
}

unsigned char* Shape::ppmRead(char* filename, int* width, int* height) {
    
    FILE* fp=NULL;
    int i, w, h, d;
    unsigned char* image;
    char head[70];		// max line <= 70 in PPM (per spec).
#ifdef WIN32
    fopen_s(&fp, filename, "rb");
#else
    fp = fopen(filename, "rb");
#endif
    if (fp==NULL) {
        perror(filename);
        return NULL;
    }
    
    // Grab first two chars of the file and make sure that it has the
    // correct magic cookie for a raw PPM file.
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) {
        fprintf(stderr, "%s: Not a raw PPM file\n", filename);
        return NULL;
    }
    
    // Grab the three elements in the header (width, height, maxval).
    i = 0;
    while (i < 3) {
        fgets(head, 70, fp);
        if (head[0] == '#')		// skip comments.
            continue;
        if (i == 0){
#ifdef WIN32
            i += sscanf_s(head, "%d %d %d", &w, &h, &d);
#else
            i += sscanf(head, "%d %d %d", &w, &h, &d);
#endif
        }
        else if (i == 1){
#ifdef WIN32
            i += sscanf_s(head, "%d %d", &h, &d);
#else
            i += sscanf(head, "%d %d", &h, &d);
#endif
        }
        else if (i == 2){
#ifdef WIN32
            i += sscanf_s(head, "%d", &d);
#else
            i += sscanf(head, "%d", &d);
#endif
        }
    }
    
    // Grab all the image data in one fell swoop.
    image = (unsigned char*)malloc(sizeof(unsigned char) * w * h * 3);
    fread(image, sizeof(unsigned char), w * h * 3, fp);
    fclose(fp);
    
    *width = w;
    *height = h;
    return image;
    
}

void Shape::toggleTexture() {
    if(textured) curTexture = (curTexture + 1) % numTextures;
}

void Shape::addTexture(char* filename, int width, int height) {
    curTexture = ++numTextures;
    //get the texture data for the quad
    GLuint texture;
    glGenTextures(1, &texture);
    GLubyte *data = ppmRead(filename, &width, &height);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
    delete[] data;  //dont' need this data now that its on the GPU
    
    //set the texturing parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    textures.push_back(texture);
    curTexture = 0;
}

