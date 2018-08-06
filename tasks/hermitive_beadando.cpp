#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <vector>
#include <array>
#include <float.h>

GLsizei winWidth = 800, winHeight = 600;
//                     P1            P2            P3           R1            P4           P5            P6          P7
vec2 points[8] = { { 20, 140 }, { 120, 160 }, { 220, 90 }, { 120, 320 }, { 320, 180 },{ 450, 200 },{ 580, 220 },{ 680, 160 } };
float epsylon = 0.001;

GLint dragged = -1;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D( 0.0, 800.0, 0.0, 600.0 );
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glPointSize(2.0);
	glLineWidth(2.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f( 0.0, 0.0, 0.9 );
	glDisable(GL_LINE_STIPPLE);
	glPointSize(5.0);
	glLineWidth(2.0);

	mat24 G;
	vec2 Q;
	vec2 Q_;
	vec4 T;
	vec4 T_;
	mat4 M;
	mat4 M_inv;
	vec2 erintoPont1;
	vec2 erintoPont2;
	vec2 erintoPont3;
	vec2 erintoPont4;
	vec2 erintoPont5;
	vec2 erintoPont6;
	vec2 erintoPont7;
	vec2 erintoPont8;

	GLfloat t1 = -1.5;
	GLfloat t2 = 0;
	GLfloat t3 = 1.5;

	vec4 M1 = { t1 * t1 * t1, t1 * t1, t1, 1 };
	vec4 M2 = { t2 * t2 * t2, t2 * t2, t2, 1 };
	vec4 M3 = { t3 * t3 * t3, t3 * t3, t3, 1 };
	vec4 M4 = { 3 * t1 * t1, 2 * t1, 1, 0 };

	M = { M1, M2, M3, M4, true };
	M_inv = inverse(M);

	//elsõ szakasz
	vec2 P1 = { points[0].x, points[0].y };
	vec2 P2 = { points[1].x, points[1].y };
	vec2 P3 = { points[2].x, points[2].y };
	vec2 R1 = { points[3].x - P1.x, points[3].y - P1.y };
	G = { P1, P2, P3, R1 };

	//görbe 1 kirajzolása
	glBegin(GL_LINE_STRIP);
		for ( GLfloat t = t1; t <= t3 ; t += 0.01 ) {
			T = { t * t * t, t * t, t, 1 };
			T_ = { 3 * t * t, 2 * t, 1, 0 };
			Q = G * M_inv * T;
			Q_ = G * M_inv * T_;

			if (abs(t - t1) < epsylon) {
				erintoPont1 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			else if ( abs(t - t2) < epsylon ) {
				erintoPont2 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			else {
				erintoPont3 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			glVertex2i( Q.x, Q.y );
		}
	glEnd();

	//második szakasz
	vec2 P4 = { P3.x, P3.y };
	vec2 P5 = { points[4].x, points[4].y };
	vec2 P6 = { points[5].x, points[5].y };
	vec2 R2 = { Q_.x, Q_.y };
	G = { P4, P5, P6, R2 };

	//görbe 2 kirajzolása
	glBegin(GL_LINE_STRIP);
		for ( GLfloat t = t1; t < t3; t += 0.01 ) {
			T = { (t * t * t), (t * t), t, 1 };
			T_ = { (3 * t * t), (2 * t), 1, 0 };
			Q = G * M_inv * T;
			Q_ = G * M_inv * T_;

			if ( abs(t - t2) < epsylon ) {
				erintoPont4 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			else{
				erintoPont5 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			glVertex2i( Q.x, Q.y );

		}
	glEnd();


	//harmadik szakasz
	vec2 P7 = { P6.x, P6.y };
	vec2 P8 = { points[6].x, points[6].y };
	vec2 P9 = { points[7].x, points[7].y };
	vec2 R3 = { (Q_.x), (Q_.y) };
	G = { P7, P8, P9, R3 };

	//görbe 3 kirajzolása
	glBegin(GL_LINE_STRIP);
		for ( GLfloat t = t1; t < t3; t += 0.01 ) {
			T = { (t * t * t), (t * t), t, 1 };
			T_ = { (3 * t * t), (2 * t), 1, 0 };
			Q = G * M_inv * T;
			Q_ = G * M_inv * T_;
			
			if ( abs(t - t2) < epsylon ) {
				erintoPont6 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			else {
				erintoPont7 = { Q.x + Q_.x, Q.y + Q_.y };
			}
			glVertex2i( Q.x, Q.y );
		}
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(0.2, 0xFF00);
	glLineWidth(0.2);

	glBegin(GL_LINES);
		// P1 erintõje
		glVertex2i(P1.x, P1.y);
		glVertex2i(erintoPont1.x, erintoPont1.y);

		glColor3f( 0.58, 0.78, 0.82 );

		//P2 érintõje
		glVertex2i( P2.x, P2.y );
		glVertex2i( erintoPont2.x, erintoPont2.y );

		//P3 érintõje
		glVertex2i( P3.x, P3.y );
		glVertex2i( erintoPont3.x, erintoPont3.y );

		//P4 érintõje
		glVertex2i( P5.x, P5.y );
		glVertex2i( erintoPont4.x, erintoPont4.y );

		//P5 érintõje
		glVertex2i( P6.x, P6.y );
		glVertex2i( erintoPont5.x, erintoPont5.y );

		//P6 érintõje
		glVertex2i( P8.x, P8.y );
		glVertex2i( erintoPont6.x, erintoPont6.y );

		//P7 érintõje
		glVertex2i(P9.x, P9.y);
		glVertex2i(erintoPont7.x, erintoPont7.y);

	glEnd();

	//kontroll pontok
	glBegin(GL_POINTS);
		glColor3f( 0.8, 0.2, 0.5 );

		for ( int i = 0; i < 9; i++ ) {
			glVertex2i( points[i].x, points[i].y );
		}
		glVertex2i( erintoPont1.x, erintoPont1.y );

		//nem fogható érintõpontok
		glColor3f( 0.58, 0.58, 0.58 );
		glVertex2i( erintoPont2.x, erintoPont2.y );
		glVertex2i( erintoPont3.x, erintoPont3.y );
		glVertex2i( erintoPont4.x, erintoPont4.y );
		glVertex2i( erintoPont5.x, erintoPont5.y );
		glVertex2i( erintoPont6.x, erintoPont6.y );
		glVertex2i(erintoPont7.x, erintoPont7.y);
	glEnd();
	glutSwapBuffers();
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(points, 8, 8, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Beadandó I. bevgraf");

	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}
