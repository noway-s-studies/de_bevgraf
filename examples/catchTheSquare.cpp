#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>
#include <time.h>

GLsizei winWidth = 600, winHeight = 600;

vec2 points[4] = { { 100, 300 },{ 200, 100 },{ 300, 200 },{ 400, 100 } };

GLint dragged = -1;

GLint Round(GLfloat n) { return (GLint)(n + 0.5); }

vec2 center = {400.0, 400.0};

GLint halfSize = 50;

GLint maxHalfSize = 75;
GLint minHalfSize = 25;
GLint incValue = 1;

bool hit = false;

GLint speed = 50;
GLint numberOfHit = 0;

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
	glColor3f(0.0, 0.0, 1.0);
	glRecti(center[0] - halfSize, center[1] - halfSize, center[0] + halfSize, center[1] + halfSize);
	glutSwapBuffers();
}

bool isMouseInsideSquare(GLint x, GLint y) {
	if (x > center[0] - halfSize && x < center[0] + halfSize) {
		if (y > center[1] - halfSize && y < center[1] + halfSize) {
			return true;
		}
	}
	printf("%d - %d -- %f - %f ::: %d\n", x, y, center[0], center[1], halfSize);
	return false;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		hit = isMouseInsideSquare(xMouse, winHeight - yMouse);
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		hit = false;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {

}


void checkHit(int n)
{
	if (hit) {
		halfSize = 50;
		center[0] = (float)((rand() % (winWidth - (maxHalfSize * 2))) + maxHalfSize);
		center[1] = (float)((rand() % (winHeight - (maxHalfSize * 2))) + maxHalfSize);
		hit = false;
		numberOfHit++;
		if (numberOfHit % 5 == 0) {
			if ((speed - 5 >= 5)) {
				speed -= 5;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(5, checkHit, 0);
}

void update(int n)
{

	if (halfSize > maxHalfSize || halfSize < minHalfSize) {
		incValue *= -1;
	}
			
	halfSize += incValue;

	glutPostRedisplay();

	glutTimerFunc(speed, update, 0);

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Drag & Drop");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutTimerFunc(speed, update, 0);
	glutTimerFunc(5, checkHit, 0);
	glutMainLoop();
	return 0;
}


