/// basic cpp
/// Basic open gl program with FPS and camera and object usage
#include <GLUT/glut.h>  // For window/context creation
#include <OpenGL/gl.h>

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Camera position
float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;  // Start 5 units away looking at the origin

// Camera rotation
float camYaw = 0.0f;    // Y-axis rotation
float camPitch = 0.0f;  // X-axis rotation

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

float fps = 0.0f;
int frameCount = 0;
TimePoint lastTime = Clock::now();

static void updateFPS()
{
  frameCount++;
  auto now = Clock::now();
  std::chrono::duration<double> elapsed = now - lastTime;

  if (elapsed.count() >= 1.0)
  {
    fps = static_cast<float>(frameCount) / elapsed.count();
    frameCount = 0;
    lastTime = now;
  }
}

void renderFPS(float fps)
{
  // Prepare text
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(2) << "FPS: " << fps;
  std::string text = ss.str();

  // Save current matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 800, 0, 600);  // window size in pixels

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Optional: Disable lighting and depth test for HUD
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glColor3f(1.0f, 1.0f, 1.0f);  // white text
  glRasterPos2i(10, 570);       // near top-left (y is from bottom in OpenGL)

  for (char c : text)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
  }

  // Restore
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

// Camera helper
void updateCamera()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Calculate forward direction
  float lx = sin(camYaw);
  float lz = -cos(camYaw);

  gluLookAt(camX, camY, camZ, camX + lx, camY, camZ + lz,  // Look at point in front
            0.0f, 1.0f, 0.0f                               // Up vector
  );
}

// Key handler
void keyboard(unsigned char key, int x, int y)
{
  float moveSpeed = 0.2f;
  float turnSpeed = 0.05f;

  switch (key)
  {
    case 'w':  // Move forward
      std::cout << " Moving forward " << std::endl;
      camX += sin(camYaw) * moveSpeed;
      camZ -= cos(camYaw) * moveSpeed;
      break;
    case 's':  // Move backward
      camX -= sin(camYaw) * moveSpeed;
      camZ += cos(camYaw) * moveSpeed;
      break;
    case 'a':  // Rotate left
      camYaw -= turnSpeed;
      break;
    case 'd':  // Rotate right
      camYaw += turnSpeed;
      break;
    case 'q':  // Rotate left
      camY -= turnSpeed;
      break;
    case 'e':  // Rotate right
      camY += turnSpeed;
      break;
    case 27:  // Escape key
      exit(0);
      break;
  }

  glutPostRedisplay();  // Request re-render
}

void Normalize(float p[3])
{
  float length = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
  if (length != 0.0f)
  {
    p[0] /= length;
    p[1] /= length;
    p[2] /= length;
  }
}

void CalcNormal(float p[3], float p1[3], float p2[3], float n[3])
{
  float pa[3] = {p1[0] - p[0], p1[1] - p[1], p1[2] - p[2]};
  float pb[3] = {p2[0] - p[0], p2[1] - p[1], p2[2] - p[2]};

  n[0] = pa[1] * pb[2] - pa[2] * pb[1];
  n[1] = pa[2] * pb[0] - pa[0] * pb[2];
  n[2] = pa[0] * pb[1] - pa[1] * pb[0];

  Normalize(n);
}

static void renderCube(void)
{
  float v[3][3];
  float n[3];
  float size = 1.0f;

  glBegin(GL_TRIANGLES);

  // Define all the cube faces
  // Front face
  float front[4][3] = {
      {-size, -size, size}, {size, -size, size}, {size, size, size}, {-size, size, size}};

  // Back face
  float back[4][3] = {
      {-size, -size, -size}, {size, -size, -size}, {size, size, -size}, {-size, size, -size}};

  // Draw front
  CalcNormal(front[0], front[1], front[2], n);
  glNormal3fv(n);
  glVertex3fv(front[0]);
  glVertex3fv(front[1]);
  glVertex3fv(front[2]);

  CalcNormal(front[0], front[2], front[3], n);
  glNormal3fv(n);
  glVertex3fv(front[0]);
  glVertex3fv(front[2]);
  glVertex3fv(front[3]);

  // Draw back
  CalcNormal(back[2], back[1], back[0], n);
  glNormal3fv(n);
  glVertex3fv(back[2]);
  glVertex3fv(back[1]);
  glVertex3fv(back[0]);

  CalcNormal(back[3], back[2], back[0], n);
  glNormal3fv(n);
  glVertex3fv(back[3]);
  glVertex3fv(back[2]);
  glVertex3fv(back[0]);

  // Draw left
  CalcNormal(back[0], front[0], front[3], n);
  glNormal3fv(n);
  glVertex3fv(back[0]);
  glVertex3fv(front[0]);
  glVertex3fv(front[3]);

  CalcNormal(back[0], front[3], back[3], n);
  glNormal3fv(n);
  glVertex3fv(back[0]);
  glVertex3fv(front[3]);
  glVertex3fv(back[3]);

  // Draw right
  CalcNormal(front[1], back[1], back[2], n);
  glNormal3fv(n);
  glVertex3fv(front[1]);
  glVertex3fv(back[1]);
  glVertex3fv(back[2]);

  CalcNormal(front[1], back[2], front[2], n);
  glNormal3fv(n);
  glVertex3fv(front[1]);
  glVertex3fv(back[2]);
  glVertex3fv(front[2]);

  // Draw top
  CalcNormal(front[3], front[2], back[2], n);
  glNormal3fv(n);
  glVertex3fv(front[3]);
  glVertex3fv(front[2]);
  glVertex3fv(back[2]);

  CalcNormal(front[3], back[2], back[3], n);
  glNormal3fv(n);
  glVertex3fv(front[3]);
  glVertex3fv(back[2]);
  glVertex3fv(back[3]);

  // Draw bottom
  CalcNormal(back[0], back[1], front[1], n);
  glNormal3fv(n);
  glVertex3fv(back[0]);
  glVertex3fv(back[1]);
  glVertex3fv(front[1]);

  CalcNormal(back[0], front[1], front[0], n);
  glNormal3fv(n);
  glVertex3fv(back[0]);
  glVertex3fv(front[1]);
  glVertex3fv(front[0]);

  glEnd();
}

/**
 * Main display routine, render scene
 */
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // Set material properties
  GLfloat mat_diffuse[] = {0.0f, 0.0f, 1.0f, 1.0f};   // Pure blue diffuse color
  GLfloat mat_specular[] = {0.2f, 0.2f, 1.0f, 1.0f};  // White highlights
  GLfloat mat_shininess[] = {70.0f};                  // High shininess

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

  updateCamera();  // Set camera

  renderCube();

  // Flip frame
  updateFPS();

  static GLfloat white[4] = {0.8, 0.8, 0.8, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  renderFPS(fps);
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  if (h == 0)
  {
    h = 1;
  }
  float ratio = (float)w / (float)h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

static void idle() { glutPostRedisplay(); }

int main(int argc, char** argv)
{
  std::cout << "Hello from C++ on macOS with clang!" << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Basic OpenGL on macOS");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);

  // Setup scnee
  static GLfloat pos[4] = {5.0, 5.0, 20.0, 0.0};
  GLint i;

  GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

  // Setup lighting parameters
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);

  // Continue with camera setup

  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(2.0, -5.0, -15.0);
  */

  // Continue display loop
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}