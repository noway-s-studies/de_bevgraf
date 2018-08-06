#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 800, winHeight = 600;

//globális változók
GLint keyStates[256];
int central_projection_mode = 1;
int N = 8;
vec3 S = { 3.0, 3.0, 3.0 };
vec3 centrum = { 0, 0, 10.0 };
mat4 w2v, rotatedX, rotatedZ, scaled, projection;
float alphaX = 0;
float alphaY = 0;
float alphaZ = 0;


struct face {
	vec3 p[4];
	vec3 normal;
	bool isVisible;
};

face cubeFaces[6] = {};
face drawableCube[6] = {};

void setNormalVectors() {
	for (int i = 0; i < 6; i++) {
		cubeFaces[i].normal = cross(cubeFaces[i].p[1] - cubeFaces[i].p[0], cubeFaces[i].p[2] - cubeFaces[i].p[0]);
	}
}

void setVisibility(mat4 A) {
	for (int idx = 0; idx < 6; idx++) {
		face f = cubeFaces[idx];
		drawableCube[idx].isVisible = false;
		vec3 rotatedNormal = hToIh(A * ihToH(f.normal));

		if (central_projection_mode == 1) {
			for (int i = 0; i < 4; i++) {
				vec3 rotatedPoint = hToIh(A * ihToH(cubeFaces[idx].p[i]));
				float scalar = dot(centrum - rotatedPoint, rotatedNormal);
				if (scalar > 0) {
					drawableCube[idx].isVisible = true;
				}
			}
		}
		else {
			if (rotatedNormal.z > 0) {
				drawableCube[idx].isVisible = true;
			}
		}
	}
}

void initCube() {
	//                      P0                   P1                   P2                 P3                  P4                 P5                  P6                  P7
	vec3 cube[8] = { { 0.5, 0.5, -0.5 },{ -0.5, 0.5, -0.5 },{ -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },{ 0.5, 0.5, 0.5 },{ -0.5, 0.5, 0.5 },{ -0.5, -0.5, 0.5 },{ 0.5, -0.5, 0.5 } };

	cubeFaces[0].p[0] = cube[0];
	cubeFaces[0].p[1] = cube[4];
	cubeFaces[0].p[2] = cube[7];
	cubeFaces[0].p[3] = cube[3];

	cubeFaces[1].p[0] = cube[4];
	cubeFaces[1].p[1] = cube[5];
	cubeFaces[1].p[2] = cube[6];
	cubeFaces[1].p[3] = cube[7];

	cubeFaces[2].p[0] = cube[1];
	cubeFaces[2].p[1] = cube[5];
	cubeFaces[2].p[2] = cube[4];
	cubeFaces[2].p[3] = cube[0];

	cubeFaces[3].p[0] = cube[0];
	cubeFaces[3].p[1] = cube[3];
	cubeFaces[3].p[2] = cube[2];
	cubeFaces[3].p[3] = cube[1];

	cubeFaces[4].p[0] = cube[7];
	cubeFaces[4].p[1] = cube[6];
	cubeFaces[4].p[2] = cube[2];
	cubeFaces[4].p[3] = cube[3];

	cubeFaces[5].p[0] = cube[6];
	cubeFaces[5].p[1] = cube[5];
	cubeFaces[5].p[2] = cube[1];
	cubeFaces[5].p[3] = cube[2];
	setNormalVectors();
}

void initMatrices() {
	rotatedZ = rotateZ(pi() / 4);
	scaled = scale(S);
	vec2 windowSize = { 10, 10 };
	vec2 windowPosition = { -5, -5 };
	vec2 viewportSize = { 450, 450 };
	vec2 viewportPosition = { winWidth / 2 - viewportSize.x / 2, winHeight / 2 - viewportSize.y / 2 };
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(1);
	initCube();
	initMatrices();
}

void drawFaces() {
	for (int idx = 0; idx < 6; idx++) {

		switch (idx) {
		case 0:
			glColor3f(0.8, 0.8, 0);
			break;
		case 1:
			glColor3f(0.4, 0.6, 0);
			break;
		case 2:
			glColor3f(0.6, 0.2, 0.4);
			break;
		case 3:
			glColor3f(0.8, 0.2, 0.2);
			break;
		case 4:
			glColor3f(0.0, 0.2, 0.4);
			break;
		case 5:
			glColor3f(0.6, 0.4, 0.2);
			break;
		}
		
		if (drawableCube[idx].isVisible == true) {
			glBegin(GL_POLYGON);
			for (int i = 0; i < 4; i++) {
				glVertex2f(drawableCube[idx].p[i].x, drawableCube[idx].p[i].y);
			}
			glEnd();
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawFaces();
	glutSwapBuffers();
}

void transform() {
	rotatedZ = rotateZ(pi() / 4);
	scaled = scale(S);
	central_projection_mode == 1 ? projection = perspective(centrum.z) : projection = ortho();
	mat4 rotation = rotateX(alphaX) * rotateY(alphaY) * rotatedZ;
	mat4 A = scaled * rotation;
	setVisibility(A);
	mat4 M = w2v * projection * A;

	for (int idx = 0; idx < 6; idx++) {
		face f = cubeFaces[idx];

		for (int i = 0; i < 4; i++) {

			vec4 pointH = ihToH(f.p[i]);
			vec4 transformedPoint = M * pointH;

			if (transformedPoint.w != 0) {
				vec3 result = hToIh(transformedPoint);
				if (result.z == 0) {
					drawableCube[idx].p[i] = {result};
				}
			}
		}

	}
}

void update(int v) {
	transform();
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y){
	keyStates[key] = 0;
}

void keyOperations(int value){

	//balra
	if (keyStates['a']) {
		alphaX += 0.05;
	}

	//jobbra
	if (keyStates['d']) {
		alphaX -= 0.05;
	}

	//lefelé
	if (keyStates['s']) {
		alphaY -= 0.05;
	}

	//felfelé
	if (keyStates['w']) {
		alphaY += 0.05;
	}

	//centrális vetítési mód
	if (keyStates['c']) {
		central_projection_mode = 1;
	}

	//merõleges vetítési mód
	if (keyStates['m']) {
		central_projection_mode = 0;
	}

	//közelít
	if (keyStates['+']) {
		S.x += 0.03;
		S.y += 0.03;
		S.z += 0.03;
	}

	//távolít
	if (keyStates['-']) {
		S.x -= 0.03;
		S.y -= 0.03;
		S.z -= 0.03;
	}

	glutPostRedisplay();
	glutTimerFunc(5, keyOperations, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bevgraf beadandó 2.");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(5, update, 0);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(5, keyOperations, 0);
	glutMainLoop();
	return 0;
}