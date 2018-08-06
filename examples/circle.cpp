#include <GL/glut.h>					// (or others, depending on the system in use)
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;


void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}


void circle(vec2 O, GLdouble r) {

	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.4f, 0.2f);

	circle(vec2(200, 300), 100);

	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowPosition(50, 100);   // Set top-left display-window position.
	glutInitWindowSize(400, 300);      // Set display-window width and height.
	glutCreateWindow("An Example OpenGL Program"); // Create display window.

	init();                            // Execute initialization procedure.
	glutDisplayFunc(draw);       // Send graphics to display window.
	glutMainLoop();                    // Display everything and wait.
	return 0;
}