// ShinyBlueCube.cpp
// A basic GLUT+OpenGL demo: shiny blue cube + FPS + camera controls
// Best model with blue cube and lighting

#include <GLUT/glut.h>   // macOS: includes both OpenGL and GLUT
#include <OpenGL/glu.h>  // for gluLookAt, gluPerspective

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Camera state
static float camX = 2.0f, camY = 5.0f, camZ = 15.0f;
static float camYaw = 0.0f;

// FPS timing
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
static float fps = 0.0f;
static int frameCount = 0;
static TimePoint lastTime = Clock::now();

static void updateFPS() {
  frameCount++;
  auto now = Clock::now();
  auto elapsed = std::chrono::duration<double>(now - lastTime).count();
  if (elapsed >= 1.0) {
    fps = frameCount / elapsed;
    frameCount = 0;
    lastTime = now;
  }
}

// Draws the FPS counter in screen-space
static void renderFPS() {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(1) << "FPS: " << fps;
  std::string text = ss.str();

  // Set up 2D orthographic projection for HUD
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 800, 0, 600);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glColor3f(1, 1, 1);
  glRasterPos2i(10, 580);
  for (char c : text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

// Normalize a vector
static void Normalize(float p[3]) {
  float L = std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
  if (L > 0.0001f) {
    p[0] /= L;
    p[1] /= L;
    p[2] /= L;
  }
}

// Compute normal for triangle (p, p1, p2)
static void CalcNormal(const float p[3], const float p1[3], 
              const float p2[3],float n[3]) {

  float pa[3] = {p1[0] - p[0], p1[1] - p[1], p1[2] - p[2]};
  float pb[3] = {p2[0] - p[0], p2[1] - p[1], p2[2] - p[2]};

  n[0] = pa[1] * pb[2] - pa[2] * pb[1];
  n[1] = pa[2] * pb[0] - pa[0] * pb[2];
  n[2] = pa[0] * pb[1] - pa[1] * pb[0];
  Normalize(n);
}

// Draws a unit cube centered at origin, using GL_TRIANGLES
static void renderCube() {
  // We'll supply material once in display(), so no glColor calls here.
  const float S = 1.0f;
  float n[3];
  // 8 corners
  float vtx[8][3] = {{-S, -S, S},  {S, -S, S},  {S, S, S},  {-S, S, S},
                     {-S, -S, -S}, {S, -S, -S}, {S, S, -S}, {-S, S, -S}};
  // indices for 12 triangles (2 per face)
  int faces[12][3] = {
      {0, 1, 2}, {0, 2, 3},  // front
      {5, 4, 7}, {5, 7, 6},  // back
      {3, 2, 6}, {3, 6, 7},  // top
      {4, 5, 1}, {4, 1, 0},  // bottom
      {1, 5, 6}, {1, 6, 2},  // right
      {4, 0, 3}, {4, 3, 7}   // left
  };

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < 12; ++i) {
    CalcNormal(vtx[faces[i][0]], vtx[faces[i][1]], vtx[faces[i][2]], n);
    glNormal3fv(n);
    glVertex3fv(vtx[faces[i][0]]);
    glVertex3fv(vtx[faces[i][1]]);
    glVertex3fv(vtx[faces[i][2]]);
  }
  glEnd();
}

// Position camera via gluLookAt
static void updateCamera() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  float lx = std::sin(camYaw), lz = -std::cos(camYaw);
  gluLookAt(camX, camY, camZ, camX + lx, camY, camZ + lz, 0, 1, 0);
}

// WASD + QE + Esc
static void keyboard(unsigned char key, int, int) {
  const float moveSpeed = 0.2f;
  const float turnSpeed = 0.05f;
  switch (key) {
    case 'w':
      camX += std::sin(camYaw) * moveSpeed;
      camZ -= std::cos(camYaw) * moveSpeed;
      break;
    case 's':
      camX -= std::sin(camYaw) * moveSpeed;
      camZ += std::cos(camYaw) * moveSpeed;
      break;
    case 'a':
      camYaw -= turnSpeed;
      break;
    case 'd':
      camYaw += turnSpeed;
      break;
    case 'q':
      camY -= moveSpeed;
      break;
    case 'e':
      camY += moveSpeed;
      break;
    case 27:
      exit(0);
      break;
  }
  glutPostRedisplay();
}

static void draw_wirebox(void) {
  float size = 4.4f;

  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);

  // Front Face
  glVertex3f(-size, 0.0f, size);  // left bottom
  glVertex3f(size, 0.0f, size);   // right bottom
  glVertex3f(size, size, size);   // top right
  glVertex3f(-size, size, size);  // top left
  // Back Face

  glVertex3f(-size, 0.0f, -size);
  glVertex3f(-size, size, -size);
  glVertex3f(size, size, -size);
  glVertex3f(size, 0.0f, -size);

  // Top Face
  glVertex3f(-size, size, -size);
  glVertex3f(-size, size, size);
  glVertex3f(size, size, size);
  glVertex3f(size, size, -size);

  // Bottom Face
  glVertex3f(-size, 0.0f, -size);
  glVertex3f(size, 0.0f, -size);
  glVertex3f(size, 0.0f, size);
  glVertex3f(-size, 0.0f, size);

  // Right face
  glVertex3f(size, 0.0f, -size);
  glVertex3f(size, size, -size);
  glVertex3f(size, size, size);
  glVertex3f(size, 0.0f, size);

  // Left Face
  glVertex3f(-size, 0.0f, -size);
  glVertex3f(-size, 0.0f, size);
  glVertex3f(-size, size, size);
  glVertex3f(-size, size, -size);
  glEnd();
}



/**
 * Main render display routine
 */
static void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 1) Position camera
  updateCamera();

  // 2) Shiny blue material
  GLfloat mat_AD[] = {0, 0, 1, 1};  // ambient + diffuse = pure blue
  GLfloat mat_SP[] = {1, 1, 1, 1};  // white specular
  GLfloat mat_SH[] = {80.0f};       // shininess
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_AD);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_SP);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_SH);

  // 3) Draw cube
  renderCube();
  draw_wirebox();

  // 4) FPS overlay
  updateFPS();
  renderFPS();

  glutSwapBuffers();
}

static void reshape(int w, int h) {
  if (h == 0) {
    h = 1;
  }
  float ar = (float)w / h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, ar, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

static void idle() { 
  glutPostRedisplay(); 
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Shiny Blue Cube with Camera & FPS");

  // Clear color & depth
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);

  // Position light #0
  GLfloat Lpos[] = {5, 5, 5, 1};
  GLfloat Lamb[] = {0.2f, 0.2f, 0.2f, 1};
  GLfloat Ldiff[] = {0.8f, 0.8f, 0.8f, 1};
  GLfloat Lspec[] = {1.0f, 1.0f, 1.0f, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, Lpos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, Lamb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Ldiff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Lspec);

  // Callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  glutMainLoop();
  return 0;
}
