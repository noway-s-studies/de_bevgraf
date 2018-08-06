#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
void myKeyboardFunc( unsigned char key, int x, int y );
void mySpecialKeyFunc( int key, int x, int y );
void drawScene(void);
void initRendering();
void resizeWindow(int w, int h);
int RunMode = 1;
float CurrentAngle = 0.0f;
float AnimateStep = 3.0f;
const double Xmin = 0.0, Xmax = 3.0;
const double Ymin = 0.0, Ymax = 3.0;
void myKeyboardFunc( unsigned char key, int x, int y ){
   switch ( key ) {
   case 'r':
      RunMode = 1-RunMode;
      if ( RunMode==1 ) {
         glutPostRedisplay();
      }
      break;
   case 's':
      RunMode = 1;
      drawScene();
      RunMode = 0;
      break;
   case 27:   // Escape key
      exit(1);
   }
}
void mySpecialKeyFunc( int key, int x, int y ){
   switch ( key ) {
   case GLUT_KEY_UP:      
      if ( AnimateStep < 1.0e3) {
         AnimateStep *= sqrt(2.0);
      }
      break;
   case GLUT_KEY_DOWN:
      if (AnimateStep>1.0e-6) {
         AnimateStep /= sqrt(2.0);
      }
      break;
   }
}
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (RunMode==1) {
        CurrentAngle += AnimateStep;
      if ( CurrentAngle > 360.0 ) {
         CurrentAngle -= 360.0*floor(CurrentAngle/360.0);
      }
   }
   
   // Rotate the image
   glMatrixMode( GL_MODELVIEW );         // Current matrix affects objects positions
   glLoadIdentity();                  // Initialize to the identity
   glTranslatef( 1.5, 1.5, 0.0 );               // Translate rotation center from origin
   glRotatef( CurrentAngle, 0.0, 0.0, 1.0 );      // Rotate through animation angle
   glTranslatef( -1.5, -1.5, 0.0 );            // Translate rotation center to origin

   // Draw three overlapping triangles of different colors
   glBegin( GL_TRIANGLES );
   glColor3f( 1.0, 0.0, 0.0 );
   glVertex3f( 0.3, 1.0, 0.5 );
   glVertex3f( 2.7, 0.85, 0.0 );
   glVertex3f( 2.7, 1.15, 0.0 );

   glColor3f( 0.0, 1.0, 0.0 );
   glVertex3f(2.53, 0.71, 0.5 );
   glVertex3f(1.46, 2.86, 0.0 );
   glVertex3f(1.2, 2.71, 0.0 );

   glColor3f( 0.0, 0.0, 1.0 );
   glVertex3f(1.667, 2.79, 0.5);
   glVertex3f(0.337, 0.786, 0.0);
   glVertex3f(0.597, 0.636, 0.0);
   glEnd();

   // Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();

   if ( RunMode==1 ) {
      glutPostRedisplay();   // Trigger an automatic redraw for animation
   }
}

// Initialize OpenGL's rendering modes
void initRendering(){
    glShadeModel( GL_FLAT );   // The default value of GL_SMOOTH is usually better
    glEnable( GL_DEPTH_TEST );   // Depth testing must be turned on
}

// Called when the window is resized
//      w, h - width and height of the window in pixels.
void resizeWindow(int w, int h){
   double scale, center;
   double windowXmin, windowXmax, windowYmin, windowYmax;
   // Define the portion of the window used for OpenGL rendering.
   glViewport( 0, 0, w, h );   // View port uses whole window
   // Set up the projection view matrix: orthographic projection
   // Determine the min and max values for x and y that should appear in the window.
   // The complication is that the aspect ratio of the window may not match the
   //      aspect ratio of the scene we want to view.
   w = (w==0) ? 1 : w;
   h = (h==0) ? 1 : h;
   if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
      scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
      center = (Xmax+Xmin)/2;
      windowXmin = center - (center-Xmin)*scale;
      windowXmax = center + (Xmax-center)*scale;
      windowYmin = Ymin;
      windowYmax = Ymax;
   }
   else {
      scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
      center = (Ymax+Ymin)/2;
      windowYmin = center - (center-Ymin)*scale;
      windowYmax = center + (Ymax-center)*scale;
      windowXmin = Xmin;
      windowXmax = Xmax;
   }
   
   // Now that we know the max & min values for x & y 
   //      that should be visible in the window,
   //      we set up the orthographic projection.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );

}

// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv ){
   glutInit(&argc,argv);

   // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   // Window position (from top corner), and size (width% and hieght)
    glutInitWindowPosition( 10, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow( "SimpleAnim" );

   // Initialize OpenGL as we like it..
    initRendering();

   // Set up callback functions for key presses
   glutKeyboardFunc( myKeyboardFunc );         // Handles "normal" ascii symbols
   glutSpecialFunc( mySpecialKeyFunc );      // Handles "special" keyboard keys

   // Set up the callback function for resizing windows
    glutReshapeFunc( resizeWindow );

   // Call this for background processing
   // glutIdleFunc( myIdleFunction );

   // call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

   fprintf(stdout, "Arrow keys control speed.  Press \"r\" to run,  \"s\" to single step.\n");
   
   // Start the main loop.  glutMainLoop never returns.
   glutMainLoop(  );

    return(0);   // This line is never reached.
}