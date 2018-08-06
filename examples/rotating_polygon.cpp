#include <GL/glut.h>
#include <math.h>
#include <bevgrafmath2017.h>
#include <vector>


GLsizei winWidth = 800, winHeight = 800;

std::vector<vec2> objectsPoints;

GLdouble start = 0, start2 = 0;
GLint noVertex1 = 8, noVertex2 = 10;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, winWidth, 0.f, winHeight);
    
    start = 0;
}

void circle(vec2 O, GLdouble r) {
    
    glBegin(GL_LINE_LOOP);
    for (GLdouble t = start ; t <= start + 2 * pi(); t += 2 * pi() / noVertex1 )
        glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
    glEnd();
    
}

void circleWithVector(vec2 O, GLdouble r) {
    objectsPoints.clear();
    for (GLdouble t = start2; t <= start2 + 2 * pi(); t += 2 * pi() / noVertex1)
        objectsPoints.push_back(vec2(O.x + r * cos(t), O.y + r * sin(t)));
}

void circleWithVector() {
    
    circleWithVector(vec2(400, 400), 50);
    
//    glBegin(GL_LINE_STRIP);
//    for (vec2 point : objectsPoints[0]) {
//        glVertex2d(point.x, point.y);
//    }
//    glEnd();
    
    for(int i = 0; i < objectsPoints.size(); i++){
         for(int j = 0; j < objectsPoints.size(); j++){
            glBegin(GL_LINES);
                glVertex2d(objectsPoints[i].x, objectsPoints[i].y);
                glVertex2d(objectsPoints[j].x, objectsPoints[j].y);
            glEnd();
         }
    }
}


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0f, 0.4f, 0.2f);
    
    circle(vec2(400, 400), 100);
    
    circleWithVector();
    
    glutSwapBuffers();
}

void update(int n) {
    
    start += 0.1;
    
    if( start > 2 * pi()) {
        start = 0;
    }
    
    start2 -= 0.1;
    
    if( start2 <  - 2 * pi()) {
        start2 = 0;
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition(50, 100);   // Set top-left display-window position.
    glutInitWindowSize(winWidth, winHeight);      // Set display-window width and height.
    glutCreateWindow("Circle and Polygon"); // Create display window.
    
    init();                            // Execute initialization procedure.
    
    glutDisplayFunc(draw);       // Send graphics to display window.

    glutTimerFunc(5, update, 0);
    
    glutMainLoop();                    // Display everything and wait.
    return 0;
}
