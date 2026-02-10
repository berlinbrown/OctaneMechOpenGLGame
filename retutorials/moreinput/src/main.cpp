/**
 * Copyright (c) 2006-2026 Berlin Brown.  All Rights Reserved
 */

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <cstdio>


static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;
static const float PERSPECTIVE_Z = 600.0f;

static int gWindowWidth = SCREEN_WIDTH;
static int gWindowHeight = SCREEN_HEIGHT;
static bool gGameReady = false;


static const int kFrameRateSamples = 60;
static const int kTargetFrameMs = 16;

static float framerate = 0.0f;
static int keyCodes[20] = {0};
static bool gKeys[256] = {false};

struct AntGlobals
{
  int paused;
};

static AntGlobals gAntGlobals = {0};
static AntGlobals* ant_globals = &gAntGlobals;

static GLUquadric* quadric = nullptr;

static void CheckRespawn(void) {}
static void AddTime(float) {}
static void AddSeconds(float) {}
static void GameTick(void) {}
static void AnimFireAnts(void) {}
static void AnimateExplosions(void) {}
static void HandleCameraKeys(bool*) {}
static void IdleGL(void)
{
  glutPostRedisplay();
}

static void OnTimer(int value)
{
  (void)value;
  glutPostRedisplay();
  glutTimerFunc(kTargetFrameMs, OnTimer, 0);
}

static int gFrames = 0;
static clock_t gLastTime = 0;
static clock_t gLastUpdateTime = 0;

static float gBoxPosX = 0.0f;
static float gBoxPosY = 0.0f;
static float gBoxPosZ = 0.0f;
static float gBoxSpinAngle = 0.0f;

static bool gMoveUp = false;
static bool gMoveDown = false;

static float gCameraYaw = 5.7106f;
static float gCameraPitch = 26.5651f;
static float gCameraDistance = 22.4499f;

static bool gIgnoreNextMouseMotion = false;
static bool gMouseCaptured = true;

static const float kDegToRad = 0.01745329252f;
static const float kMouseSensitivity = 0.05f;
static const float kBoxSpinSpeed = 20.0f;
static const float kBoxMoveSpeed = 5.0f;
static const float kBoxStepMove = 0.5f;
static const float kMinCameraDistance = 5.0f;
static const float kMaxCameraDistance = 80.0f;

static void UpdateCameraOrbit(float deltaYawDeg, float deltaPitchDeg)
{
  gCameraYaw += deltaYawDeg;
  gCameraPitch += deltaPitchDeg;

  if (gCameraPitch > 89.0f) gCameraPitch = 89.0f;
  if (gCameraPitch < -89.0f) gCameraPitch = -89.0f;
}

static void UpdateCameraDistance(float delta)
{
  gCameraDistance += delta;
  if (gCameraDistance < kMinCameraDistance) gCameraDistance = kMinCameraDistance;
  if (gCameraDistance > kMaxCameraDistance) gCameraDistance = kMaxCameraDistance;
}


static void GetFramesPerSecond(void)
{
  clock_t now;
  float delta;

  GameTick();
  CheckRespawn();

  if (gFrames++ >= kFrameRateSamples)
  {
    now = clock();
    delta = (now - gLastTime) / (float)CLOCKS_PER_SEC;
    AddTime((float)(now - gLastTime));
    AddSeconds(delta);
    gLastTime = now;
    framerate = kFrameRateSamples / delta;
    gFrames = 0;
  }
}

static void DrawHUD(void)
{
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);

  GetFramesPerSecond();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

static void InitGame(void)
{
  srand((unsigned int)time(NULL));
  gLastTime = clock();

  
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);

  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat lightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat lightDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat lightSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

  quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);
  gluQuadricTexture(quadric, GL_TRUE);


  // Render spinning box

  gGameReady = true;
}

static void AnimateScene(void)
{
  clock_t now = clock();
  if (gLastUpdateTime == 0)
  {
    gLastUpdateTime = now;
  }
  float delta = (now - gLastUpdateTime) / (float)CLOCKS_PER_SEC;
  gLastUpdateTime = now;
  if (delta > 0.05f)
  {
    delta = 0.05f;
  }

  if (gMoveUp)
  {
    gBoxPosY += kBoxMoveSpeed * delta;
  }
  if (gMoveDown)
  {
    gBoxPosY -= kBoxMoveSpeed * delta;
  }

  std::printf("[Box] gBoxPosY=%.2f\n", gBoxPosY);

  gBoxSpinAngle += kBoxSpinSpeed * delta;
  if (gBoxSpinAngle >= 360.0f) gBoxSpinAngle -= 360.0f;

  if (ant_globals->paused == 0)
  {
    AnimFireAnts();
  }

  AnimateExplosions();
}

static void RenderText(float x, float y, const char* text)
{
  glRasterPos2f(x, y);
  while (*text)
  {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text);
    ++text;
  }
}

static void DrawHUDText(void)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, gWindowWidth, 0.0, gWindowHeight);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glColor3f(1.0f, 1.0f, 1.0f);

  RenderText(12.0f, 16.0f, "HUD: Arrow Up/Down move box | Mouse look | Scroll zoom");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

static void OnSpecialKeyDown(int key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == GLUT_KEY_UP)
  {
    gMoveUp = true;
    gBoxPosY += kBoxStepMove;
  }
  else if (key == GLUT_KEY_DOWN)
  {
    gMoveDown = true;
    gBoxPosY -= kBoxStepMove;
  }
  std::printf("[KeyDown] key=%d moveUp=%d moveDown=%d\n", key, gMoveUp ? 1 : 0, gMoveDown ? 1 : 0);
  glutPostRedisplay();
}

static void OnSpecialKeyUp(int key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == GLUT_KEY_UP)
  {
    gMoveUp = false;
    gBoxPosY -= kBoxStepMove;
  }
  else if (key == GLUT_KEY_DOWN)
  {
    gMoveDown = false;
    gBoxPosY += kBoxStepMove;
  }
  std::printf("[KeyUp] key=%d moveUp=%d moveDown=%d\n", key, gMoveUp ? 1 : 0, gMoveDown ? 1 : 0);
  glutPostRedisplay();
}

static void OnMouseButton(int button, int state, int x, int y)
{
  (void)x;
  (void)y;
  if (state == GLUT_DOWN)
  {
    if (button == 3)
    {
      UpdateCameraDistance(-1.5f);
      glutPostRedisplay();
      return;
    }
    if (button == 4)
    {
      UpdateCameraDistance(1.5f);
      glutPostRedisplay();
      return;
    }
  }
}

static void OnMouseMove(int x, int y)
{
  if (!gMouseCaptured)
  {
    return;
  }
  if (gIgnoreNextMouseMotion)
  {
    gIgnoreNextMouseMotion = false;
    return;
  }

  int centerX = gWindowWidth / 2;
  int centerY = gWindowHeight / 2;

  int dx = x - centerX;
  int dy = y - centerY;

  UpdateCameraOrbit(dx * kMouseSensitivity, -dy * kMouseSensitivity);

  std::printf("[MouseMove] x=%d y=%d dx=%d dy=%d yaw=%.2f pitch=%.2f dist=%.2f\n",
              x, y, dx, dy, gCameraYaw, gCameraPitch, gCameraDistance);

  gIgnoreNextMouseMotion = true;
  glutWarpPointer(centerX, centerY);
  glutPostRedisplay();
}

static void OnKeyDown(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == 'w' || key == 'W')
  {
    gMoveUp = true;
    gBoxPosY += kBoxStepMove;
  }
  else if (key == 's' || key == 'S')
  {
    gMoveDown = true;
    gBoxPosY -= kBoxStepMove;
  }
  std::printf("[KeyDown] key=%d moveUp=%d moveDown=%d\n", key, gMoveUp ? 1 : 0, gMoveDown ? 1 : 0);
  glutPostRedisplay();
}

static void OnKeyUp(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
  if (key == 'w' || key == 'W')
  {
    gMoveUp = false;
    gBoxPosY -= kBoxStepMove;
  }
  else if (key == 's' || key == 'S')
  {
    gMoveDown = false;
    gBoxPosY += kBoxStepMove;
  }
  std::printf("[KeyUp] key=%d moveUp=%d moveDown=%d\n", key, gMoveUp ? 1 : 0, gMoveDown ? 1 : 0);
  glutPostRedisplay();
}

static void DrawGLScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float yawRad = gCameraYaw * kDegToRad;
  float pitchRad = gCameraPitch * kDegToRad;
  float cosPitch = cosf(pitchRad);
  float sinPitch = sinf(pitchRad);
  float cosYaw = cosf(yawRad);
  float sinYaw = sinf(yawRad);

  float targetX = 0.0f;
  float targetY = 0.0f;
  float targetZ = 0.0f;

  float camX = targetX + gCameraDistance * cosPitch * sinYaw;
  float camY = targetY + gCameraDistance * sinPitch;
  float camZ = targetZ + gCameraDistance * cosPitch * cosYaw;

  gluLookAt(camX, camY, camZ,
            targetX, targetY, targetZ,
            0.0f, 1.0f, 0.0f);

  GLfloat lightPos[] = {camX + 5.0f, camY + 10.0f, camZ + 5.0f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  GLfloat matSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0f);
  glColor3f(0.1f, 0.2f, 0.9f);

  glPushMatrix();
  glTranslatef(0.0f, -2.2f, 0.0f);
  glScalef(1.4f, 0.12f, 1.4f);
  glColor3f(0.4f, 0.4f, 0.4f);
  glutSolidCube(4.0f);
  glPopMatrix();

  glColor3f(0.1f, 0.2f, 0.9f);
  glPushMatrix();
  glTranslatef(gBoxPosX, gBoxPosY, gBoxPosZ);
  glRotatef(gBoxSpinAngle, 0.0f, 1.0f, 0.0f);
  glutSolidCube(4.0f);
  glPopMatrix();

  DrawHUDText();
}

static void DisplayGL(void)
{
  if (!gGameReady)
  {
    InitGame();
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)gWindowWidth / (GLfloat)gWindowHeight, 0.1f, PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  HandleCameraKeys(gKeys);
  AnimateScene();
  DrawGLScene();

  glutSwapBuffers();
}

static void ResizeGL(int width, int height)
{
  if (height <= 0) height = 1;

  gWindowWidth = width;
  gWindowHeight = height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(gWindowWidth, gWindowHeight);
  glutCreateWindow("UmbraMech");

  glutDisplayFunc(DisplayGL);
  glutReshapeFunc(ResizeGL);

  glutIdleFunc(nullptr);

  glutSpecialFunc(OnSpecialKeyDown);
  glutSpecialUpFunc(OnSpecialKeyUp);
  glutKeyboardFunc(OnKeyDown);
  glutKeyboardUpFunc(OnKeyUp);
  glutMouseFunc(OnMouseButton);
  glutPassiveMotionFunc(OnMouseMove);
  glutMotionFunc(OnMouseMove);
  glutSetCursor(GLUT_CURSOR_NONE);

  if (gMouseCaptured)
  {
    gIgnoreNextMouseMotion = true;
    glutWarpPointer(gWindowWidth / 2, gWindowHeight / 2);
  }

  glutTimerFunc(kTargetFrameMs, OnTimer, 0);

  glutMainLoop();

  return 0;
}