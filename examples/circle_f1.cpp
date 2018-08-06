#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <bevgrafmath2017.h>

int novX1 = 1;
int novY1 = 1;
int radius = 80;
vec2 circle = { 100.0, 100.0 };


void init()
{
	glClearColor(0.25, 0.25, 0.25, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glShadeModel(GL_FLAT);
	glPointSize(5.0);
}

void circle_empty()
{

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(circle.x + radius * cos(t), circle.y + radius * sin(t));
	glEnd();
}

void grid()
{
	int i, j, insidePoint;
	double r, g, b;
	r = g = b = 1.0;

	glColor3f(r, g, b);

	for (i = 10; i <= 790; i += 10)
	{
		for (j = 10; j <= 590; j += 10)
		{
			insidePoint = (pow(i - circle.x, 2) + pow(j - circle.y, 2)) - pow(radius, 2);

			if (insidePoint < 0)
				r = g = b = 0.5;
			else
				r = g = b = 1.0;

			glColor3f(r, g, b);
			glBegin(GL_POINTS);
			glVertex2i(i, j);
			glEnd();
		}
	}
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	grid();
	circle_empty();
	glutSwapBuffers();
}

void update(int n)
{
	circle.x += novX1;
	circle.y -= novY1;

	if (circle.y + radius > 600 || circle.y - radius < 0)
		novY1 *= -1;
	if (circle.x + radius > 800 || circle.x - radius < 0)
		novX1 *= -1;

	glutPostRedisplay();
	glutTimerFunc(5, update, 0);

}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("vonal-pont");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}
