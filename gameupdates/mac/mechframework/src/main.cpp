#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <bot.hpp>
#include <objects.hpp>
#include <lights.hpp>
#include <gldrawlib.hpp>
#include <globals.hpp>

void RenderBot(DriverBotPtr boid);
DriverBotPtr CreateBot(int bot_id);

namespace {

struct Star {
  float x;
  float y;
  float z;
  float size;
  float brightness;
};

int gWindowWidth = 1024;
int gWindowHeight = 768;

static constexpr int kSpecialKeyBase = 256;
bool gKeys[512] = {false};

float gPitchOffset = 0.0f;
float gFovOffset = 0.0f;
float gCamYawDeg = 0.0f;
float gCamDistance = 120.0f;
bool gMouseDrag = false;
int gPrevMouseX = 0;
int gPrevMouseY = 0;

std::vector<Star> gStars;
DriverBotPtr gPlayerBot = nullptr;

float ClampFloat(float value, float minv, float maxv) {
  if (value < minv) return minv;
  if (value > maxv) return maxv;
  return value;
}

void InitStars() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  gStars.clear();
  gStars.reserve(400);

  for (int i = 0; i < 400; ++i) {
    const float theta = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * static_cast<float>(M_PI);
    const float phi = static_cast<float>(std::rand()) / RAND_MAX * static_cast<float>(M_PI) * 0.5f;
    const float radius = 460.0f + static_cast<float>(std::rand()) / RAND_MAX * 260.0f;

    Star s{};
    s.x = radius * std::sin(phi) * std::cos(theta);
    s.y = std::fabs(radius * std::cos(phi));
    s.z = radius * std::sin(phi) * std::sin(theta);
    s.size = 1.0f + static_cast<float>(std::rand()) / RAND_MAX * 2.4f;
    s.brightness = 0.5f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f;
    gStars.push_back(s);
  }
}

void DrawGround() {
  // Draw a lit base floor first so light/material response is visible.
  RenderPlane();
  // Keep the classic hex grid as an overlay guide.
  RenderGrid();
}

void DrawStars() {
  glDisable(GL_LIGHTING);
  glPointSize(2.0f);

  glBegin(GL_POINTS);
  for (const Star& s : gStars) {
    glColor3f(s.brightness, s.brightness, s.brightness * 0.95f);
    glVertex3f(s.x, s.y, s.z);
  }
  glEnd();
}

void InitPlayerBot() {
  gPlayerBot = CreateBot(0);
  gPlayerBot->x = 0.0f;
  gPlayerBot->y = 0.0f;
  gPlayerBot->heading = 0.0f;
  gPlayerBot->target_dir = 0.0f;
  gPlayerBot->linearv = 0.0f;
  gPlayerBot->alive = ALIVE_STATE;
  gPlayerBot->size[0] = 1.3f;
  gPlayerBot->size[1] = 1.3f;
  gPlayerBot->size[2] = 1.3f;
  gPlayerBot->color[0] = 0.78f;
  gPlayerBot->color[1] = 0.82f;
  gPlayerBot->color[2] = 0.92f;
}

void HandlePlayerBotKeys() {
  if (!gPlayerBot || gPlayerBot->alive == DEAD_STATE) {
    return;
  } 

  const float turnSpeed = 1.0f;
  const float moveSpeed = 0.15f;

  if (gKeys['a'] || gKeys['A'] || gKeys[kSpecialKeyBase + GLUT_KEY_LEFT]) {
    gPlayerBot->heading -= turnSpeed;
  }
  if (gKeys['d'] || gKeys['D'] || gKeys[kSpecialKeyBase + GLUT_KEY_RIGHT]) {
    gPlayerBot->heading += turnSpeed;
  }
  if (gPlayerBot->heading < 0.0f) gPlayerBot->heading += 360.0f;
  if (gPlayerBot->heading >= 360.0f) gPlayerBot->heading -= 360.0f;

  if (gKeys['w'] || gKeys['W'] || gKeys[kSpecialKeyBase + GLUT_KEY_UP]) {
    gPlayerBot->x -= std::sin(gPlayerBot->heading * (static_cast<float>(M_PI) / 180.0f)) * moveSpeed;
    gPlayerBot->y -= std::cos(gPlayerBot->heading * (static_cast<float>(M_PI) / 180.0f)) * moveSpeed;
  }
  if (gKeys['s'] || gKeys['S'] || gKeys[kSpecialKeyBase + GLUT_KEY_DOWN]) {
    gPlayerBot->x += std::sin(gPlayerBot->heading * (static_cast<float>(M_PI) / 180.0f)) * moveSpeed;
    gPlayerBot->y += std::cos(gPlayerBot->heading * (static_cast<float>(M_PI) / 180.0f)) * moveSpeed;
  }
}

void DrawPlayerCharacter() {
  if (!gPlayerBot) { 
    return;
  }

  RenderBot(gPlayerBot);
}

void DrawHUD() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, gWindowWidth, gWindowHeight, 0.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glColor3ub(0, 255, 0);

  const int x = 10;
  const int y0 = 20;
  const int dy = 14;

  if (gPlayerBot) {
    glRasterPos2i(x, y0);
    PrintText("POS:  X=%.1f  Y=%.1f", gPlayerBot->x, gPlayerBot->y);

    glRasterPos2i(x, y0 + dy);
    PrintText("HDG:  %.1f deg", gPlayerBot->heading);
  }

  glRasterPos2i(x, y0 + dy * 2);
  PrintText("CAM:  dist=%.0f  yaw=%.1f  pitch=%.1f", gCamDistance, gCamYawDeg, gPitchOffset);

  glRasterPos2i(x, y0 + dy * 4);
  PrintText("WASD/Arrows: move bot   Mouse-drag: camera   +/-: zoom   R: reset");

  glPopAttrib();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void HandleCameraKeys() {
  if (gKeys['a'] || gKeys['A']) {
    gCamYawDeg -= 1.8f;
  }
  if (gKeys['d'] || gKeys['D']) {
    gCamYawDeg += 1.8f;
  }
  if (gKeys['i'] || gKeys['I']) {
    gFovOffset -= 0.45f;
  }
  if (gKeys['k'] || gKeys['K']) {
    gFovOffset += 0.45f;
  }

  if (gKeys['o'] || gKeys['O']) {
    gPitchOffset += 0.9f;
  }

  if (gKeys['l'] || gKeys['L']) {
    gPitchOffset -= 0.9f;
  }

  gPitchOffset = ClampFloat(gPitchOffset, -140.0f, 220.0f);
  gFovOffset = ClampFloat(gFovOffset, -20.0f, 30.0f);
}

void DisplayGL() {
  HandleCameraKeys();
  HandlePlayerBotKeys();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.09f, 1.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  const float fov = ClampFloat(45.0f + gFovOffset, 20.0f, 90.0f);
  gluPerspective(fov, static_cast<GLfloat>(gWindowWidth) / static_cast<GLfloat>(gWindowHeight), 0.1f, 2000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const float yawRad = gCamYawDeg * (static_cast<float>(M_PI) / 180.0f);
  const float camX = std::sin(yawRad) * gCamDistance;
  const float camZ = std::cos(yawRad) * gCamDistance;

  gluLookAt(camX, 70.0f + gPitchOffset, camZ,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

  // Update light positions in world space after camera/view transform.
  SetLights();

  DrawStars();
  glEnable(GL_LIGHTING);
  DrawGround();
  DrawPlayerCharacter();
  DrawHUD();

  glutSwapBuffers();
}

void ResizeGL(int width, int height) {
  if (height <= 0) height = 1;
  gWindowWidth = width;
  gWindowHeight = height;
  glViewport(0, 0, width, height);
}

void KeyDown(unsigned char key, int, int) {
  gKeys[static_cast<unsigned char>(key)] = true;

  switch (key) {
    case '=':
    case '+':
      gCamDistance = ClampFloat(gCamDistance - 8.0f, 20.0f, 360.0f);
      break;
    case '-':
    case '_':
      gCamDistance = ClampFloat(gCamDistance + 8.0f, 20.0f, 360.0f);
      break;
    case 'r':
    case 'R':
      gCamYawDeg = 0.0f;
      gPitchOffset = 0.0f;
      gFovOffset = 0.0f;
      gCamDistance = 120.0f;
      break;
    case 'q':
    case 'Q':
    case 27:
      std::exit(0);
      break;
    default:
      break;
  }
}

void KeyUp(unsigned char key, int, int) {
  gKeys[static_cast<unsigned char>(key)] = false;
}

void SpecialKeyDown(int key, int, int) {
  gKeys[kSpecialKeyBase + key] = true;
}

void SpecialKeyUp(int key, int, int) {
  gKeys[kSpecialKeyBase + key] = false;
}

void MouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    gMouseDrag = (state == GLUT_DOWN);
    gPrevMouseX = x;
    gPrevMouseY = y;
    return;
  }

  if (state == GLUT_DOWN && (button == 3 || button == 4)) {
    const float delta = (button == 3) ? -6.0f : 6.0f;
    gCamDistance = ClampFloat(gCamDistance + delta, 20.0f, 360.0f);
  }
}

void MouseMove(int x, int y) {
  if (!gMouseDrag) return;

  const int dx = x - gPrevMouseX;
  const int dy = y - gPrevMouseY;

  gPrevMouseX = x;
  gPrevMouseY = y;

  gCamYawDeg += dx * 0.35f;
  gPitchOffset -= dy * 0.45f;
  gPitchOffset = ClampFloat(gPitchOffset, -140.0f, 220.0f);
}

void IdleGL() {
  glutPostRedisplay();
}

}  // namespace

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(gWindowWidth, gWindowHeight);
  glutCreateWindow("Mech Framework");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // Initialize lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  Super_LoadGlobals();

  InitGlobals();
  InitMaterial();
  GenerateLights();
  SetLights();
  InitObjects();
  InitPlayerBot();

  InitStars();

  glutDisplayFunc(DisplayGL);
  glutReshapeFunc(ResizeGL);
  glutKeyboardFunc(KeyDown);
  glutKeyboardUpFunc(KeyUp);
  glutSpecialFunc(SpecialKeyDown);
  glutSpecialUpFunc(SpecialKeyUp);
  glutMouseFunc(MouseButton);
  glutPassiveMotionFunc(MouseMove);
  glutMotionFunc(MouseMove);
  glutIdleFunc(IdleGL);

  glutMainLoop();
  return 0;
}
