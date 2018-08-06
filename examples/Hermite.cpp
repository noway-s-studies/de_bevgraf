#include <GL/glut.h>

typedef struct vector2d { GLfloat x, y; } VECTOR2D;

typedef struct point2d { GLfloat x, y; } POINT2D;

GLsizei winWidth = 800, winHeight = 600;

VECTOR2D initVector2D(GLfloat x, GLfloat y) {
	VECTOR2D P;
	P.x = x;
	P.y = y;
	return P;
}

POINT2D initPoint2D(GLfloat x, GLfloat y) {
	POINT2D P;
	P.x = x;
	P.y = y;
	return P;
}

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, winWidth, 0.0f, winHeight, 0.0f, 1.0f);
	glLineWidth(3.0);
	glPointSize(5.0);
}

void HermiteCurve()
{
	POINT2D kezdP = initPoint2D( 220, 240 );
	POINT2D vegP = initPoint2D( 420, 240 );

	VECTOR2D kezdE = initVector2D( 100, 200 );
	VECTOR2D vegE = initVector2D( 100, -200 );

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_LINE_STRIP);

		for (GLfloat t = 0; t <= 1; t += 0.01 )
		{
			glVertex2f(  (2*t*t*t-3*t*t+1)*kezdP.x + (-2*t*t*t+3*t*t)*vegP.x + (t*t*t-2*t*t+t)*kezdE.x + (t*t*t-t*t)*vegE.x,
				         (2*t*t*t-3*t*t+1)*kezdP.y + (-2*t*t*t+3*t*t)*vegP.y + (t*t*t-2*t*t+t)*kezdE.y + (t*t*t-t*t)*vegE.y);
		}
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_POINTS);
		glVertex2f(kezdP.x, kezdP.y);
		glVertex2f(vegP.x, vegP.y);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2f(kezdP.x, kezdP.y);
		glVertex2f(kezdP.x + kezdE.x, kezdP.y + kezdE.y );
		glVertex2f(vegP.x, vegP.y);
		glVertex2f(vegP.x + vegE.x, vegP.y + vegE.y);
	glEnd();
	
	glFlush( );
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (50, 100);   // Set top-left display-window position.
    glutInitWindowSize (800, 600);      // Set display-window width and height.
    glutCreateWindow ("Hermite"); // Create display window.
    init ( );                            // Execute initialization procedure.
    glutDisplayFunc (HermiteCurve);       // Send graphics to display window.
    glutMainLoop ( );                    // Display everything and wait.
    return 0;
}