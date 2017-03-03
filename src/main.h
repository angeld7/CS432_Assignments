#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Circle.h"
#include "Camera.h"
#include "Polyhedron.h"
#include "Plane.h"
#include "Sphere.h"
#include <list>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const float ROTATION_INC = TWO_PI / 20;
const float BRIGHTNESS_INC = 1 / 360.0;

float sunRotation = 0;

int t = 0;

Camera* camera;
Camera camera1;
Camera camera2;

void drawRay(vec3 ray);
vec4 getRayFromWindow(int mx, int my, vec4* pfront);

bool altCamera = false;

bool animating = false;

void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

std::list<Shape> shapeList;
std::list<Shape> overlays;

mat4 convertDoubleArrToMat4(double* arr);
double* convertMat4ToDoubleArr(mat4 m);
bool checkCollision();

std::vector<Light> lights;
