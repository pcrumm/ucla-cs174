// fragment shading of sphere model

#include "Angel.h"
#include "camera.h"

const int NumTimesToSubdivide = 5;
const int NumTriangles        = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices         = 3 * NumTriangles;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

point4 points[NumVertices];
vec3   normals[NumVertices];

int dr = 10;
int window_width, window_height;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection, Translation;

//----------------------------------------------------------------------------

int Index = 0;

void
triangle( const point4& a, const point4& b, const point4& c )
{
    vec3  normal = normalize( cross(b - a, c - b) );

    normals[Index] = normal;  points[Index] = a;  Index++;
    normals[Index] = normal;  points[Index] = b;  Index++;
    normals[Index] = normal;  points[Index] = c;  Index++;
}

//----------------------------------------------------------------------------

point4
unit( const point4& p )
{
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
    point4 t;
    if ( len > DivideByZeroTolerance ) {
    t = p / sqrt(len);
    t.w = 1.0;
    }

    return t;
}

void
divide_triangle( const point4& a, const point4& b,
         const point4& c, int count )
{
    if ( count > 0 ) {
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle(  a, v1, v2, count - 1 );
        divide_triangle(  c, v2, v3, count - 1 );
        divide_triangle(  b, v3, v1, count - 1 );
        divide_triangle( v1, v3, v2, count - 1 );
    }
    else {
        triangle( a, b, c );
    }
}

void
tetrahedron( int count )
{
    point4 v[4] = {
    vec4( 0.0, 0.0, 1.0, 1.0 ),
    vec4( 0.0, 0.942809, -0.333333, 1.0 ),
    vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
    vec4( 0.816497, -0.471405, -0.333333, 1.0 )
    };

    divide_triangle( v[0], v[1], v[2], count );
    divide_triangle( v[3], v[2], v[1], count );
    divide_triangle( v[0], v[3], v[1], count );
    divide_triangle( v[0], v[2], v[3], count );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    // Subdivide a tetrahedron into a sphere
    tetrahedron( NumTimesToSubdivide );

    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
          NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),
             sizeof(normals), normals );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(sizeof(points)) );

    // Initialize shader lighting parameters
    point4 light_position( 0.0, 0.0, 2.0, 0.0 );
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
    color4 material_specular( 1.0, 0.0, 1.0, 1.0 );
    float  material_shininess = 5.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
          1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
          1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
          1, specular_product );
    
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
          1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
         material_shininess );
         
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    Translation = glGetUniformLocation(program, "Translate");
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUniformMatrix4fv( ModelView, 1, GL_TRUE, camera.getMVMatrix() );
    glUniformMatrix4fv( Translation, 1, GL_TRUE, camera.getWorldTransformMatrix() );

    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
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
    window_width = width;
    window_height = height;
    camera.setWindowDim(width, height);

    glUniformMatrix4fv( Projection, 1, GL_TRUE, camera.getPMatrix() );
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

    window_width = 512;
    window_height = 512;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( window_width, window_height );

    camera.setWindowDim(window_width, window_height);

    glutCreateWindow( "Assignment 3" );

    camera.setDefaultPos (0, 0, 2);

    glewInit();
    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutSpecialFunc (specialKeys);

    glutMainLoop();
    return 0;
}