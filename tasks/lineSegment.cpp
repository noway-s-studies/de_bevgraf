#include <GL/glut.h>
#include <math.h>
#define PI 3.141592653589793
typedef struct point2d { GLdouble x, y; } POINT2D;

GLint keyStates[256];
GLfloat xh0 = 400, yh0 = 0, xh1 = 400, yh1 = 600;   // F�gg�leges egyenes
GLfloat xv0 = 0, yv0 = 300, xv1 = 800, yv1 = 300;   // V�zszintes egyenes
GLfloat delta = 0.01;

void init (void){
    glClearColor (1.0, 1.0, 1.0, 0.0);              // T�rl�sz�n
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 800.0, 0.0, 600.0);            // 0x0 �s 800x600 k�z�tt t�rt�nik a koordin�t�z�s
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
void keyPressed (unsigned char key, int x, int y) { // billenty� lenyom
    keyStates[key] = 1;
}
void keyUp (unsigned char key, int x, int y) {      // billenty� felenged
    keyStates[key] = 0;
}
void keyOperations ( ) {    // billenty�zet
    if (keyStates['a']) { xh0-=delta; xh1+=delta; }               // jobbra
    if (keyStates['d']) { xh0+=delta; xh1-=delta; }               // balra
    if (keyStates['s']) { yv0-=delta; yv1+=delta; }               // fel
    if (keyStates['w']) { yv0+=delta; yv1-=delta; }               // le
    glutPostRedisplay( );
}
void lineSegment (void){
    keyOperations();
    glClear (GL_COLOR_BUFFER_BIT);  // be�ll�tott sz�nnel felt�ltj�k a k�perny� puffert / k�perny� t�rl�s
    glColor3f (0.3, 0.3, 0.3);      // be�ll�tjuk az alap�rtelmezett sz�nt, 3f=3 lebeg� pontos t�pus
    glBegin (GL_LINES);             // egyenes rajzol�sa
        glVertex2f (xh0, yh0);      // f�gg�leges kezd�pont
        glVertex2f (xh1, yh1);      // f�gg�leges v�gpont
        glVertex2f (xv0, yv0);      // v�zszintes kezd�pont
        glVertex2f (xv1, yv1);      // v�zszintes v�gpont
    glEnd ( );
    circle( initPoint2D( 100, 200 ), 30);
    glutSwapBuffers( );
}
int main (int argc, char** argv){
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Lapmozgat�s, vill�z�s kiker�l�se
    glutInitWindowPosition (100, 100);              // A l�trej�v� grafikus ablak kezd� poz�ci�ja a kijelz�n
    glutInitWindowSize (800, 600);                  // Ablak m�rete
    glutCreateWindow ("1. Pattog� labd�k");         // Program c�me
    init ( );
    glutDisplayFunc (lineSegment);                  // megadjuk, mely f�ggv�ny tartalmazza a grafikai utas�t�sokat

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop ( );

    return 0;
}


