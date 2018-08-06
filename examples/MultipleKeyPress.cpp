#include <GL/glut.h>

GLint keyStates[256];

GLfloat x00 = 100, y00 = 40, x11 = 200, y11 = 200;

GLfloat delta = 0.01;

void init (void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 400.0, 0.0, 300.0);
}

void keyPressed (unsigned char key, int x, int y) {
    keyStates[key] = 1;
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = 0;
}

void keyOperations ( ) {
    if (keyStates['a']) {	x00-=delta; }
    if (keyStates['d']) {   x00+=delta; }

    if (keyStates['s']) {	y11-=delta; }
    if (keyStates['w']) {   y11+=delta; }


    glutPostRedisplay( );
}

void lineSegment (void)
{

    keyOperations();
    glClear (GL_COLOR_BUFFER_BIT);

    glColor3f (0.0, 0.4, 0.2);
    glBegin (GL_LINES);
        glVertex2f (x00, y00);
        glVertex2f (x11, y11);
    glEnd ( );

    glutSwapBuffers( );
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition (50, 100);

    glutInitWindowSize (400, 300);

    glutCreateWindow ("multiple key press");

    init ( );

    glutDisplayFunc (lineSegment);

    glutKeyboardFunc(keyPressed);

    glutKeyboardUpFunc(keyUp);

    glutMainLoop ( );

    return 0;
}


