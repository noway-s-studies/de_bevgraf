#include <GL/glut.h>

GLsizei winWidth = 800, winHeight = 600;

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, winWidth, 0.0f, winHeight, 0.0f, 1.0f);
	glLineWidth(3.0f);
	glPointSize(5.0f);
}

void curve()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.4f, 0.2f);
	glBegin(GL_LINE_STRIP);

	for (GLfloat t = -10; t <= 20; t += 0.01) {

		glVertex2f( t * t * t * 0.03125f + 200.0f, t * t + 200.0f);
	}
	glEnd();
        glFlush();
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (50, 100);   // Set top-left display-window position.
    glutInitWindowSize (800, 600);      // Set display-window width and height.
    glutCreateWindow ("Curve"); // Create display window.
    init ( );                            // Execute initialization procedure.
    glutDisplayFunc (curve);       // Send graphics to display window.
    glutMainLoop ( );                    // Display everything and wait.
    return 0;
}
