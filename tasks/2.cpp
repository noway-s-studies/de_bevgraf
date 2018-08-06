#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>

GLsizei winWidth = 600, winHeight = 600;

int projection_mode = 2;			// Vetítési beállítás; középpontos/perspektív = 1; párhuzamos=2 
float angleX = 0, angleY = 0, angleZ = 0;	// Forgatási szögek
GLfloat animSpeed = 0.1f;			// Animáció lépték
vec3	S = { 5.0, 5.0, 5.0 },			// Skálázás
		center = { 0, 0, 20.0 };	// Néző pozíciója perspektív nézetben
mat4 WtV, projection;

struct surface {
	vec3 point[4];
	vec3 normal;
	bool isVisible;
};

surface realCube[6] = {};
surface drawCube[6] = {};


/*  Kocka
	   (B)-----(A)
	   /|      /|
	  / |     / |
	(F)-----(E) |
	 | (C)---|-(D)
	 | /     | /
	 |/      |/
	(G)-----(H)      */

void setNormalVectors() {				// a 6 oldal normál vektirának megadása (láthatóság)
	for (int i = 0; i < 6; i++) {
		realCube[i].normal = cross(		// jobbsodrású vektoriális szorzat
			realCube[i].point[1] - realCube[i].point[0],
			realCube[i].point[2] - realCube[i].point[0]);
	}
}
void setColor(int c) {
	switch (c) {
		case 0:	// A-E-H-D
			return glColor3f(1.0, 0.0, 0.0);	//	Piross
		case 1:	// E-F-G-H
			return glColor3f(0.0, 0.0, 0.0);	//	Fekete
		case 2: // B-F-E-A
			return glColor3f(0.0, 1.0, 0.0);	//	Zöld
		case 3:	// A-D-C-B
			return glColor3f(1.0, 1.0, 0.0);	//	Sárga
		case 4:	// H-G-C-D
			return glColor3f(1.0, 0.0, 1.0);	//	Lila
		case 5:	// G-F-B-C
			return glColor3f(0.0, 0.0, 1.0);	//	Kék
	}
}
void makeCube() {
	GLfloat s = 1;
	vec3 cube[8] = {
		{ s, s,-s },	// A
		{-s, s,-s },	// B
		{-s,-s,-s },	// C
		{ s,-s,-s },	// D
		{ s, s, s },	// E
		{-s, s, s },	// F
		{-s,-s, s },	// G
		{ s,-s, s }		// H
	};
	// A-E-H-D
	realCube[0].point[0] = cube[0];
	realCube[0].point[1] = cube[4];
	realCube[0].point[2] = cube[7];
	realCube[0].point[3] = cube[3];
	// E-F-G-H
	realCube[1].point[0] = cube[4];
	realCube[1].point[1] = cube[5];
	realCube[1].point[2] = cube[6];
	realCube[1].point[3] = cube[7];
	// B-F-E-A
	realCube[2].point[0] = cube[1];
	realCube[2].point[1] = cube[5];
	realCube[2].point[2] = cube[4];
	realCube[2].point[3] = cube[0];
	// A-D-C-B
	realCube[3].point[0] = cube[0];
	realCube[3].point[1] = cube[3];
	realCube[3].point[2] = cube[2];
	realCube[3].point[3] = cube[1];
	// H-G-C-D
	realCube[4].point[0] = cube[7];
	realCube[4].point[1] = cube[6];
	realCube[4].point[2] = cube[2];
	realCube[4].point[3] = cube[3];
	// G-F-B-C
	realCube[5].point[0] = cube[6];
	realCube[5].point[1] = cube[5];
	realCube[5].point[2] = cube[1];
	realCube[5].point[3] = cube[2];
	setNormalVectors();
}

void setVisibility(mat4 SR) {
	for (int side = 0; side < 6; side++) {
		drawCube[side].isVisible = false;
		vec3 normalSR = hToIh(
			SR *
			ihToH(
				realCube[side].normal
			)
		);

		if (projection_mode == 1) {
			for (int i = 0; i < 4; i++) {
				vec3 pointSR = hToIh(
					SR *
					ihToH(
						realCube[side].point[i]
					)
				);
				float scalar = dot(			// Skalaris szorzat
					center - pointSR,
					normalSR);
				if (scalar > 0) {
					drawCube[side].isVisible = true;
				}
			}
		}
		else {
			if (normalSR.z > 0) {
				drawCube[side].isVisible = true;
			}
		}
	}
}

void transform() {
	projection_mode == 1 ? projection = perspective(center.z) : projection = ortho();
	mat4 SR = scale(S) * (rotateX(angleX) * rotateY(angleY) * rotateZ(angleZ));
	setVisibility(SR);
	for (int side = 0; side < 6; side++) {
		for (int i = 0; i < 4; i++) {
			vec4 transformPoint = WtV * projection * SR * ihToH(realCube[side].point[i]);
			if (transformPoint.w != 0) {
				drawCube[side].point[i] = { hToIh(transformPoint) };
			}
		}

	}
}

void drawRealCube() {
	for (int side = 0; side < 6; side++) {
		if (drawCube[side].isVisible == true) {
			glBegin(GL_POLYGON);
				setColor(side);
				for (int i = 0; i < 4; i++) {
					glVertex2f(
						drawCube[side].point[i].x, 
						drawCube[side].point[i].y);
				}
			glEnd();
		}
	}
}

void makeViewportMatrix() {
	vec2 windowSize = { 30, 30 };
	vec2 windowPosition = { -15, -15 };
	vec2 viewportSize = { 600, 600 };
	vec2 viewportPosition = {
		winWidth / 2 - viewportSize.x / 2,
		winHeight / 2 - viewportSize.y / 2 };
	WtV = windowToViewport3(
		windowPosition,			// vpPos 0 3 * vSize/wSize 0 0 * -wPos -> 0 3
		windowSize,			//       1 3               1 1            1 3
		viewportPosition,		//       2 3               2 2            2 3
		viewportSize);			//     eltolás           skálázás       eltolás
}

///////////////////////////////////////////////////////////////////////////////
// main
///////////////////////////////////////////////////////////////////////////////

void update(int v) {
	transform();
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

static void key(unsigned char gomb, int x, int y) {
	switch (gomb) {
	case 27:	// Esc - Program bezárása
		exit(0);
		break;
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
	case '1':
		projection_mode = 1;
		break;
	case '2':
		projection_mode = 0;
		break;
	case '+':	// Nagyítás
		if (S.x < 10.0f) {
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
	drawRealCube();
	transform();
	glutSwapBuffers();
}

void init() {
	glMatrixMode(GL_PROJECTION);	// módosítandó transzformáció kiválasztása
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);	// vágósík beállítása
	makeCube();
	makeViewportMatrix();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Bevgraf beadandó 2.");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}