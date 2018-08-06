#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define maxNumberOfPoints 64
void myKeyboardFunc( unsigned char key, int x, int y );
void myMouseFunc( int button, int state, int x, int y );
void displayLines(void);
void removeFirstPoint();
void removeLastPoint();
void addNewPoint( float x, float y );
void initRendering();
void resizeWindow(int w, int h); 
float ArrayOfPoints[maxNumberOfPoints][2];
int currentNumberPoints = 0;

// Window size in pixels
int frameHeight;
int frameWidth;

void keysFunction (unsigned char key, int x, int y){
   switch (key) {
   case 27:         // Escape key
      exit(0);
      break;
   }
}
void removeFirstPoint() {
   int i;
   if ( currentNumberPoints>0 ) {
      // Remove the first point, slide the rest down
      currentNumberPoints--;
      for ( i=0; i<currentNumberPoints; i++ ) {
         ArrayOfPoints[i][0] = ArrayOfPoints[i+1][0];
         ArrayOfPoints[i][1] = ArrayOfPoints[i+1][1];
      }
   }
}
// Left button presses place a control point.
void myMouseFunc( int button, int state, int x, int y ) {
   if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN ) {
      float xPos = ((float)x)/((float)(frameWidth-1));
      float yPos = ((float)y)/((float)(frameHeight-1));
      yPos = 1.0f-yPos;         // Flip value since y position is from top row.
      addNewPoint( xPos, yPos );
      glutPostRedisplay();
   }
}
void removeLastPoint() {
   if ( currentNumberPoints>0 ) {
      currentNumberPoints--;
   }
}
void addNewPoint( float x, float y ) {
   if ( currentNumberPoints>=maxNumberOfPoints ) {
      removeFirstPoint();
   }
   ArrayOfPoints[currentNumberPoints][0] = x;
   ArrayOfPoints[currentNumberPoints][1] = y;
   currentNumberPoints++;
}
void displayLines(void){
   int i;
   glClear(GL_COLOR_BUFFER_BIT);
   // Draw the line segments
   glColor3f(1.0f, 0.0f, 0.8f);         // Reddish/purple lines
   if ( currentNumberPoints>1 ) {
      glBegin( GL_LINE_STRIP );
      for ( i=0; i<currentNumberPoints; i++ ) {
         glVertex2f( ArrayOfPoints[i][0], ArrayOfPoints[i][1] );
      }
      glEnd();
   }
   // Draw the interpolated points second.
   glColor3f( 0.0f, 0.0f, 0.0f);         // Draw points in black
   glBegin( GL_POINTS );
   for ( i=0; i<currentNumberPoints; i++ ) {
      glVertex2f( ArrayOfPoints[i][0], ArrayOfPoints[i][1] );
   }
   glEnd();
   glFlush();
}
void initRendering() {
   glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
   // Make big points and wide lines.  (This may be commented out if desired.)
   glPointSize(8);
   glLineWidth(5);
   // The following commands should induce OpenGL to create round points and 
   //   antialias points and lines.  (This is implementation dependent unfortunately, and
   //  may slow down rendering considerably.)
   //  You may comment these out if you wish.
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_LINE_SMOOTH);
   glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);   // Make round points, not square points
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);      // Antialias the lines
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void resizeWindow(int w, int h){
   frameHeight = (h>1) ? h : 2;
   frameWidth = (w>1) ? w : 2;
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);  // Always view [0,1]x[0,1].
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
int main(int argc, char** argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   initRendering();
   glutDisplayFunc(displayLines);
   glutReshapeFunc(resizeWindow);
   glutKeyboardFunc(keysFunction);
   glutMouseFunc(myMouseFunc);
   glutMainLoop();
   return 0;               // This line is never reached
}
