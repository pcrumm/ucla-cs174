#pragma once
#include "Planet.h"
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;
typedef int          shader;

class Moon : public Planet
{
public:
  bool is_moon;
  int orbit_radius;
  float planet_orbit_angle;

  float moon_speed;

  Moon (int planet_origin_x, int planet_origin_y, int planet_origin_z)
    : Planet (color4 (.925, .98, .984, 1.0), color4 (.925, .98, .984, 1.0), color4 (.925, .98, .984, 1.0))
  {
    is_moon = true;
    orbit_radius = 10;
    planet_orbit_angle = 0;

    this->orig_x = planet_origin_x;
    this->orig_y = planet_origin_y;
    this->orig_z = planet_origin_z;

    Planet::set_position(this->orig_x, this->orig_y, this->orig_z);
  }

  virtual mat4
  get_translation()
  {
    return
      RotateY (this->orbit_angle)
      * Translate (this->pos_x, this->pos_y, this->pos_z)
      * Scale (this->scale, this->scale, this->scale)
      * RotateY (this->planet_orbit_angle)
      * Translate (0, 0, this->orbit_radius)
      * RotateY (this->orbit_angle);
  }

  virtual void
  orbit()
  {
    this->planet_orbit_angle += this->moon_speed;
    Planet::orbit();
  }

  void set_moon_speed(float s)
  {
    this->moon_speed = s;
  }

private:
  int orig_x, orig_y, orig_z;
};