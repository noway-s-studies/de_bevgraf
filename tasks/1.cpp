#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

struct system {
	GLfloat positionX = 100.0f;
	GLfloat positionY = 100.0f;
	GLfloat weight = 1280.0f;
	GLfloat height = 720.0f;
	GLfloat bgcolor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
} window;

vec2 points[7] = {
	{ 300, 270 },
	{ 280, 450 },
	{ 420, 510 },
	{ 600, 600 },
	{ 710, 600 },
	{ 950, 450 },
	{ 1050, 340 }
};
vec2 affect[4] = {
	{ 200, 340 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
};

void init() {
	glClearColor(window.bgcolor[0], window.bgcolor[1], window.bgcolor[2], window.bgcolor[3]);
	gluOrtho2D(0.f, window.weight, 0.f, window.height);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_POINT_SMOOTH);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void Hermite31GMT() {
	GLfloat t1 = -1.5;
	GLfloat t2 = 0;
	GLfloat t3 = 1.5;
	vec4 o1 = { t1*t1*t1, t1*t1, t1, 1 };
	vec4 o2 = { t2*t2*t2, t2*t2, t2, 1 };
	vec4 o3 = { t3*t3*t3, t3*t3, t3, 1 };
	vec4 o4 = { 3 * t1*t1,  2 * t1,  1, 0 };
	mat4  N = { o1, o2, o3, o4, true };
	mat4  M = inverse(N);
	mat24 G = { points[0], points[1], points[2], affect[0] - points[0] };
	mat24 C = G*M;
	// 1. Vektor
	glBegin(GL_LINE_STRIP);
		glLineWidth(2.0);
		glColor3f(0.6, 0.6, 0.6);
		glVertex2f(points[0].x, points[0].y);
		glVertex2f(affect[0].x, affect[0].y);
	glEnd();
	// 1. Ív
	glLineWidth(5.0);
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 0.0);
		for (GLfloat t = t1; t <= t3; t += 0.01) {
			vec4 TI1 = { t*t*t, t*t, t, 1 };
			vec2 P = C*TI1;
			glVertex2f(P.x, P.y);

			vec4 TI2 = { 3 * t*t, 2 * t, 1, 0 };
			affect[1] = C*TI2;
		}
	glEnd();

	G = { points[2], points[3], points[4], affect[1] };
	C = G*M;
	// 2. Vektor
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
		glColor3f(0.6, 0.6, 0.6);
		glVertex2f(points[2].x, points[2].y);
		glVertex2f(points[2].x + affect[1].x, points[2].y + affect[1].y);
	glEnd();
	// 2. Ív
	glLineWidth(5.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.6, 0.6);
	for (GLfloat t = t1; t <= t3; t += 0.01) {
		vec4 TI = { t*t*t, t*t, t, 1 };
		vec2 P = C*TI;
		glVertex2f(P.x, P.y);

		vec4 TI2 = { 3 * t*t, 2 * t, 1, 0 };
		affect[2] = C*TI2;
	}
	glEnd();

	G = { points[4], points[5], points[6], affect[2] };
	C = G*M;
	// 3. Vektor
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2f(points[4].x, points[4].y);
	glVertex2f(points[4].x + affect[2].x, points[4].y + affect[2].y);
	glEnd();
	// 3. Ív
	glLineWidth(5.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.6, 0.6, 0.0);
	for (GLfloat t = t1; t <= t3; t += 0.01) {
		vec4 TI = { t*t*t, t*t, t, 1 };
		vec2 P = C*TI;
		glVertex2f(P.x, P.y);

		vec4 TI2 = { 3 * t*t, 2 * t, 1, 0 };
		affect[3] = C*TI2;
	}
	glEnd();

	// 4. Vektor
	glLineWidth(1.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2f(points[6].x, points[6].y);
	glVertex2f(points[6].x + affect[3].x, points[6].y + affect[3].y);
	glEnd();

	// Pontok
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(affect[0].x, affect[0].y);
	glVertex2f(points[0].x, points[0].y);
	glVertex2f(points[1].x, points[1].y);
	glVertex2f(points[2].x, points[2].y);
	glVertex2f(points[3].x, points[3].y);
	glVertex2f(points[4].x, points[4].y);
	glVertex2f(points[5].x, points[5].y);
	glVertex2f(points[6].x, points[6].y);
	glVertex2f(points[7].x, points[7].y);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	Hermite31GMT();
	glutSwapBuffers();
}

GLint getActivePoint(vec2 p[], GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };
	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

GLint draggedP = -1;
GLint draggedV = -1;

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	if (draggedP >= 0) {
		points[draggedP].x = xMouse;
		points[draggedP].y = window.height - yMouse;
		glutPostRedisplay();
	}
	if (draggedV >= 0) {
		affect[draggedV].x = xMouse;
		affect[draggedV].y = window.height - yMouse;
		glutPostRedisplay();
	}
}

void mouse(int button, int action, int x, int y) {
	vec2 p = vec2(x, window.height - y);
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		for (int i = 0; i <= 7; i++) {
			if (dist(points[i], p) < 20.0f) {
				for (int j = i + 1; j <= 7; j++) {
					points[getActivePoint(points, 4, 20.0f, x, window.height - y)] = p;
					draggedP = i;
				}
				break;
			}
			else {
				draggedP = -1;
			}
		}
		if (dist(affect[0], p) < 20.0f) {
			affect[getActivePoint(affect, 4, 20.0f, x, window.height - y)] = p;
			draggedV = 0;
		}
		else {
			draggedV = -1;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize((int)window.weight, (int)window.height);
	glutInitWindowPosition((int)window.positionX, (int)window.positionY);
	glutCreateWindow("Turánszki Róbert (U5KA1V) - Beadandó 1.");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}
