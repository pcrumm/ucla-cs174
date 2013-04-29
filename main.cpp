// fragment shading of sphere model

#include "Angel.h"
#include "camera.h"
#include "planet.h"
#include "solar.h"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

int dr = 10;
int window_width, window_height;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection, Translation, program, ModelTransform;

//----------------------------------------------------------------------------

void add_planets()
{
    Planet sun;
    sun.generate(7);
    sun.set_position(1, 1, 0);
    sun.set_scale(50);
    sun.make_sun();

    ss.add_planet(sun);

    Planet ice_queen; // my ex lives here
    ice_queen.generate(4);
    ice_queen.set_position(1, 1, 300);
    ice_queen.set_scale(15);
    ice_queen.set_orbit_speed(1.5);

    ss.add_planet(ice_queen);

    Planet swamp;
    swamp.generate(4);
    swamp.set_position(1, 1, 380);
    swamp.set_scale(9);
    swamp.set_orbit_speed(-.5);

    ss.add_planet(swamp);

    Planet water;
    water.generate(7);
    water.set_position(1, 1, 400);
    water.set_scale(4);
    water.set_orbit_speed(.7);

    ss.add_planet(water);

    Planet mud;
    mud.generate(6);
    mud.set_position(1, 1, 420);
    mud.set_scale(12);
    mud.set_orbit_speed(1.0);

    ss.add_planet(mud);
}

// OpenGL initialization
void
init()
{
    add_planets();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    /* glBufferData( GL_ARRAY_BUFFER, p.num_points() + p.num_normals(),
          NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, p.num_points(), p.get_points() );
    glBufferSubData( GL_ARRAY_BUFFER, p.num_points(),
             p.num_normals(), p.get_normals() ); */

    ss.add_planets_to_buffer();

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize shader lighting parameters
    point4 light_position( 1.0, 1.0, 51.0, 0.0 );
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
    ModelTransform = glGetUniformLocation(program, "ModelTransform");
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( .1, .1, .1, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUniformMatrix4fv( ModelView, 1, GL_TRUE, camera.getMVMatrix() );
    glUniformMatrix4fv( Translation, 1, GL_TRUE, camera.getWorldTransformMatrix() );

    ss.draw_planets(program, ModelTransform);
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
    case 'i': camera.moveOnZAxis(5); break;
    case 'j': camera.moveOnXAxis(-5); break;
    case 'k': camera.moveOnXAxis(5); break;
    case 'm': camera.moveOnZAxis(-5); break;

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
        case GLUT_KEY_UP: camera.moveOnYAxis(5); break;
        case GLUT_KEY_DOWN: camera.moveOnYAxis(-5); break;
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

void
idle()
{
    ss.do_orbits();
    glutPostRedisplay();
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

    camera.setDefaultPos (0, 0, 500);

    glewInit();
    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutSpecialFunc (specialKeys);
    glutIdleFunc (idle);

    glutMainLoop();
    return 0;
}