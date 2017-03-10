#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "Polyhedron.h"

class Skybox:public Shape{
private:
    GLuint textureID;
public:
    Skybox();
    virtual void init();
    virtual void display(Camera* camera);
};

#endif
