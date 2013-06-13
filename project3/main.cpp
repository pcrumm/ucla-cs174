// fragment shading of sphere model

#include "Angel.h"
#include "camera.h"
#include "planet.h"
#include "moon.h"
#include "solar.h"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

int dr = 1;
int window_width, window_height;

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection, Translation, program, ModelTransform;

bool bind_camera = false; // If set to true, we attach the camera to the mud

//----------------------------------------------------------------------------

void add_planets()
{
    Planet *sun = new Planet (color4 (0, 0, 0, 1), color4 (0, 0, 0, 1), color4 (0, 0, 0, 1));
    sun->generate(7, SHADE_DEFAULT);
    sun->set_position(1, 1, 0);
    sun->set_scale(50);
    sun->make_sun();

    ss.add_planet(sun);

    // my ex lives here
    Planet *ice_queen = new Planet (color4 (.925, .98, .984, 1.0), color4 (.925, .98, .984, 1.0), color4 (.925, .98, .984, 1.0));
    ice_queen->generate(4, SHADE_DEFAULT);
    ice_queen->set_position(1, 1, 200);
    ice_queen->set_scale(15);
    ice_queen->set_orbit_speed(1.5);

    ss.add_planet(ice_queen);

    Planet *swamp = new Planet ( color4 (.46, .60, .41, 1), color4 (.46, .60, .41, 1), color4 (.46, .60, .41, 1));
    swamp->generate(4, SHADE_GOURD);
    swamp->set_position(1, 1, 320);
    swamp->set_scale(9);
    swamp->set_orbit_speed(-.5);

    ss.add_planet(swamp);

    Planet *water = new Planet ( color4 (.39, .77, .90, 1), color4 (.39, .77, .90, 1), color4 (.39, .77, .90, 1));
    water->generate(7, SHADE_PHONG);
    water->set_position(1, 1, 400);
    water->set_scale(4);
    water->set_orbit_speed(.7);
    water->shininess = 1.0;

    ss.add_planet(water);

    // EC: The moon, which will orbit the water planet
    Moon *m = new Moon (1, 1, 400);
    m->generate(7, SHADE_GOURD);
    m->set_scale(2);
    m->set_orbit_speed(.7);
    m->set_moon_speed(-4);

    ss.add_planet(m);

    Planet *mud = new Planet ( color4 (.64, .37, .062, 1), color4 (.64, .37, .062, 1), color4 (0, 0, 0, 1));
    mud->generate(6, SHADE_DEFAULT);
    mud->set_position(1, 1, 480);
    mud->set_scale(12);
    mud->set_orbit_speed(1.0);

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

    ss.add_planets_to_buffer();

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

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
    if (bind_camera) // If the camera is bound, limit actions
    {
        if (key != 'b' && key != 'q' && key != ' ')
            return;
    }

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

    // camera up and down rotation
    case 'e': camera.rotateCameraUD(5); break;
    case 'd': camera.rotateCameraUD(-5); break;

    // bind the camera to a planet
    case 'b':
        if (bind_camera)
        {
            bind_camera = false;
            camera.reset();
        }
        else
        {
            bind_camera = true;
        }
        break;

    case ' ':  // reset values to their defaults
        bind_camera = false;
        camera.reset();
        break;
    }

    glutPostRedisplay();
}

void specialKeys (int key, int x, int y)
{
    if (bind_camera) // Limit movement with a bound camera
    {
        if (key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT)
            return;
    }

    switch (key)
    {
        case GLUT_KEY_LEFT: camera.rotateCameraLR(dr); break;
        case GLUT_KEY_RIGHT: camera.rotateCameraLR(-dr); break;
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

    // If we're binding the camera, we attach it here...
    if (bind_camera)
    {
        vec4 planet_pos = ss.get_planet_position(5);
        camera.setDefaultPos(planet_pos.x, planet_pos.y + 6, planet_pos.z+30);
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

    window_width = 1024;
    window_height = 1024;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( window_width, window_height );

    camera.setWindowDim(window_width, window_height);

    glutCreateWindow( "Assignment 3" );

    camera.setDefaultPos (0, 70, 600);

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