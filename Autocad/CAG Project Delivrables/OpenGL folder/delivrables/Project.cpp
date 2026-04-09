#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

float tank1Level = 0.5f;
float tank2Level = 0.1f;

// Function to draw a rectangle
void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw the scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Tank dimensions
    float tankWidth = 0.4f;
    float tankHeight = 0.6f;
    float pipeWidth = 0.8f;
    float pipeHeight = 0.1f;

    // Draw tanks
    drawRectangle(-0.8f, -0.5f, tankWidth, tankHeight, 1.0f, 0.9f, 0.0f); // Tank 1 (outline)
    drawRectangle(0.4f, -0.5f, tankWidth, tankHeight, 1.0f, 0.9f, 0.0f);  // Tank 2 (outline)

    // Draw tank levels
    drawRectangle(-0.8f, -0.5f, tankWidth, tank1Level, 0.0f, 0.9f, 1.0f); // Tank 1 level
    drawRectangle(0.4f, -0.5f, tankWidth, tank2Level, 0.0f, 0.9f, 1.0f); // Tank 2 level

    // Draw pipe
    drawRectangle(-0.4f, -0.5f, pipeWidth, pipeHeight, 0.0f, 0.9f, 1.0f); // Pipe at the bottom of the tanks

    glFlush();
}

// Function to update the scene
void updateScene(int value) {
    // Tank height and transfer rate
    float tankHeight = 0.6f;
    float transferRate = 0.001f;

    // Calculate the middle of the tanks
    const float MIDDLE_LEVEL = tankHeight / 2.0f;

    // Transfer liquid until both levels reach the middle
    if (tank1Level > MIDDLE_LEVEL && tank2Level < MIDDLE_LEVEL) {
        tank1Level -= transferRate;
        tank2Level += transferRate;
        glutPostRedisplay(); // Trigger a redraw
        glutTimerFunc(10, updateScene, 0); // Schedule the next update
    }
}

// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Example");

    // Register callback functions
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    

    // Initialize OpenGL
    glClearColor(1.0f, 0.5f, 1.0f, 1.0f);

    // Start the animation loop
    glutTimerFunc(25, updateScene, 0); // Start the update process

    glutMainLoop(); // Start the main event loop

    return 0;
}
