//
// A GLSL "Hellow World"
// Display a blue square
//

#include "main.h"

//----------------------------------------------------------------------------

// OpenGL initialization
void
init(){
    camera = Camera();
    Polyhedron polyhedron = Polyhedron();
    shapeList.push_back(polyhedron);
    polyhedron.init();
    
    glClearColor( 0.0, 0.0, 0.4, 0.0 );
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
    glutTimerFunc(10, timerCallback, value);
    glutPostRedisplay();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    std::list<Shape>::iterator it;
    for (it = shapeList.begin(); it != shapeList.end(); ++it) {
        it->display(camera);
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
            animating = !animating;
            break;
        case 'p': case 'P':
            camera.togglePerspective();
            break;
        case 'X':
            camera.pitchUp();
            break;
        case 'x':
            camera.pitchDown();
            break;
        case 'Z':
            camera.rollClockwise();
            break;
        case 'z':
            camera.rollCounterClockwise();
            break;
        case 'C':
            camera.yawCounterClockwise();
            break;
        case 'c':
            camera.yawClockwise();
            break;
        
    }
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            camera.moveFoward();
            break;
        case GLUT_KEY_DOWN:
            camera.moveBackward();
            break;
        case GLUT_KEY_LEFT:
            camera.moveLeft();
            break;
        case GLUT_KEY_RIGHT:
            camera.moveRight();
            break;
    
    }
}

void mouse(int button, int state, int x, int y) {
}

void reshape(int width, int height) {
    camera.calculateMatrix();
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

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
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
    for(Shape shape : shapeList) {
        shape.deleteBuffer();
    }
}
