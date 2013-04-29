#include "Angel.h"
#include "planet.h"
#include <vector>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;

/**
 * Contains our solar system and handles per-frame drawing
 * and positioning. By convention, the first planet we add
 * will be our sun.
 */
class SolarSystem {
public:
  SolarSystem()
  {
  }

  void
  add_planet(Planet p)
  {
    planets.insert (planets.end(), p);
  }

  void add_planets_to_buffer()
  {
    int offset = 0;

    // A little inefficient, but first figure out our necessary buffer size
    int buf_size = 0;

    for (int i = 0; i != planets.size(); i++)
    {
      buf_size += planets[i].num_points() + planets[i].num_normals();
    }

    // Setup the buffer
    glBufferData( GL_ARRAY_BUFFER, buf_size,
          NULL, GL_STATIC_DRAW );

    for (int i = 0; i != planets.size(); i++)
    {
      Planet p = planets[i];
      glBufferSubData( GL_ARRAY_BUFFER, offset, p.num_points(), p.get_points() );
      offset += p.num_points();
      glBufferSubData( GL_ARRAY_BUFFER, offset,
               p.num_normals(), p.get_normals() );

      offset += p.num_normals();
    }
  }

  void draw_planets(GLuint program, GLuint ModelTransform)
  {
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );

    // Initialize shader lighting parameters
    point4 light_position( 0.0, 0.0, -1.0, 0.0 );
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    int offset = 0;
    for (int i = 0; i != planets.size(); i++)
    {
      Planet p = planets[i];

      glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                 BUFFER_OFFSET(offset) );

      offset += p.num_points();

      glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                 BUFFER_OFFSET(offset) );

      offset += p.num_normals();

      glUniformMatrix4fv( ModelTransform, 1, GL_TRUE, p.get_translation() );

      glUniform1f (glGetUniformLocation (program, "isSun"), (p.is_sun ? 1.0 : 0.0));
 
      // Lighting stuff
      color4 ambient_product = light_ambient * p.ambient;
      color4 diffuse_product = light_diffuse * p.diffuse;
      color4 specular_product = light_specular * p.specular;

      glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
            1, ambient_product );
      glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
            1, diffuse_product );
      glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
            1, specular_product );
      
      glUniform4fv( glGetUniformLocation(program, "LightPosition"),
            1, light_position );

      glUniform1f( glGetUniformLocation(program, "Shininess"),
           p.shininess );

      glDrawArrays (GL_TRIANGLES, 0, p.NumVertices);
    }
  }

  void do_orbits()
  {
    for (int i = 0; i != planets.size(); i++)
    {
      if (planets[i].is_sun)
        continue;

      planets[i].orbit();
    }
  }
private:
  std::vector<Planet> planets;
} ss;