#include "Shape.h"

class Polyhedron:public Shape{
public:
    virtual void init();
    virtual void rotate(float theta);
};
