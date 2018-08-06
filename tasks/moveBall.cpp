#include <math.h>
#include <stdlib.h>
#if defined(__APPLE__)
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#else
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        #include <windows.h>
    #endif
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif
#ifndef Mat_PI
    #define Mat_PI 3.14159265359
#endif

GLint Display_Read[2]   = { 800, 600 };
GLint Display_Write[2]  = { 800, 600 };
GLint Display_Start[2]  = { 300, 200 };
GLfloat MoveBall[2]     = { 1, 1 };

void myBall(GLfloat Ox, GLfloat Oy, GLfloat r) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glColor4f(1.0f,1.0f,0.0f,0.5f);
        for (GLfloat t = 0; t <= 2 * Mat_PI; t += 0.05)
            glVertex2d(Ox + r * cos(t), Oy + r * sin(t));
    glEnd();
}
void myMoveBall( GLfloat Ox, GLfloat Oy, GLfloat r, GLfloat x, GLfloat y){
    myBall(Ox, Oy, 10.0);
    Ox += x;
    if ( Ox + r > Display_Read[0] || Ox - r < 0  )
        x *= -1;
}

// Ha éppen nincs semmilyen más esemény
void onIdle() {
    // Visszaadja a jelenlegi időt miliszekundumban
    static GLint last_time  = glutGet(GLUT_ELAPSED_TIME); 
    GLint present_time      = glutGet(GLUT_ELAPSED_TIME);
    // Az előző onIdle óta eltelt idő
    GLint diff              = present_time - last_time; 
    // A következő meghíváskor az előző idő majd a mostani idő lesz.
    last_time               = present_time; 
    // Figyelt időegység (5 miliszekundum)
    const GLint time_step   = 5;
    glutPostRedisplay();
}
// Egérgomb állapotának megváltozása
void onMouse( GLint button, GLint action, GLint xMouse , GLint yMouse ) {
    if(button==GLUT_LEFT_BUTTON && action==GLUT_DOWN){
        myMoveBall(xMouse, Display_Read[1]-yMouse, 10.0, MoveBall[0], MoveBall[1]);
        glutPostRedisplay();
    }
}
// Lenyomott egérgomb mozgó állapot
void onMouseMotion(int, int) {
}
// Billentyű lenyomás kezelése
void onKeyboard(unsigned char, int, int) {
}
// Billentyű felengedés kezelése
void onKeyboardUp(unsigned char, int, int) {
}
// Inicializálás (globális változók megadása)
void onInitialization() {
    // Háttérszín beallítása.
    glClearColor(0.0, 0.0, 0.0, 1); 
    // Kepernyőtörlés (BackBuffer) törlése a megadott színnel
    glClear(GL_COLOR_BUFFER_BIT);  
    // Rajzvászon koordinátáinak megadása
    gluOrtho2D (0.0,(GLfloat)Display_Write[0],0.0,(GLfloat)Display_Write[1]); 
}

// Képernyő törlése -> rajzolás
void onDisplay() { 
    glFlush();
}

int main(int argc, char **argv) {
    // GLUT inicializálása 
    glutInit(&argc, argv);
    // Program ablakmérete
    glutInitWindowSize(Display_Read[0], Display_Read[1]);
    // Program ablak kiinduló pozíciója
    glutInitWindowPosition(Display_Start[0], Display_Start[1]);
    // Megjelenítési mód inicializálása 
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    // Program ablak létrehozása
    glutCreateWindow("Grafika pelda program");   

    onInitialization(); 
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onIdle);    
    
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseMotion);

    // Vezérlés átadása a GLUT-nak (üzenetkezelő ciklus indítása)
    glutMainLoop(); 
    return 0;
}