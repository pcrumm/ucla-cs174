// ------------------------------------
// Draw a gasket, based upon the book"s example code.
// ------------------------------------

#if defined(__APPLE__)
# include <GL/glew.h>
# include <GL/gl.h>
# include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

#include <stdlib.h>
#include "Angel.h"

/**
 * Viewport-related global variables.
 */
GLint windowWidth, windowHeight, windowXPos, windowYPos;
const int numGasketPoints = 5000;

void drawGasket(void)
{
    vec2 points[numGasketPoints];

    vec2 vertices[3] = {
        vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0) // The starting vertices for the triangle
    };

    points[0] = vec2(.25, .5); // Triangle mid, our initial point

    for (int i = 1; i < numGasketPoints; i++)
    {
        int j = rand() % 3;
        points[i] = (points[i-1] + vertices[j]) / 2.0;
    }

    // And for display...
    GLuint vert[1];
    glGenVertexArrays(1, vert);
    glBindVertexArray(vert[0]);

    // Setup the buffer
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Shaders
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Init the vertex positions for the shader
    GLuint pos = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // And set the clear color
    glClearColor(1.0, 1.0, 1.0, 1.0);

}

/**
 * display
 *
 * Make the screen magic happen.
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, numGasketPoints);
    glFlush();
}

/**
 * keyPress
 * Handler of keyPress events. We don"t do too many here so it"s a bit of a ghetto.
 */
void keyPress(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Quit
        case 'q':
        case 'Q':
            exit(0);

        // Otherwise, do nothing
        default:
            return;
    }
}

/**
 * registerEventHandlers
 *
 * A wrapper for all of the event handlers (click, press, etc.)
 * that we might want to use.
 */
void registerEventHandlers()
{
    // Keyboard presses
    glutKeyboardFunc(keyPress);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Setup the window with some sensible defaults
    windowXPos = windowYPos = 0;
    windowWidth = windowHeight = 500;
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowXPos, windowYPos);
    glutCreateWindow("CS174 - Assignment 1");


    // This will prevent an access violation on glGenVertexArrays
    glewExperimental = GL_TRUE;
    glewInit();

    drawGasket(); // Make sure everything we need is sent to the proper place

    // Display stuff and event handlers
    glutDisplayFunc(display);
    registerEventHandlers();

    // Fire up the rest
    glutMainLoop();

    return 0;
}