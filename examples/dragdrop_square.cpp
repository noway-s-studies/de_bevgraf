#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;

vec2 points[1] = { {400, 300} };
GLint halfSize = 50;

GLint dragged = -1;

GLint Round(GLfloat n) { return (GLint)(n + 0.5); }

double r, g, b = 0.0;

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glPointSize(10.0);
	glLineWidth(5.0);
	glLineStipple(1, 0xFF00);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(r, g, b);
	glRecti(points[0].x - halfSize, points[0].y - halfSize, points[0].x + halfSize, points[0].y + halfSize);
	

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2i(points[0].x, points[0].y);
	glEnd();

	glutSwapBuffers();

}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

GLint getActivePoint2(vec2 *p, GLint size, GLint sens, GLint x, GLint y) {
	GLint i;
	for (i = 0; i < size; i++)
		if (fabs((*(p + i)).x - x) < sens && fabs((*(p + i)).y - y) < sens)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(points, 1, 4, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	GLint i;
	if (dragged >= 0) {
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		if (points[dragged].x + halfSize >= 800)
			points[dragged].x = 800;
		if (points[dragged].x - halfSize <= 0)
			points[dragged].x = 0;
		if (points[dragged].y + halfSize >= 600)
			points[dragged].y = 600;
		if (points[dragged].y - halfSize <= 0)
			points[dragged].y = 0;
		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'r':
		r = 1.0;
		g = 0.0;
		b = 0.0;
		break;
	case 'g':
		r = 0.0;
		g = 1.0;
		b = 0.0;
		break;
	case 'b':
		r = 0.0;
		g = 0.0;
		b = 1.0;
		break;
	case 'w':
		halfSize += 1;
		break;
	case 's':
		halfSize -= 1;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Drag & Drop");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}


