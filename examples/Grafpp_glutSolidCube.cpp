#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #include <windows.h>
  #endif
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#endif

void onDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutSolidCube(1.0f);

  glutSwapBuffers();
}

void onIdle() {
  glutPostRedisplay();
}

void onInitialization();

void onMouse(int, int, int, int) {}

void onMouseMotion(int, int) {}

void onKeyboard(unsigned char, int, int) {}

void onKeyboardUp(unsigned char, int, int) {}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow("Grafika pelda program");

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  onInitialization();

  glutDisplayFunc(onDisplay);
  glutMouseFunc(onMouse);
  glutIdleFunc(onIdle);
  glutKeyboardFunc(onKeyboard);
  glutKeyboardUpFunc(onKeyboardUp);
  glutMotionFunc(onMouseMotion);

  glutMainLoop();

  return 0;
}

// Ez a rész szükséges ahhoz, hogy a kockát 3d-ben lásd, 
// de a példa szempontjából most lényegtelen, hogy 
// ténylegesen mit csinál.
void onInitialization() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(60, 1, 0.1, 20);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(-3, 2, -2, 0, 0, 0, 0, 1, 0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  float p[4] = {-2, 4, -1, 0};
  glLightfv(GL_LIGHT0, GL_POSITION, p);
  float c[4] = {0.0f, 0.4f, 1.0f, 1.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, c);
}