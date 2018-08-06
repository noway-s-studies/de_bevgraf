#include <GL/glut.h>					// (or others, depending on the system in use)
#include <math.h>
#define PI 3.141592653589793
typedef struct point2d { GLdouble x, y; } POINT2D;

void init(){
    glClearColor (1.0, 1.0, 1.0, 0.0);              // Törlõszín
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 800.0, 0.0, 600.0);            // 0x0 és 800x600 között történik a koordinátázás
}

POINT2D initPoint2D(GLdouble x, GLdouble y) {
	POINT2D P;
	P.x = x;
	P.y = y;
	return P;
}
void circle(POINT2D O, GLdouble r) {
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * PI; t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}
void draw(){
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.0f, 0.4f, 0.2f);
	circle( initPoint2D( 100, 200 ), 30);
    glFlush ( );
}
int main (int argc, char** argv){
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (50, 100);   // Set top-left display-window position.
    glutInitWindowSize (400, 300);      // Set display-window width and height.
    glutCreateWindow ("An Example OpenGL Program"); // Create display window.
    init ( );                            // Execute initialization procedure.
    glutDisplayFunc (draw);       // Send graphics to display window.
    glutMainLoop ( );                    // Display everything and wait.
    return 0;
}
