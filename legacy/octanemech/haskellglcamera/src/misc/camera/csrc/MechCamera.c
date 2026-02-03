//
// Simple OpenGL camera demo in C
//
// Berlin Brown
//

#include <GL/gl.h>    // Header File For The OpenGL32 Library
#include <GL/glu.h>   // Header File For The GLu32 Library
#include <GL/glut.h>  // Header File For The GLUT Library
#include <time.h>
#include <unistd.h>  // needed to sleep

int screenWidth = 640;
int screenHeight = 480;
float angle = 0.0f;

float posx = 0.0f;
float posy = 3.2f;
float posz = -4.5;

// ASCII code for the escape key.
#define ESCAPE 27

// The number of our GLUT window
int window;

void initGL(void)
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)screenWidth / screenHeight, 1.0f, 100.0f);
  glMatrixMode(GL_MODELVIEW);

  glViewport(0, 0, screenWidth, screenHeight);

  // GL_MODELVIEW is about having different
  // objects being pushed into a "world space".
  glMatrixMode(GL_MODELVIEW);
}

void displayLoop(void)
{
  angle += 0.1f;

  // Clear The Screen And The Depth Buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(posx, posy, posz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glPushMatrix();
  // Rotate quad around the y axis
  glRotatef(angle, 0.0f, 1.0f, 0.0f);
  // Set color for quad
  glBegin(GL_QUADS);

  // Top of cube
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);

  // Bottom of cube
  glColor3f(1.0f, 0.5f, 0.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);

  // Front of cube
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);

  // Back of cube.
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);

  // Left of cube
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);

  // Right of cube
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();
  glPopMatrix();

  // Lower the position of the plain and render
  glPushMatrix();
  glTranslatef(0.0f, -1.1f, -0.0f);

  // Bottom of cube
  glBegin(GL_QUADS);
  glColor3f(0.9f, 0.9f, 0.9f);
  glVertex3f(2.0f, 0.0f, 2.0f);
  glVertex3f(-2.0f, 0.0f, 2.0f);
  glVertex3f(-2.0f, 0.0f, -2.0f);
  glVertex3f(2.0f, 0.0f, -2.0f);
  glEnd();
  glPopMatrix();

  // Continue with GL, swap the double buffer
  glutSwapBuffers();
  glutPostRedisplay();
}

//
// The function called whenever a key is pressed.
void keyPressed(unsigned char key, int x, int y)
{
  usleep(100);

  // If escape is pressed, kill everything.
  if (key == ESCAPE)
  {
    glutDestroyWindow(window);
    exit(0);
  }
  else if (key == 's')
  {
    // move the camera back when the s character is pressed
    posz -= 0.5f;
  }
  else if (key == 'w')
  {
    posz += 0.5f;
  }
  else if (key == 'd')
  {
    posx += 0.5f;
  }
  else if (key == 'a')
  {
    posx -= 0.5f;
  }  // End of if - else
}

//
// Main entry point into the program
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(screenWidth, screenHeight);

  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("Mech Camera Demo");

  glutDisplayFunc(&displayLoop);
  glutIdleFunc(&displayLoop);
  // glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);

  // Initialize our GL window before main loop
  initGL();
  glutMainLoop();

  return 1;
}
