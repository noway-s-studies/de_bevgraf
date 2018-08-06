#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

struct system {
	GLfloat positionX = 100.0f;
	GLfloat positionY = 100.0f;
	GLfloat weight = 800.0f;
	GLfloat height = 800.0f;
	GLfloat bgcolor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
} window;

static GLfloat xForgat = 0.0f;
static GLfloat yForgat = 0.0f;
static GLfloat zForgat = 0.0f;
static GLfloat sForgat = 2.5f;	// forgatás sebessége

/*  Kocka kirajzolása
	   (H)-----(G)
	   /|      /|
	  / |     / |
	(D)-----(C) |
	 | (E)---|-(F)
	 | /     | /
	 |/      |/
	(A)-----(B)      */
namespace Cube {
	GLfloat colors[6][3] = { 
		{ 1.0, 0.0, 0.0 },//	Piross
		{ 0.0, 0.0, 0.0 },//	Fekete
		{ 0.0, 1.0, 0.0 },//	Zöld
		{ 1.0, 1.0, 0.0 },//	Sárga
		{ 1.0, 0.0, 1.0 },//	Lila
		{ 0.0, 0.0, 1.0 } //	Kék
	};
	int v = 1;
	GLint points[6][4][3] = {
		{ {-v,-v, v },{ v,-v, v },{ v, v, v },{-v, v, v } },//	A-B-C-D
		{ {-v,-v,-v },{ v,-v,-v },{ v, v,-v },{-v, v,-v } },//	E-F-G-H
		{ {-v,-v,-v },{-v,-v, v },{-v, v, v },{-v, v,-v } },//	E-A-D-H
		{ { v,-v,-v },{ v,-v, v },{ v, v, v },{ v, v,-v } },//	F-B-C-G
		{ { v, v,-v },{ v, v, v },{-v, v, v },{-v, v,-v } },//	G-C-D-H
		{ { v,-v,-v },{ v,-v, v },{-v,-v, v },{-v,-v,-v } } //	F-B-A-E
	};
	void draw() {
		for (int i = 0; i < 6; i++) {
			glBegin(GL_POLYGON);
			for (int j = 0; j < 4; j++) {
				glColor3f(    colors[i][0],   colors[i][1],   colors[i][2]);
				glVertex3d(points[i][j][0],points[i][j][1],points[i][j][2]);
			}
			glEnd();
		}
	}
}
static void init(void) {
	glClearColor(window.bgcolor[0], window.bgcolor[1], window.bgcolor[2], window.bgcolor[3]);
	gluOrtho2D(0.f, window.weight, 0.f, window.height);
	glMatrixMode(GL_PROJECTION);
	// LáthatóSág: Z-puffer algoritmus
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glLoadIdentity();

	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 1000.0);
};

// Általános gombok hatása
static void key(unsigned char gomb, int x, int y) {
	switch (gomb) {
		// Esc - Program bezárása
	case 27:
		exit(0);
		break;
	case '8':
		xForgat -= sForgat;
		break;
	case '2':
		xForgat += sForgat;
		break;
	case '4':
		yForgat -= sForgat;
		break;
	case '6':
		yForgat += sForgat;
	case '1':
		zForgat += sForgat;
		break;
	case '3':
		zForgat -= sForgat;
		break;
	}
	if (xForgat > 356.0f)
		xForgat = 0.0f;
	if (xForgat < -1.0f)
		xForgat = 355.0f;
	if (yForgat > 356.0f)
		yForgat = 0.0f;
	if (yForgat < -1.0f)
		yForgat = 355.0f;
	if (zForgat > 356.0f)
		zForgat = 0.0f;
	if (zForgat < -1.0f)
		zForgat = 355.0f;
	glutPostRedisplay();
}


// Transzformáció
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -6.0);
	glRotatef(xForgat, 1.0, 0.0, 0.0);
	glRotatef(yForgat, 0.0, 1.0, 0.0);
	glRotatef(zForgat, 0.0, 0.0, 1.0);
	Cube::draw();
	glutSwapBuffers();
};

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize((int)window.weight, (int)window.height);
	glutInitWindowPosition((int)window.positionX, (int)window.positionY);
	glutCreateWindow("Turánszki Róbert (U5KA1V) - Beadandó 2.");
	init();
	glutDisplayFunc(Display);
	glutKeyboardFunc(key);
	glutMainLoop();
	return 0;
};
