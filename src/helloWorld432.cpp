//
// A GLSL "Hellow World"
// Display a blue square
//

#include "main.h"

//----------------------------------------------------------------------------

// OpenGL initialization
void
init(){
    camera1 = Camera(vec3(0,2,0),vec3(0,1,0));
    camera1.allowPick = true;
    camera2 = Camera(vec3(0,10,0),vec3(0,0,1));
    camera2.allowMove = false;
    camera = &camera1;
    
    Sphere sphere = Sphere(vec3(0,1,-4),1.0f, 4);
    sphere.setColor(vec3(0,0,0));
    sphere.setMaterial(vec4(1,0,0,1), vec4(1,0,0,1), vec4(1,0,0,1), 50);
    sphere.init();
    shapeList.push_back(sphere);
    
    Plane plane = Plane(vec3(0,0,0), 100, 100);
    plane.setColor(vec3(0,1,0)); //green
    plane.setMaterial(vec4(0,1,0,1), vec4(0,1,0,1), vec4(0,1,0,1), 50);
    plane.init();
    overlays.push_back(plane);
    
    Light light1;
    light1.position = vec4(0,10,-1,1);
    light1.specular = vec4(1,1,1,1);
    light1.diffuse = vec4(1,1,1,1);
    light1.ambient = vec4(.2,.2,.2,1);
    lights.push_back(light1);
    
    glClearColor( 1.0, 1.0, 1, 1.5 );
    glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

void timerCallback(int value)
{
    if(animating) {
        std::list<Shape>::iterator it;
        for (it = shapeList.begin(); it != shapeList.end(); ++it) {
            it->rotate(ROTATION_INC);
        }
    }
    Light light = lights[0];
    light.position = RotateZ(ROTATION_INC) * light.position;
    glutTimerFunc(10, timerCallback, value);
    glutPostRedisplay();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    std::list<Shape>::iterator it;
    
    for (it = overlays.begin(); it != overlays.end(); ++it) {
        it->display(camera,lights);
    }
    for (it = shapeList.begin(); it != shapeList.end(); ++it) {
        it->display(camera,lights);
    }
    
    
	glFlush();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
        case 'q': case 'Q': {
            exit( EXIT_SUCCESS );
            break;
        }
        case ' ':
            altCamera = !altCamera;
            if(altCamera) {
                camera = &camera2;
            } else {
                camera = &camera1;
            }
            break;
        case 'p': case 'P':
            camera->togglePerspective();
            break;
        case 'X':
            camera->pitchUp();
            if (checkCollision()) camera->pitchDown();
            break;
        case 'x':
            camera->pitchDown();
            if (checkCollision()) camera->pitchUp();
            break;
        case 'Z':
            camera->rollClockwise();
            if (checkCollision()) camera->rollCounterClockwise();
            break;
        case 'z':
            camera->rollCounterClockwise();
            if (checkCollision()) camera->rollClockwise();
            break;
        case 'C':
            camera->yawCounterClockwise();
            if (checkCollision()) camera->yawClockwise();
            break;
        case 'c':
            camera->yawClockwise();
            if (checkCollision()) camera->yawCounterClockwise();
            break;
        
    }
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            camera->moveFoward();
            if (checkCollision()) camera->moveBackward();
            break;
        case GLUT_KEY_DOWN:
            camera->moveBackward();
            if (checkCollision()) camera->moveFoward();
            break;
        case GLUT_KEY_LEFT:
            camera->moveLeft();
            if (checkCollision()) camera->moveRight();
            break;
        case GLUT_KEY_RIGHT:
            camera->moveRight();
            if (checkCollision()) camera->moveLeft();
            break;
    
    }
}

vec4 getRayFromWindow(int mx, int my, vec4* pfront) {
    float width = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);
    
    float x = (((float) mx / width) * 2.0f) - 1;
    float y = 1 - (((float) my / height) * 2.0f);
    
    pfront = new vec4(x,y,-1.0f,1.0f);
    
    double* invViewArr = new double[16];
    Shape::gluInvertMatrix(convertMat4ToDoubleArr(camera->viewMatrix), invViewArr);
    mat4 inverseView = convertDoubleArrToMat4(invViewArr);
    
    vec4 pworld = inverseView * *pfront;
    vec4 ray = pworld - camera->eye;
    return ray;
}

bool checkCollision() {
    std::list<Shape>::iterator it;
    for (it = shapeList.begin(); it != shapeList.end(); ++it) {
        vec3* p = new vec3[3];
        float t = it->checkCollisionPoly(camera->getEye(), camera->getEye(), p);
        if (t > 0 && t < INT_MAX) return true;
    }
    return false;
}

void drawRay(vec3 ray) {
    vec3 eye = camera->getEye();
    vec3 r = vec3(ray.x,ray.y,ray.z);
    Shape l = Shape();
    l.setPoints(new vec3[2]{eye,eye+r*100},2);
    l.setColor(vec3(1,0,0));
    l.init();
    shapeList.push_back(l);
}

void mouse(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && camera->allowPick) {
        vec4* front;
        vec4 ray = getRayFromWindow(x, y, front);
        //drawRay(vec3(ray.x,ray.y,ray.z));
        float t = INT_MAX;
        vec3* points = new vec3[3];
        std::list<Shape>::iterator it;
        for (it = shapeList.begin(); it != shapeList.end(); ++it) {
            vec3* p = new vec3[3];
            float t2 = it->checkCollisionPoly(ray, camera->getEye(), p);
            if(t2 < t && t2 > 0) {
                points = p;
                t = t2;
            }
        }
        if(t > 0) {
            Shape shape = Shape();
            shape.setPoints(points, 3);
            shape.setColor(vec3(0,0,0));
            shape.init();
            overlays.push_back(shape);
        }
    }
}

double* convertMat4ToDoubleArr(mat4 m){
    double* ret = new double[16];
    for(int i = 0; i < 16; i++){
        int r = i / 4;
        int c = i % 4;
        ret[i] = (double)m[r][c];
    }
    return ret;
}

mat4 convertDoubleArrToMat4(double* arr) {
    return mat4(vec4(arr[0],  arr[1],  arr[2],  arr[3]),
                vec4(arr[4],  arr[5],  arr[6],  arr[7]),
                vec4(arr[8],  arr[9],  arr[10], arr[11]),
                vec4(arr[12], arr[13], arr[14], arr[15]));
}

void reshape(int width, int height) {
    glViewport(0,0,width,height);
    camera->calculateMatrix();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#endif
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

    glutCreateWindow( "CS 432 Hello World" );
	m_glewInitAndVersion();


    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard );
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
	glutWMCloseFunc(close);
    glutReshapeFunc(reshape);
    
    glutTimerFunc(10, timerCallback, 0);
    
    glutMainLoop();
    return 0;
}

void m_glewInitAndVersion(void)
{
   fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
   fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef __APPLE__
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
   fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }
   fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
}

void close(){
    for(Shape shape : shapeList) {
        shape.deleteBuffer();
    }
}
