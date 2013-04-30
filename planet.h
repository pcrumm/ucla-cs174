#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;
typedef int          shader;

#ifndef PLANET_INC
#define PLANET_INC 1

#define SHADE_DEFAULT 1
#define SHADE_GOURD 2
#define SHADE_PHONG 3

class Planet {
public:
  int NumTimesToSubdivide;
  int NumTriangles;  // (4 faces)^(NumTimesToSubdivide + 1)
  int NumVertices;
  int scale;
  bool is_sun;
  float orbit_angle;
  float orbit_speed;

  // Attributes used for shading
  color4 ambient;
  color4 diffuse;
  color4 specular;
  float shininess;

  Planet(color4 ambient, color4 diffuse, color4 specular)
  {
    Index = 0;
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
    scale = 1;
    is_sun = false;
    orbit_angle = 0;
    orbit_speed = 0;

    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    shininess = 0.5;
  }

  /***
   * Drawing functions
   */
  void
  triangle( const point4& a, const point4& b, const point4& c, shader s )
  {
      if (s == SHADE_DEFAULT)
      {
        vec3 normal = normalize( cross(b - a, c - b) );

        normals[Index] = normal;
        points[Index] = a;
        Index++;

        normals[Index] = normal;
        points[Index] = b;
        Index++;

        normals[Index] = normal;
        points[Index] = c;
        Index++;
      }
      else // this is currently SHADE_GOURD. these need to be separated sometime
      {
        normals[Index] = vec3 (a.x, a.y, a.z);
        points[Index] = a;
        Index++;

        normals[Index] = vec3 (b.x, b.y, b.z);
        points[Index] = b;
        Index++;

        normals[Index] = vec3 (c.x, c.y, c.z);
        points[Index] = c;
        Index++;
      }
  }

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
           const point4& c, int count, shader s )
  {
      if ( count > 0 ) {
          point4 v1 = unit( a + b );
          point4 v2 = unit( a + c );
          point4 v3 = unit( b + c );
          divide_triangle(  a, v1, v2, count - 1, s );
          divide_triangle(  c, v2, v3, count - 1, s );
          divide_triangle(  b, v3, v1, count - 1, s );
          divide_triangle( v1, v3, v2, count - 1, s );
      }
      else {
          triangle( a, b, c, s );
      }
  }

  void
  tetrahedron( int count, shader s )
  {
      point4 v[4] = {
      vec4( 0.0, 0.0, 1.0, 1.0 ),
      vec4( 0.0, 0.942809, -0.333333, 1.0 ),
      vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
      vec4( 0.816497, -0.471405, -0.333333, 1.0 )
      };

      divide_triangle( v[0], v[1], v[2], count, s );
      divide_triangle( v[3], v[2], v[1], count, s );
      divide_triangle( v[0], v[3], v[1], count, s );
      divide_triangle( v[0], v[2], v[3], count, s );
  }

  void
  generate(int num_subdivisions, shader s)
  {
    this->NumTimesToSubdivide = num_subdivisions;
    this->NumTriangles = pow(4, num_subdivisions + 1);
    this->NumVertices = this->NumTriangles*3;

    points = new point4[NumVertices];
    normals = new vec3[NumVertices];

    tetrahedron (this->NumTimesToSubdivide, s);
  }

  /**
   * Accessors
   */
  point4*
  get_points()
  {
    return this->points;
  }

  vec3*
  get_normals()
  {
    return this->normals;
  }

  int
  num_points()
  {
    return sizeof(this->points[0])*this->NumVertices;
  }

  int
  num_normals()
  {
    return sizeof(this->normals[0])*this->NumVertices;
  }

  /**
   * Meta-data
   */
  // Returns a translation matrix for the position of the given planet relative
  // to the origin.
  mat4
  get_translation()
  {
    return RotateY (this->orbit_angle) * Translate (this->pos_x, this->pos_y, this->pos_z) * Scale (this->scale, this->scale, this->scale);
  }

  void set_position(int x, int y, int z)
  {
    this->pos_x = x;
    this->pos_y = y;
    this->pos_z = z;
  }

  void set_scale(int s)
  {
    this->scale = s;
  }

  void make_sun()
  {
    this->is_sun = true;
  }

  void orbit()
  {
    orbit_angle += this->orbit_speed;
  }

  void set_orbit_speed(float s)
  {
    this->orbit_speed = s;
  }

private:
  int Index;
  float pos_x, pos_y, pos_z;

  point4 *points;
  vec3 *normals;
};
#endif