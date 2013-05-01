#include "planet.h"


Planet::Planet(color4 ambient, color4 diffuse, color4 specular)
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
Planet::triangle( const point4& a, const point4& b, const point4& c, shader s )
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
Planet::unit( const point4& p )
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
Planet::divide_triangle( const point4& a, const point4& b,
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
Planet::tetrahedron( int count, shader s )
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
Planet::generate(int num_subdivisions, shader s)
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
Planet::get_points()
{
  return this->points;
}

vec3*
Planet::get_normals()
{
  return this->normals;
}

int
Planet::num_points()
{
  return sizeof(this->points[0])*this->NumVertices;
}

int
Planet::num_normals()
{
  return sizeof(this->normals[0])*this->NumVertices;
}

/**
 * Meta-data
 */
// Returns a translation matrix for the position of the given planet relative
// to the origin.
mat4
Planet::get_translation()
{
  return RotateY (this->orbit_angle) * Translate (this->pos_x, this->pos_y, this->pos_z) * Scale (this->scale, this->scale, this->scale);
}

void
Planet::set_position(int x, int y, int z)
{
  this->pos_x = x;
  this->pos_y = y;
  this->pos_z = z;
}

void
Planet::set_scale(int s)
{
  this->scale = s;
}

void
Planet::make_sun()
{
  this->is_sun = true;
}

void
Planet::orbit()
{
  orbit_angle += this->orbit_speed;
}

void
Planet::set_orbit_speed(float s)
{
  this->orbit_speed = s;
}