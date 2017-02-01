//
// A GLSL "Hellow World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Circle.h"
#include <list>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const float ROTATION_INC = TWO_PI / 360;
const float BRIGHTNESS_INC = 1 / 360.0;

bool animating = false;

void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

std::list<Shape> shapeList;

//----------------------------------------------------------------------------

// OpenGL initialization
void
init(){
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void timerCallback(int value)
{
    
    if(animating) {
        std::list<Shape>::iterator it;
        for (it = shapeList.begin(); it != shapeList.end(); ++it) {
            it->rotate(ROTATION_INC);
            it->increaseBrightness(BRIGHTNESS_INC);
        }
    }
    glutTimerFunc(1000 / 60, timerCallback, value);
    glutPostRedisplay();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    std::list<Shape>::iterator it;
    for (it = shapeList.begin(); it != shapeList.end(); ++it) {
        it->display();
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
    }
}

void mouse(int button, int state, int x, int y) {
    std::string pcVShader = "pcvshader.glsl";
    std::string pcFShader = "pcfshader.glsl";
    
    float xWorld = (((float) x / glutGet(GLUT_WINDOW_WIDTH)) * 2.0) - 1;
    float yWorld = 1 - (((float) y / glutGet(GLUT_WINDOW_HEIGHT)) * 2.0);
    vec2 center = vec2(xWorld, yWorld);
    switch ( button) {
        case GLUT_LEFT_BUTTON:
        {
            if(state == GLUT_UP) {
                vec2 squarePoints[4] = {
                    vec2( xWorld - 0.2, yWorld + 0.2),
                    vec2( xWorld + 0.2, yWorld + 0.2),
                    vec2( xWorld + 0.2, yWorld - 0.2),
                    vec2( xWorld - 0.2, yWorld - 0.2)
                };
                Shape square = Shape(pcVShader, pcFShader);
                square.setPoints(squarePoints, 4);
                if(glutGetModifiers() == GLUT_ACTIVE_SHIFT || glutGetModifiers() == GLUT_ACTIVE_CTRL) {
                    square.setRandomColors();
                } else {
                    square.setColor(vec3(1.0, 0.0, 0.0));
                }
                square.init();
                square.center = center;
                shapeList.push_back(square);
            }
            break;
        }
        case GLUT_RIGHT_BUTTON:
        {
            if(state == GLUT_UP) {
                vec2 trianglePoints[3] = {
                    vec2( xWorld, yWorld + 0.2),
                    vec2( xWorld - 0.2, yWorld - 0.2),
                    vec2( xWorld + 0.2, yWorld - 0.2)
                };
                Shape triangle = Shape(pcVShader, pcFShader);
                triangle.setPoints(trianglePoints, 3);
                if(glutGetModifiers() == GLUT_ACTIVE_SHIFT || glutGetModifiers() == GLUT_ACTIVE_CTRL) {
                    triangle.setRandomColors();
                } else {
                    triangle.setColor(vec3(0.0, 0.0, 1.0));
                }
                triangle.init();
                triangle.center = center;
                shapeList.push_back(triangle);
            }
            break;
        }
        default:
            break;
    }
    display();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_SINGLE);
#else
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE);
#endif
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

    glutCreateWindow( "CS 432 Hello World" );
	m_glewInitAndVersion();


    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
	glutWMCloseFunc(close);
    
    glutTimerFunc(1000 / 60, timerCallback, 0);
    
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
