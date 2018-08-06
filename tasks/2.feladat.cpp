#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 600, winHeight = 600;

float angleX = pi() / 2, angleY = 0, angleZ = 0;
GLfloat animSpeed = 0.1f;
vec3 S = { 10.0, 10.0, 10.0 },
center = { 0, 0, 70.0 };
mat4 WtV1, WtV2, projection;

struct surface {
	vec3 point[3];
	vec3 normal;
	bool isVisible;
};

surface realDiam[10] = {};
surface drawDiam1[10] = {};
surface drawDiam2[10] = {};

void setNormalVectors() {
	for (int i = 0; i < 10; i++) {
		realDiam[i].normal = cross(
			realDiam[i].point[1] - realDiam[i].point[0],
			realDiam[i].point[2] - realDiam[i].point[0]);
	}
}
void setColor(int c) {
	switch (c) {
	case 0:
		return glColor3f(0.2, 0.0, 1.0);
	case 1:
		return glColor3f(0.9, 0.4, 0.2);
	case 2:
		return glColor3f(0.8, 0.1, 0.2);
	case 3:
		return glColor3f(0.1, 0.8, 0.0);
	case 4:
		return glColor3f(0.4, 0.0, 0.2);
	case 5:
		return glColor3f(0.6, 0.7, 1.0);
	case 6:
		return glColor3f(0.3, 0.2, 0.8);
	case 7:
		return glColor3f(0.5, 0.5, 0.0);
	case 8:
		return glColor3f(0.5, 0.7, 0.1);
	case 9:
		return glColor3f(0.0, 0.1, 0.2);
	}
}

void makeCube() {
	vec3 allPoints[7] = {
		{ 0, 3, 0 },	// A 0
		{-2, 1, 0 },	// B 1
		{-1,-1, 0 },	// C 2
		{ 1,-1, 0 },	// D 3
		{ 2, 1, 0 },	// E 4
		{ 0, 0, 5 },	// X 5
		{ 0, 0,-5 }		// Y 6
	};
	// A-B-Y
	realDiam[0].point[0] = allPoints[0];
	realDiam[0].point[1] = allPoints[1];
	realDiam[0].point[2] = allPoints[6];
	// B-C-Y
	realDiam[1].point[0] = allPoints[1];
	realDiam[1].point[1] = allPoints[2];
	realDiam[1].point[2] = allPoints[6];
	// C-D-Y
	realDiam[2].point[0] = allPoints[2];
	realDiam[2].point[1] = allPoints[3];
	realDiam[2].point[2] = allPoints[6];
	// D-E-Y
	realDiam[3].point[0] = allPoints[3];
	realDiam[3].point[1] = allPoints[4];
	realDiam[3].point[2] = allPoints[6];
	// E-A-Y
	realDiam[4].point[0] = allPoints[4];
	realDiam[4].point[1] = allPoints[0];
	realDiam[4].point[2] = allPoints[6];
	// B-A-X
	realDiam[5].point[0] = allPoints[1];
	realDiam[5].point[1] = allPoints[0];
	realDiam[5].point[2] = allPoints[5];
	// C-B-X
	realDiam[6].point[0] = allPoints[2];
	realDiam[6].point[1] = allPoints[1];
	realDiam[6].point[2] = allPoints[5];
	// D-C-X
	realDiam[7].point[0] = allPoints[3];
	realDiam[7].point[1] = allPoints[2];
	realDiam[7].point[2] = allPoints[5];
	// E-D-X
	realDiam[8].point[0] = allPoints[4];
	realDiam[8].point[1] = allPoints[3];
	realDiam[8].point[2] = allPoints[5];
	// A-E-X
	realDiam[9].point[0] = allPoints[0];
	realDiam[9].point[1] = allPoints[4];
	realDiam[9].point[2] = allPoints[5];
	setNormalVectors();
}

void setVisibility1(mat4 SR) {
	for (int side = 0; side < 10; side++) {
		drawDiam1[side].isVisible = false;
		vec3 normalSR = hToIh(SR * ihToH(realDiam[side].normal)) * -1;
		if (normalSR.z > 1) {
			drawDiam1[side].isVisible = true;
		}
	}
}

void setVisibility2(mat4 SR) {
	for (int side = 0; side < 10; side++) {
		drawDiam2[side].isVisible = false;
		vec3 normalSR2 = hToIh(SR * ihToH(realDiam[side].normal));
		for (int i = 0; i < 3; i++) {
			vec3 pointSR = hToIh(SR *ihToH(realDiam[side].point[i]));
			float scalar = dot(pointSR - center
				, normalSR2);
			if (scalar > 0) {
				drawDiam2[side].isVisible = true;
			}
		}
	}
}

void transform() {
	mat4 SR = scale(S) * (rotateX(angleX) * rotateY(angleY) * rotateZ(angleZ));
	setVisibility1(SR);
	for (int side = 0; side < 10; side++) {
		for (int i = 0; i < 3; i++) {
			vec4 transformPoint = WtV1 * ortho() * SR * ihToH(realDiam[side].point[i]);
			if (transformPoint.w != 0) {
				drawDiam1[side].point[i] = { hToIh(transformPoint) };
			}
		}

	}
	setVisibility2(SR);
	for (int side = 0; side < 10; side++) {
		for (int i = 0; i < 3; i++) {
			vec4 transformPoint = WtV2 * perspective(center.z) * SR * ihToH(realDiam[side].point[i]);
			if (transformPoint.w != 0) {
				drawDiam2[side].point[i] = { hToIh(transformPoint) };
			}
		}
	}
}

void drawrealDiam() {
	for (int side = 0; side < 10; side++) {
		if (drawDiam1[side].isVisible == true) {
			glBegin(GL_POLYGON);
			setColor(side);
			for (int i = 0; i < 3; i++) {
				glVertex2f(
					drawDiam1[side].point[i].x,
					drawDiam1[side].point[i].y);
			}
			glEnd();
		}
	}
	for (int side = 0; side < 10; side++) {
		if (drawDiam2[side].isVisible == true) {
			glBegin(GL_POLYGON);
			setColor(side);
			for (int i = 0; i < 3; i++) {
				glVertex2f(
					drawDiam2[side].point[i].x,
					drawDiam2[side].point[i].y);
			}
			glEnd();
		}
	}
}

void makeViewportMatrix() {
	vec2 windowSize = { 100, 100 };
	vec2 windowPosition = { -50, -50 };
	vec2 viewportSize = { 300, 300 };
	vec2 viewportPosition = {
		winWidth / 2 / 2 - viewportSize.x / 2,
		winHeight / 2 - viewportSize.y / 2 };
	WtV1 = windowToViewport3(
		windowPosition,
		windowSize,
		viewportPosition,
		viewportSize);

	viewportPosition = {
		winWidth / 2 + (winWidth / 2 / 2 - viewportSize.x / 2),
		winHeight / 2 - viewportSize.y / 2
	};
	WtV2 = windowToViewport3(
		windowPosition,
		windowSize,
		viewportPosition,
		viewportSize);
}


void update(int v) {
	transform();
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

static void key(unsigned char gomb, int x, int y) {
	switch (gomb) {
	case '8':	// Fel
		angleX -= animSpeed;
		break;
	case '5':	// Le
		angleX += animSpeed;
		break;
	case '4':	// Jobbra
		angleY -= animSpeed;
		break;
	case '6':	// Balra
		angleY += animSpeed;
		break;
	case '7':	// Jobbra
		angleZ -= animSpeed;
		break;
	case '9':	// Balra
		angleZ += animSpeed;
		break;
	case '+':	// Nagyítás
		if (S.x < 300.0f) {
			S.x += animSpeed;
			S.y += animSpeed;
			S.z += animSpeed;
		}
		break;
	case '-':	// Kicsinyítés
		if (S.x > 1.0f) {
			S.x -= animSpeed;
			S.y -= animSpeed;
			S.z -= animSpeed;
		}
		break;
	}
	glutPostRedisplay();
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawrealDiam();
	transform();
	glutSwapBuffers();
}

void init() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	makeCube();
	makeViewportMatrix();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2. feladat");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}