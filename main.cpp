// ------------------------
// GLUT harness v. 1.0
// ------------------------

#include "Angel.h"
#include "camera.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a 5x5 cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -02.5, -02.5,  02.5, 1.0 ),
    point4( -02.5,  02.5,  02.5, 1.0 ),
    point4(  02.5,  02.5,  02.5, 1.0 ),
    point4(  02.5, -02.5,  02.5, 1.0 ),
    point4( -02.5, -02.5, -02.5, 1.0 ),
    point4( -02.5,  02.5, -02.5, 1.0 ),
    point4(  02.5,  02.5, -02.5, 1.0 ),
    point4(  02.5, -02.5, -02.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.5, 0.5, 0.5, 1.0 ),  // not white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

const GLfloat  dr = 10;

GLuint  mvp;  // model-view-projection matrix
GLuint world_transform; // transforms the world for the camera

// Projection transformation parameters

GLuint current_color = 0;

GLuint currentColor; // uniform for passing the current color to the shader
GLuint translation; // translation matrix

GLfloat window_width = 700, window_height = 700;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    colorcube();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
          NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(sizeof(points)) );

    mvp = glGetUniformLocation( program, "mvp" );
    world_transform = glGetUniformLocation (program, "wt");

    currentColor = glGetUniformLocation (program, "currentColor");
    translation = glGetUniformLocation (program, "translation");
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUniformMatrix4fv ( mvp, 1, GL_TRUE, camera.getMVPMatrix() );
    glUniformMatrix4fv ( world_transform, 1, GL_TRUE, camera.getWorldTransformMatrix() );

    /**
     * OpenGL 2.1, which I'm stuck on, does not allow me to use gl_InstanceID and
     * glDrawArraysInstanced. Instead, we are doing this manually... what a nightmare.
     */

     // Base translation matrix
     mat4 t;

     int cube_color;

     for (int i = 0; i < 2; i++)
     {
        // Place our cubes at ((+/-) 10, 0, 0)
        t = Translate (10 * pow(-1, i), 0, 0);

        glUniformMatrix4fv (translation, 1, GL_TRUE, t);

        // Color
        cube_color = 0;
        glUniform4f (currentColor, vertex_colors[cube_color][0], vertex_colors[cube_color][1], vertex_colors[cube_color][2], vertex_colors[cube_color][3]);

        glDrawArrays( GL_TRIANGLES, 0, NumVertices );
     }

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
    case 033: // Escape Key
    case 'q': case 'Q':
        exit( EXIT_SUCCESS );
        break;

    // translation
    case 'i': camera.moveOnZAxis(1); break;
    case 'j': camera.moveOnXAxis(-1); break;
    case 'k': camera.moveOnXAxis(1); break;
    case 'm': camera.moveOnZAxis(-1); break;

    // field of view
    case 'n': camera.changeFOV(-5); break;
    case 'w': camera.changeFOV(5); break;

    case ' ':  // reset values to their defaults
        camera.reset();
        break;
    }

    glutPostRedisplay();
}

void specialKeys (int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT: camera.rotateCamera(dr); break;
        case GLUT_KEY_RIGHT: camera.rotateCamera(-dr); break;
        case GLUT_KEY_UP: camera.moveOnYAxis(1); break;
        case GLUT_KEY_DOWN: camera.moveOnYAxis(-1); break;
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
    camera.setWindowDim (width, height);
    window_width = width;
    window_height = height;
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize( window_width, window_height );
    glutCreateWindow( "Assignment 4" );
    glewInit();

    // Set the default camera position
    camera.setDefaultPos (0, 0, 15);

    // And make sure the window size is set...
    camera.setWindowDim (window_width, window_height);

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutSpecialFunc (specialKeys);

    glutMainLoop();
    return 0;
}