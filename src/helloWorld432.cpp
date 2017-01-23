//
// A GLSL "Hellow World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Circle.h"
void m_glewInitAndVersion(void);  //pre-implementation declaration (could do in header file)
void close(void);

Shape* square;
Shape* triangle;
Circle* circle;

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    vec2 squarePoints[4] = {
        vec2( 0.25, 0.25),
        vec2( 0.75, 0.25),
        vec2( 0.75, 0.75),
        vec2( 0.25, 0.75)
    };
    vec2 trianglePoints[3] = {
        vec2( -0.2, -0.2),
        vec2( 0.2, 0.2),
        vec2( 0.2, -0.2)
    };
    std::string pcVShader = "pcvshader.glsl";
    std::string pcFShader = "pcfshader.glsl";
    square = new Shape(pcVShader, pcFShader);
    square->setPoints(squarePoints, 4);
    square->setColor(vec4( 1.0, 0.0, 0.0, 1.0 ));
    square->init();
    
    triangle = new Shape(pcVShader, pcFShader);
    triangle->setPoints(trianglePoints, 3);
    triangle->setColor(vec4( 0.0, 0.0, 1.0, 1.0 ));
    triangle->init();
    
    circle = new Circle(pcVShader, pcFShader, -0.55, -0.55, 0.3);
    circle->setRandomColors();
    circle->init();
}

//----------------------------------------------------------------------------

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    square->display();
    triangle->display();
    circle->display();
    
	glFlush();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
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
    glutInitWindowSize( 512, 512 );

    glutCreateWindow( "CS 432 Hello World" );
	m_glewInitAndVersion();


    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutWMCloseFunc(close);

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
    square->deleteBuffer();
    triangle->deleteBuffer();
}
