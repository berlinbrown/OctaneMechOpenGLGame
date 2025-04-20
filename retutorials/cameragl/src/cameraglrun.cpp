/// basic cpp
/// Basic open gl program with FPS and camera and object usage
#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>  // For window/context creation
#include <cmath>

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

float fps = 0.0f;
int frameCount = 0;
TimePoint lastTime = Clock::now();

void updateFPS() {
    frameCount++;
    auto now = Clock::now();
    std::chrono::duration<double> elapsed = now - lastTime;

    if (elapsed.count() >= 1.0) {
        fps = static_cast<float>(frameCount) / elapsed.count();
        frameCount = 0;
        lastTime = now;
    }
}

void renderFPS(float fps) {
    // Prepare text
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << "FPS: " << fps;
    std::string text = ss.str();

    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // window size in pixels

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Optional: Disable lighting and depth test for HUD
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f); // white text
    glRasterPos2i(10, 570); // near top-left (y is from bottom in OpenGL)

    for (char c : text) {
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

static void renderCube(void)
{
	float v[3][3] = {0};
	float size = 1.0f;

	// change the size here
	// Note: starts from ground

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	glColor3f(1.0f, 0.0f, 0.0f);
	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Finish the front
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = size;

	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Draw the back triangle
	//-----------------------------
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = -size;

	glColor3f(1.0f, 1.0f, 0.0f);
	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// Finish the back
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = -size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

    //  Draw the right side
	//  Triangle
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	glColor3f(0.0f, 0.5f, 1.0f);
	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// FINISh the right side of the box
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// FINISh the left side of the box
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = size;

	glColor3f(1.0f, 0.5f, 1.0f);
	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// Draw the left side
	// Triangle
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = size;

	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Draw the top and bottom
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = -size;

	glColor3f(0.6f, 0.6f, 0.6f);
	// Calc normal and dra
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Draw one of the bottom triangles
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	// Calc normal and draw
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Lets finish the bottom with the second triangle
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	glColor3f(0.03f, 0.3f, 0.3f);
	// Calc normal and dra
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Go back and finish the top
	v[0][0] = -size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = -size;

	// Calc normal and dra
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	glEnd();

} 

/**
 * Main display routine, render scene
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Render triangle
    static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
    static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
    static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

    glPushMatrix();
    glBegin(GL_TRIANGLES);    
        glColor3f(1, 0, 0); 
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
        glVertex2f(-0.5f, -0.5f);
        
        glColor3f(0, 1, 0); 
        glVertex2f(0.5f, -0.5f);

        glColor3f(0, 0, 1); 
        glVertex2f(0.0f, 0.5f);
    glEnd();
    glFlush();
    glPopMatrix();

    // Render cube
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    renderCube();

    // Flip frame
    updateFPS();

    static GLfloat white[4] = {0.8, 0.8, 0.8, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    renderFPS(fps);
    glutSwapBuffers();
}

static void idle() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    std::cout << "Hello from C++ on macOS with clang!" << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Basic OpenGL on macOS");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // Setup scnee
    static GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
    GLint i;
    
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // Continue with camera setup
      
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 3.0, -10.0);

    // Continue display loop
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();    
    return 0;
}