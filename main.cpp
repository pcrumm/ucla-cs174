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
const int numGasketPoints = 50000;
int numDrawnPoints;
int mainWindow;
bool draw_mandelbrot = false; // If this is false, we draw the SG.
GLint redDraw = 1, blueDraw = 0, greenDraw = 0;

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

    numDrawnPoints = numGasketPoints;

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

    GLint colorUniform = glGetUniformLocation(program, "colorUniform");
    glUniform3f(colorUniform, redDraw, greenDraw, blueDraw);

    // Init the vertex positions for the shader
    GLuint pos = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // And set the clear color
    glClearColor(1.0, 1.0, 1.0, 1.0);

}

/**
 * Draw a mandelbrot fractal.
 * http://en.wikipedia.org/wiki/Mandelbrot_set
 * http://warp.povusers.org/Mandelbrot/
 */
void drawMandelbrot(void)
{
    vec2 points[numGasketPoints];

    // The acceptable ranges for the fractal. real in (-2,1), imaginary in (-1.2, 1.8)
    float min_real = -2.0;
    float max_real = 1.0;
    float min_imag = -1.2;
    float max_imag = min_imag + (max_real - min_real);

    // Scaling factors, so we stay in our ranges
    float real_factor = (max_real-min_real)/(windowWidth-1);
    float imag_factor = (max_imag-min_imag)/(windowHeight-1);

    // Some clerical stuff
    unsigned max_iter = 30;
    int pixel_count = 0;

    for (float y=0; y<windowHeight; ++y)
    {
        float c_imag = max_imag - y*imag_factor;
        for (float x=0; x<windowWidth; ++x)
        {
            float c_real = min_real + x*real_factor;

            float z_real = c_real, z_imag = c_imag;
            bool should_draw = true;
            for (unsigned n=0; n<max_iter; ++n)
            {
                float z_real2 = z_real*z_real, z_imag2 = z_imag*z_imag;
                if (z_real2 + z_imag2 > 4)
                {
                    should_draw = false;
                    break;
                }
                z_imag = 2*z_real*z_imag + c_imag;
                z_real = z_real2 - z_imag2 + c_real;
            }
            if (should_draw)
            {
                // We need to normalize all pixels to be between 1 and -1.
                points[pixel_count++] = vec2(((2 * x) / windowHeight - 1) , ((2 * y) / windowHeight - 1));
            }
        }
    }

    numDrawnPoints = pixel_count;

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

    GLint colorUniform = glGetUniformLocation(program, "colorUniform");
    glUniform3f(colorUniform, redDraw, greenDraw, blueDraw);

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
    glDrawArrays(GL_POINTS, 0, numDrawnPoints);
    glFlush();
}

/**
 * draw_proper_fractal
 * Draws the correct fractal based on the value of draw_mandelbrot.
 */
void drawProperFractal(void)
{
    if (draw_mandelbrot)
    {
        drawMandelbrot();
    }
    else
    {
        drawGasket();
    }
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
            break;

        // Color selection
        case 'r':
            redDraw = 1.0;
            greenDraw = blueDraw = 0.0;
            break;
        case 'g':
            greenDraw = 1.0;
            redDraw = blueDraw = 0.0;
            break;
        case 'b':
            blueDraw = 1.0;
            greenDraw = redDraw = 0.0;
            break;

        // Switch fractals
            case 'm': // Draw the mandelbrot
                draw_mandelbrot = true;
                break;
            case 's':
                draw_mandelbrot = false;
                break;

        // Otherwise, do nothing
        default:
            return;
    }

    drawProperFractal();
    glutSetWindow(mainWindow);
    glutPostRedisplay();
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
    mainWindow = glutCreateWindow("CS174 - Assignment 1");


    // This will prevent an access violation on glGenVertexArrays
    glewExperimental = GL_TRUE;
    glewInit();

    // Draw the correct fractal, based on our options
    drawProperFractal();

    // Display stuff and event handlers
    glutDisplayFunc(display);
    registerEventHandlers();

    // Fire up the rest
    glutMainLoop();

    return 0;
}