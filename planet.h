#pragma once
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;
typedef int          shader;

#define SHADE_DEFAULT 1
#define SHADE_GOURD 2
#define SHADE_PHONG 3

class Moon;

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

  Planet(color4 ambient, color4 diffuse, color4 specular);

  /***
   * Drawing functions
   */
  void
  triangle( const point4& a, const point4& b, const point4& c, shader s );

  point4
  unit( const point4& p );

  void
  divide_triangle( const point4& a, const point4& b,
           const point4& c, int count, shader s );

  void
  tetrahedron( int count, shader s );

  void
  generate(int num_subdivisions, shader s);

  /**
   * Accessors
   */
  point4*
  get_points();

  vec3*
  get_normals();

  int
  num_points();

  int
  num_normals();

  /**
   * Meta-data
   */
  // Returns a translation matrix for the position of the given planet relative
  // to the origin.
  virtual mat4
  get_translation();

  void
  set_position(int x, int y, int z);

  void
  set_scale(int s);

  void
  make_sun();

  virtual void
  orbit();

  void
  set_orbit_speed(float s);

  vec4
  get_position();

protected:
  int Index;
  float pos_x, pos_y, pos_z;

  point4 *points;
  vec3 *normals;
};