#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Circle.h"
#include "Camera.h"
#include "Polyhedron.h"
#include "Plane.h"
#include "Sphere.h"
#include "TexturedCube.h"
#include "Skybox.h"
#include <list>
#include <stdlib.h>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const float ROTATION_INC = TWO_PI;
const float BRIGHTNESS_INC = 1 / 360.0;
const float GROUND_SCALE = 75.0f;
const float MOVE_INC = 0.1f;
const float MOVE_MAX = 1.0f;
const float ROT_MOVE = TWO_PI / (MOVE_INC / MOVE_MAX);
const float EDGE = 2.5f;
const int ROWS = GROUND_SCALE;
const int LANES = 3;
const int DENSITY = 5;
const mat4 FOWARD_MATRIX = Translate(0, 0, FOWARD_INC);
const float EXPAND_INC = 1.01f;
const float MAX_EXPAND = 1000.0f;

int lane = 1;
int row = 0;

float move = 0;

float sunRotation = 0;
float fowardInc = 0;
int t = 0;

bool pause = false;
bool dead = false;
float curExpand = 0;
mat4 nonDead;

Camera* camera;
Camera* camera1;
Camera* camera2;

Shape* plane1;
Shape* plane2;
Shape* sphere;

int* lanes;
std::vector<Shape*> boxes;

Skybox* skybox;

void drawRay(vec3 ray);
vec4 getRayFromWindow(int mx, int my, vec4* pfront);

bool altCamera = false;

bool animating = false;

void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

std::vector<Shape*> wallList;
std::list<Shape> overlays;

mat4 convertDoubleArrToMat4(double* arr);
double* convertMat4ToDoubleArr(mat4 m);

std::vector<Light> lights;


