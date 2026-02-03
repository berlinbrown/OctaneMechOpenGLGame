/// basic cpp
#include <GLUT/glut.h>  // For window/context creation
#include <OpenGL/gl.h>

#include <iostream>

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  glColor3f(1, 0, 0);
  glVertex2f(-0.5f, -0.5f);
  glColor3f(0, 1, 0);
  glVertex2f(0.5f, -0.5f);
  glColor3f(0, 0, 1);
  glVertex2f(0.0f, 0.5f);
  glEnd();
  glFlush();
}

int main(int argc, char** argv)
{
  std::cout << "Hello from C++ on macOS with clang!" << std::endl;
  glutInit(&argc, argv);
  glutCreateWindow("Basic OpenGL on macOS");
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}