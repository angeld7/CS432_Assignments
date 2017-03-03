#ifndef _LIGHT_H
#define _LIGHT_H


class Light {
public:
    vec4 position, ambient, diffuse, specular, dir;
    bool on = true;
};


#endif
