//
// A GLSL "Hellow World"
// Display a blue square
//

#include "main.h"

//----------------------------------------------------------------------------

// OpenGL initialization
void
init(){
    camera1 = new Camera(vec3(0,1,0),vec3(0,1,0));
    camera1->allowMove = false;
    camera2 = new Camera(vec3(0,10,0),vec3(0,0,1));
    camera2->allowMove = false;
    camera = camera1;
    
    skybox = new Skybox();
    
    sphere = new Sphere(vec3(0,0.5f,-4),0.5f, 4);
    sphere->setRandomColors();
    sphere->setMaterial(vec4(.3,.3,.3,1), vec4(.2,.2,.2,1), vec4(0,0,0,1), 50);
    sphere->init();
    
    TexturedCube* rw1 = new TexturedCube(vec3(EDGE,1,0), GROUND_SCALE, 1);
    rw1->addTexture("crate_texture.ppm", 512, 512);
    rw1->addTexture("gummi.ppm", 512, 512);
    rw1->setColor(vec3(0,0,1));
    rw1->setMaterial(vec4(1,1,1,1), vec4(1,1,1,1), vec4(1,1,1,1), 50);
    rw1->init();
    wallList.push_back(rw1);
    
    TexturedCube* lw1 = new TexturedCube(vec3(-EDGE,1,0), GROUND_SCALE, 1);
    lw1->addTexture("crate_texture.ppm", 512, 512);
    lw1->addTexture("gummi.ppm", 512, 512);
    lw1->setColor(vec3(0,0,1));
    lw1->setMaterial(vec4(1,1,1,1), vec4(1,1,1,1), vec4(1,1,1,1), 50);
    lw1->init();
    wallList.push_back(lw1);
    
    TexturedCube* rw2 = new TexturedCube(vec3(EDGE,1,-GROUND_SCALE), GROUND_SCALE, 1);
    rw2->addTexture("crate_texture.ppm", 512, 512);
    rw2->addTexture("gummi.ppm", 512, 512);
    rw2->setColor(vec3(0,0,1));
    rw2->setMaterial(vec4(1,1,1,1), vec4(1,1,1,1), vec4(1,1,1,1), 50);
    rw2->init();
    wallList.push_back(rw2);
    
    TexturedCube* lw2 = new TexturedCube(vec3(-EDGE,1,-GROUND_SCALE), GROUND_SCALE, 1);
    lw2->addTexture("crate_texture.ppm", 512, 512);
    lw2->addTexture("gummi.ppm", 512, 512);
    lw2->setColor(vec3(0,0,1));
    lw2->setMaterial(vec4(1,1,1,1), vec4(1,1,1,1), vec4(1,1,1,1), 50);
    lw2->init();
    wallList.push_back(lw2);
    
    plane1 = new Plane(vec3(0,0,0), GROUND_SCALE, GROUND_SCALE);
    plane1->addTexture("ground.ppm", 512, 512);
    plane1->setColor(vec3(0,0,0)); //green
    plane1->setMaterial(vec4(.4,.4,.4,1), vec4(.4,.4,.4,1), vec4(.4,.4,.4,1), 1);
    plane1->init();
    overlays.push_back(*plane1);
    
    plane2 = new Plane(vec3(0,0,-GROUND_SCALE), GROUND_SCALE, GROUND_SCALE);
    plane2->addTexture("ground.ppm", 512, 512);
    plane2->setColor(vec3(0,0,0)); //green
    plane2->setMaterial(vec4(.4,.4,.4,1), vec4(.4,.4,.4,1), vec4(.4,.4,.4,1), 1);
    plane2->init();
    overlays.push_back(*plane2);
    
    Light light1;
    light1.position = vec4(100,100,100,1);
    light1.specular = vec4(1,1,1,1);
    light1.diffuse = vec4(1,1,1,1);
    light1.ambient = vec4(.2,.2,.2,1);
    lights.push_back(light1);
    
    Light light2;
    light2.on = false;
    light2.specular = vec4(1,1,1,1);
    light2.diffuse = vec4(.5,.5,.5,1);
    light2.ambient = vec4(0,0,0,1);
    lights.push_back(light2);
    
    glClearColor( 0, 0, 1, 1.5 );
    glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

void timerCallback(int value)
{
    for(int i=0;i<wallList.size();i++) {
        wallList[i]->modelMatrix = wallList[i]->modelMatrix * Translate(0, 0, FOWARD_INC);
    }
    plane1->modelMatrix = plane1->modelMatrix * Translate(0, 0, FOWARD_INC);
    plane2->modelMatrix = plane2->modelMatrix * Translate(0, 0, FOWARD_INC);
    fowardInc += FOWARD_INC;
    if(fowardInc > GROUND_SCALE) {
        fowardInc = 0;
        plane1->modelMatrix = Translate(0, 0, -GROUND_SCALE);
        wallList[0]->modelMatrix = Translate(EDGE, 1, -GROUND_SCALE);
        wallList[1]->modelMatrix = Translate(-EDGE, 1, -GROUND_SCALE);
        Shape* t = plane2;
        plane2 = plane1;
        plane1 = t;
        
        Shape* w1 = wallList[0];
        Shape* w2 = wallList[1];
        wallList[0] = wallList[2];
        wallList[1] = wallList[3];
        wallList[2] = w1;
        wallList[3] = w2;
    }
    sphere->modelMatrix = sphere->modelMatrix * RotateX(-ROTATION_INC);
    if(move >= MOVE_MAX || move <= -MOVE_MAX) {
        move = 0;
    }
    if(move > 0) {
        move += MOVE_INC;
        sphere->modelMatrix = sphere->modelMatrix * Translate(MOVE_INC, 0, 0);
    } else if(move < 0) {
        move -= MOVE_INC;
        sphere->modelMatrix = sphere->modelMatrix * Translate(-MOVE_INC, 0, 0);
    }
    glutTimerFunc(10, timerCallback, value);
    glutPostRedisplay();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    std::list<Shape>::iterator it;
    
    skybox->display(camera);
    
    plane1->display(camera, lights);
    plane2->display(camera, lights);
    for(int i=0;i<wallList.size();i++) {
        wallList[i]->display(camera,lights);
    }
    sphere->display(camera, lights);
    
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
            lights[1].on = !lights[1].on;
            break;
        case 'p': case 'P':
            camera->togglePerspective();
            break;
        case 'X':
            camera->pitchUp();
            break;
        case 'x':
            camera->pitchDown();
            break;
        case 'Z':
            camera->rollClockwise();
            break;
        case 'z':
            camera->rollCounterClockwise();
            break;
        case 'C':
            camera->yawCounterClockwise();
            break;
        case 'c':
            camera->yawClockwise();
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            camera->moveFoward();
            break;
        case GLUT_KEY_DOWN:
            camera->moveBackward();
            break;
        case GLUT_KEY_LEFT:
            camera->moveLeft();
            if(move == 0 && lane>1) {
                move = -MOVE_INC;
                lane--;
            }
            break;
        case GLUT_KEY_RIGHT:
            camera->moveRight();
            if(move == 0 && lane<3) {
                move = MOVE_INC;
                lane++;
            }
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
    for(int i=0;i<wallList.size();i++) {
        wallList[i]->deleteBuffer();
    }
}
