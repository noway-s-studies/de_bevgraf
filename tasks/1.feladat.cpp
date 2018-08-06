#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;
GLint xGo[7] = { 1,1,1,1,1,1,1 }, xPause[7];
GLint yGo[7] = { 1,1,1,1,1,1,1 }, yPause[7];
GLint status = 0;


vec2 points[7] = {
	{ 400, 300 },
	{ 400, 300 },
	{ 400, 300 },
	{ 400, 300 },
	{ 400, 300 },
	{ 400, 300 },
	{ 400, 300 }
};
vec2 affect[7] = {};

void init() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.f, winWidth, 0.f, winHeight);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_POINT_SMOOTH);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void Hermite31GMT() {
	affect[0] = { (points[1].x - points[6].x) / 2, (points[1].y - points[6].y) / 2 };
	affect[1] = { (points[2].x - points[0].x) / 2, (points[2].y - points[0].y) / 2 };
	affect[2] = { (points[3].x - points[1].x) / 2, (points[3].y - points[1].y) / 2 };
	affect[3] = { (points[4].x - points[2].x) / 2, (points[4].y - points[2].y) / 2 };
	affect[4] = { (points[5].x - points[3].x) / 2, (points[5].y - points[3].y) / 2 };
	affect[5] = { (points[6].x - points[4].x) / 2, (points[6].y - points[4].y) / 2 };
	affect[6] = { (points[0].x - points[5].x) / 2, (points[0].y - points[5].y) / 2 };
	GLfloat t1 = -1;
	GLfloat t2 = 1;
	vec4 o1 = { t1*t1*t1, t1*t1, t1, 1 };
	vec4 o2 = { t2*t2*t2, t2*t2, t2, 1 };
	vec4 o3 = { 3 * t1*t1,  2 * t1,  1, 0 };
	vec4 o4 = { 3 * t2*t2,  2 * t2,  1, 0 };
	mat4  N = { o1, o2, o3, o4, true };
	mat4  M = inverse(N);

	glLineWidth(10.0);

	mat24 G = { points[0], points[1], affect[0], affect[1] };
	mat24 C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 0.0);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[1], points[2], affect[1], affect[2] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.6, 0.0, 0.6);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[2], points[3], affect[2], affect[3] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.4, 0.6, 0.0);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[3], points[4], affect[3], affect[4] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.7, 0.0, 0.0);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[4], points[5], affect[4], affect[5] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.8, 0.3, 0.4);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[5], points[6], affect[5], affect[6] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 1.0);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	G = { points[6], points[0], affect[6], affect[0] };
	C = G*M;
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 1.0);
		for (GLfloat t = t1; t <= t2; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);
		}
	glEnd();

	glPointSize(5.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
		glVertex2f(points[0].x, points[0].y);
		glVertex2f(points[1].x, points[1].y);
		glVertex2f(points[2].x, points[2].y);
		glVertex2f(points[3].x, points[3].y);
		glVertex2f(points[4].x, points[4].y);
		glVertex2f(points[5].x, points[5].y);
		glVertex2f(points[6].x, points[6].y);
	glEnd();
}

void mov() {
	GLint p = 0;
	GLint patt[16] = { 1, 2, 3, 2, 3, 1, 4, 2, 3, 4, 3, 2, 1, 2, 3, 4 };
	for (GLint i = 0; i < 7; i++) {
		if (points[i].x > winWidth - 1) {
			p == 16 ? p = 0 : p++;
			xGo[i] = -patt[p];
		}
		if (points[i].y > winHeight - 1) {
			p == 16 ? p = 0 : p++;
			yGo[i] = -patt[p];
		}
		if (points[i].x < 1) {
			p == 16 ? p = 0 : p++;
			xGo[i] = patt[p];
		}
		if (points[i].y < 1) {
			p == 16 ? p = 0 : p++;
			yGo[i] = patt[p];
		}
		points[i].x = points[i].x + xGo[i];
		points[i].y = points[i].y + yGo[i];
	}
}

void update(int v) {
	mov();
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	Hermite31GMT();
	glutSwapBuffers();
}

static void key(unsigned char gomb, int x, int y) {
	switch (gomb) {
	case '0':
		if (xGo[0] == 0) {
			for (int i = 0; i < 7; i++) {
				xGo[i] = xPause[i];
				yGo[i] = xPause[i];
			}
			break;
		}
		else {
			for (int i = 0; i < 7; i++) {
				xPause[i] = xGo[i];
				xPause[i] = yGo[i];
				xGo[i] = 0;
				yGo[i] = 0;
			}
			break;
		}
	}
	glutPostRedisplay();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("1. feladat");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}