#include <GL/glut.h>
#include <math.h>
#define PI 3.141592653589793
typedef struct point2d { GLdouble x, y; } POINT2D;

GLint keyStates[256];
GLfloat xh0 = 400, yh0 = 0, xh1 = 400, yh1 = 600;   // Függõleges egyenes
GLfloat xv0 = 0, yv0 = 300, xv1 = 800, yv1 = 300;   // Vízszintes egyenes
GLfloat delta = 0.01;

void init (void){
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
void keyPressed (unsigned char key, int x, int y) { // billentyû lenyom
    keyStates[key] = 1;
}
void keyUp (unsigned char key, int x, int y) {      // billentyû felenged
    keyStates[key] = 0;
}
void keyOperations ( ) {    // billentyûzet
    if (keyStates['a']) { xh0-=delta; xh1+=delta; }               // jobbra
    if (keyStates['d']) { xh0+=delta; xh1-=delta; }               // balra
    if (keyStates['s']) { yv0-=delta; yv1+=delta; }               // fel
    if (keyStates['w']) { yv0+=delta; yv1-=delta; }               // le
    glutPostRedisplay( );
}
void lineSegment (void){
    keyOperations();
    glClear (GL_COLOR_BUFFER_BIT);  // beállított színnel feltöltjük a képernyõ puffert / képernyõ törlés
    glColor3f (0.3, 0.3, 0.3);      // beállítjuk az alapértelmezett színt, 3f=3 lebegõ pontos típus
    glBegin (GL_LINES);             // egyenes rajzolása
        glVertex2f (xh0, yh0);      // függõleges kezdõpont
        glVertex2f (xh1, yh1);      // függõleges végpont
        glVertex2f (xv0, yv0);      // vízszintes kezdõpont
        glVertex2f (xv1, yv1);      // vízszintes végpont
    glEnd ( );
    circle( initPoint2D( 100, 200 ), 30);
    glutSwapBuffers( );
}
int main (int argc, char** argv){
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Lapmozgatás, villózás kikerülése
    glutInitWindowPosition (100, 100);              // A létrejövõ grafikus ablak kezdõ pozíciója a kijelzõn
    glutInitWindowSize (800, 600);                  // Ablak mérete
    glutCreateWindow ("1. Pattogó labdák");         // Program címe
    init ( );
    glutDisplayFunc (lineSegment);                  // megadjuk, mely függvény tartalmazza a grafikai utasításokat

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop ( );

    return 0;
}


