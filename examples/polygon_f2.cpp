#include <GL/glut.h>
#include <math.h>
#include <bevgrafmath2017.h>
#include <vector>

std::vector<vec2> points = {};

int n = 18;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 600.0, 0.0, 600.0);
	glPointSize(10.0);
	glLineWidth(1.0);
}

void setupPolygon() 
{
	vec2 O = { 300.0, 300.0 };
	int r = 150;


	for (GLdouble t = 0; t <= 2 * pi(); t += 2 * pi() / n)
		points.push_back(vec2(O.x + r * cos(t), O.y + r * sin(t)));
		
}

void display()
{


	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);                                


	glBegin(GL_LINE_LOOP);				//polygon
	glColor3f(1.0, 0.0, 0.0);
	for (vec2 point : points)
		glVertex2d(point.x, point.y);
	glEnd();

	glBegin(GL_LINES);					

	for (int j = 0; j < n; j++)
		for (int k = j + 2; k < n; k++)
		{
			if (j == 0 && k == n - 1) break;
			
			glColor3f(0.0, 0.0, 1.0);
			glVertex2d(points[j].x, points[j].y);
			glColor3f(1.0, 1.0, 1.0);
			glVertex2d(points[k].x, points[k].y);

		}

	glEnd();

	glFlush();

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Szabalyos sokszog");
	init();
	setupPolygon();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;

}

