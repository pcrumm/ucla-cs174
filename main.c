#if defined(__APPLE__)
# include <GL/glew.h>
# include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

#include <stdlib.h>

void drawGasket(void)
{

}

/**
 * keyPress
 * Handler of keyPress events. We don't do too many here so it's a bit of a ghetto.
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

    // Setup the window
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("title");

    // Event handlers
    registerEventHandlers();

    // Fire up the rest
    glewInit();
    glutDisplayFunc(drawGasket);
    glutMainLoop();
}