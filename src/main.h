#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Circle.h"
#include "Camera.h"
#include "Polyhedron.h"
#include <list>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const float ROTATION_INC = TWO_PI / 20;
const float BRIGHTNESS_INC = 1 / 360.0;

Camera camera;

bool animating = false;

void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

std::list<Shape> shapeList;
