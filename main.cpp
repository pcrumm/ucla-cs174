// ------------------------
// GLUT harness v. 1.0
// ------------------------

#include "Angel.h"
#include "camera.h"
#include "tga.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec3  point3;
typedef Angel::vec2  point2;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices]; // cube points
point3 cubeNormals[NumVertices];
point2 cubeUV[NumVertices]; // for texture mapping

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

const GLfloat  dr = 10;

GLuint  mvp;  // model-view-projection matrix
GLuint world_transform; // transforms the world for the camera

// Projection transformation parameters

GLuint cube_texture; // Texture identifier
GLint uniformTex; // Texture uniform
GLuint translation; // translation matrix
GLuint vao;
GLuint program;

GLfloat window_width = 700, window_height = 700;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void quad( int a, int b, int c, int d, const point3& normal )
{
    points[Index] = vertices[a]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0.0f, 1.0f); Index++;
    points[Index] = vertices[b]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0.0f, 0.0f); Index++;
    points[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 0.0f); Index++;
    points[Index] = vertices[a]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0.0f, 1.0f); Index++;
    points[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 0.0f); Index++;
    points[Index] = vertices[d]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1.0f, 1.0f); Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2, point3( 0.0f,  0.0f,  1.0f) );
    quad( 2, 3, 7, 6, point3( 1.0f,  0.0f,  0.0f) );
    quad( 3, 0, 4, 7, point3( 0.0f, -1.0f,  0.0f) );
    quad( 6, 5, 1, 2, point3( 0.0f,  1.0f,  0.0f) );
    quad( 4, 5, 6, 7, point3( 0.0f,  0.0f, -1.0f) );
    quad( 5, 4, 0, 1, point3(-1.0f,  0.0f,  0.0f) );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    glEnable( GL_DEPTH_TEST );
    colorcube();

    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram(program);

    // Create a vertex array object
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
     GLuint buffer[2];
     glGenBuffers( 2, buffer );

     int psize = sizeof(points);
     int nsize = sizeof(cubeNormals);

     glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
     glBufferData( GL_ARRAY_BUFFER, psize, points, GL_STATIC_DRAW );
     GLuint vPosition = glGetAttribLocation( program, "vPosition" );
     glEnableVertexAttribArray( vPosition );
     glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

     glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
     glBufferData( GL_ARRAY_BUFFER, nsize, cubeNormals, GL_STATIC_DRAW );
     GLuint vNormal = glGetAttribLocation( program, "vNormal" );
     glEnableVertexAttribArray( vNormal );
     glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // Map our texture coordinates
    GLuint tbuffer;
    int tsize =  sizeof(cubeUV);
    glGenBuffers( 1, &tbuffer );
    glBindBuffer( GL_ARRAY_BUFFER, tbuffer );
    glBufferData( GL_ARRAY_BUFFER, tsize, cubeUV, GL_STATIC_DRAW );
    GLuint vTexCoords = glGetAttribLocation( program, "vTexCoords" );
    glEnableVertexAttribArray( vTexCoords );
    glVertexAttribPointer( vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    // MVP/WT matrices
    mvp = glGetUniformLocation( program, "mvp" );
    world_transform = glGetUniformLocation (program, "wt");

    translation = glGetUniformLocation (program, "translation");

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

     glBindVertexArrayAPPLE( vao );

     for (int i = 0; i < 2; i++)
     {
        // Place our cubes at ((+/-) 10, 0, 0)
        t = Translate (10 * pow(-1, i), 0, 0);

        glUniformMatrix4fv (translation, 1, GL_TRUE, t);

        // Texturing
        glEnable( GL_TEXTURE_2D );

        // Load texture
        TgaImage slab;
        if (!slab.loadTGA("tennis.tga"))
        {
            printf("Could not load texture file.\n");
            exit(1);
        }

        glGenTextures(1, &cube_texture);
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, slab.width, slab.height, 0,
                     (slab.byteCount == 3) ? GL_BGR : GL_BGRA,
                     GL_UNSIGNED_BYTE, slab.data );
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

        if (i == 0) // first uses nearest-neighbor filtering
        {
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else //mipmap the second with trilinear filtering
        {
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        uniformTex = glGetUniformLocation(program, "Tex");
        glUniform1i(uniformTex, 0);

        glDisable( GL_TEXTURE_2D );


        glBindTexture( GL_TEXTURE_2D, cube_texture);
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
    case 'o': camera.moveOnZAxis(-1); break;

    case ' ':  // reset values to their defaults
        camera.reset();
        break;
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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize( window_width, window_height );
    glutCreateWindow( "Assignment 4" );
    glewInit();


    // Set the default camera position
    camera.setDefaultPos (0, 0, 30);

    // And make sure the window size is set...
    camera.setWindowDim (window_width, window_height);

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );

    glutMainLoop();
    return 0;
}