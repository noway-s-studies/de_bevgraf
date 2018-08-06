#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <vector>

GLsizei winWidth = 400, winHeight = 300;
std::vector<vec2> points = { { 100.0, 100.0 }, { 200.0, 150.0 }, { 300.0, 175.0 }, { 350.0, 50.0 } };

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(10);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
    
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glutSwapBuffers();
}

void mouse(int button, int action, int x, int y)
{
    vec2 p = vec2(x, winHeight - y - 1);

    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        points.push_back(p);
    }

    else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
    {
	    for (int i = 0; i < points.size(); i++)
        {
            if (dist(points[i], p) < 10)
            {
                points.erase(points.begin() + i);
            }
        }
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Insert & Remove Points");
	init();
	glutDisplayFunc(display);
    glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
