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

#ifndef M_PI
  #define M_PI 3.14159265359
#endif

// Egyszerű 2D vektor osztály
struct Vector {
  float x, y;
  Vector(float v = 0.0f) : x(v), y(v) {}
  Vector(float x, float y) : x(x), y(y) {}
  void operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
  }
  Vector& operator=(const Vector& v) {
    x = v.x;
    y = v.y;
    return *this;
  }
  Vector operator*(const Vector& v) const {
    return Vector(x*v.x, y*v.y);
  }
} ball_pos, ball_speed(-0.46f, 1.13f); // Két globális változó, a labda jelenlegi állapotát tárolják.

struct Color {
  float r, g, b;
  Color(float r, float g, float b) : r(r), g(g), b(b) { }
};

#define CIRCLE_RESOLUTION 16
const float ball_radius = 0.1f;

// A lasztit kirajzoló függvény
void drawBall(const Vector pos, const Color& color) {
  glBegin(GL_TRIANGLE_FAN); {
   
    glColor3f(color.r, color.g, color.b);
    glVertex2f(pos.x, pos.y);
   
    for(int i = 0; i <= CIRCLE_RESOLUTION; i++) {
      float angle = float(i) / CIRCLE_RESOLUTION * 2.0f * M_PI;
      glVertex2f(pos.x + ball_radius*cos(angle), pos.y + ball_radius*sin(angle));
    }
  } glEnd();
}

const int memory_size = 200;

// Az elmúlt 200 frameben a labda helyzeteit tároló osztály. Emiatt húz csíkot.
// Ez lényegében egy fixen 200 elemű várakozási sor, az egyszerűség kedvéért egy
// tömbbel megvalósítva.
struct BallTrail {
  Vector positions[memory_size];
  int current_top;

  BallTrail() : current_top(0) { }
  void addPosition(const Vector& v) {
    current_top = (current_top + 1) % memory_size;
    positions[current_top] = v;
  }
  void draw() {
    // Rajzoljuk ki a labdát, és az összes előzményét
    for(int i = 1; i <= memory_size; i++) { 
      drawBall(
        positions[(current_top + i) % memory_size], 
        Color( // A színe függjön a labda korától
          0.0f, i == memory_size ? 0.1f : 0.1f*i/memory_size, 
          i == memory_size ? 0.8f : 0.2f*i/memory_size
        )
      );
    }
  }
} balltrail;

void onDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);

  balltrail.addPosition(ball_pos); // Hozzáadjuk a jelenlegi helyzetet a memóriához.
  balltrail.draw(); // És kirajzoljuk az összeset.

  glutSwapBuffers();
}

// std::min helyett
template<typename T>
T min(const T& a, const T& b) {
  if(a < b) {
    return a;
  } else {
    return b;
  }
}

void onIdle() {
  static int last_time = glutGet(GLUT_ELAPSED_TIME); // Visszaadja a jelenlegi időt miliszekundumban
  int curr_time = glutGet(GLUT_ELAPSED_TIME);
  int diff = curr_time - last_time; // Az előző onIdle óta eltelt idő
  last_time = curr_time; // A következő meghíváskor az előző idő majd a mostani idő lesz.

  // Két onIdle között eltelt idő nagyon változó tud lenni, és akár elég nagy is lehet
  // ahhoz, hogy a labda látványosan bele menjen a falba, mielőtt visszapattan. Ezért
  // osszuk fel az elelt időt kisebb részekre, pl. max 5 miliszekundumos egységekre,
  // és ilyen időközönként nézzük meg, hogy a labda ütközött-e a fallal.
  const int time_step = 5;
  for(int i = 0; i < diff; i += time_step) {
    // Az időosztás végén egy kisebb egység marad, mint az idő egység. Pl. ha a diff 11,
    // akkor azt akarjuk, hogy 5, 5, 1 egységekre bontsuk azt, ne 5, 5, 5-re. És utána 
    // számoljuk át a másodperce. 
    float dt = min(diff-i, time_step) / 1000.0f;

    // Módosítsuk a sebességet ha ütközött a fallal, teljesen rugalmas ütközést feltételezve.
    // Ilyenkor a labda a fal irányára merőlegesen pontosan ellentétes irányba halad tovább.
    // Hogy ne legyen olyan unalmas a pattogás, adjuk a sebességéhez egy kis random számot, 
    // hogy pár fokkal meg is változzon a visszapattanás szöge.
    if(ball_pos.x + ball_radius > 1) {
      ball_speed.x = -fabs(ball_speed.x) + (0.1f - 0.2f*rand()/RAND_MAX);
    } else if(ball_pos.x - ball_radius < -1) {
      ball_speed.x = fabs(ball_speed.x) + (0.1f - 0.2f*rand()/RAND_MAX);
    }
    if(ball_pos.y + ball_radius > 1) {
      ball_speed.y = -fabs(ball_speed.y) + (0.1f - 0.2f*rand()/RAND_MAX);
    } else if(ball_pos.y - ball_radius < -1) {
      ball_speed.y = fabs(ball_speed.y) + (0.1f - 0.2f*rand()/RAND_MAX);
    }

    // Mozgassuk a labdát a ds = v * dt képlet alapján.
    ball_pos += ball_speed * dt;
  }

  glutPostRedisplay(); // Megváltozott a jelenet, újra kell rajzolni
}

void onInitialization() {}

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