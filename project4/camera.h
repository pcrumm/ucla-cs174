#include "Angel.h"
#define PI 3.14159

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec2  point2;

/**
 * Some notes on camera philosophy.
 * By default, our camera is placed at the origin. We will
 * transform the world accordingly to fix into this box.
 */
class Camera
{
  // The effective position of the camera. We take the z axis to come
  // out of the screen.
  float x_pos, y_pos, z_pos;
  float default_x, default_y, default_z;

  // The camera's current rotation, in degrees. We take -z to be
  // at 0 degrees.
  int rotation;

  // Set the camera's FOV, in degrees.
  int fov;

  float window_width, window_height;

public:
  Camera ()
  {
    // We default to the origin for our coordinate system
    x_pos = 0;
    y_pos = 0;
    z_pos = 0;
    default_x = 0;
    default_y = 0;
    default_z = 0;

    fov = 100;
    rotation = 0;
  }

  float deg_to_rad(int angle)
  {
    return angle * (PI/180);
  }
  /**
   * MOVEMENT
   *
   * Note that all of our movement is relative to the camera's current
   * orientation. We update our _pos variables accordingly.
   */
  void moveOnXAxis (int distance)
  {
    this->x_pos += distance*cos(this->deg_to_rad(this->rotation));
    this->z_pos -= distance*sin(this->deg_to_rad(this->rotation));
  }

  void moveOnYAxis (int distance)
  {
    // The y-axis has no rotation, so it moves dependently
    this->y_pos += distance;
  }

  void moveOnZAxis (int distance)
  {
    this->z_pos -= distance*cos(this->deg_to_rad(this->rotation));
    this->x_pos -= distance*sin(this->deg_to_rad(this->rotation));
  }

  // We rotate in degrees; we define left to be a positive angle increase.
  void rotateCamera (int rotation)
  {
    this->rotation += rotation;
  }

  void changeFOV (int delta)
  {
    this->fov += delta;
  }

  void setWindowDim (int w, int h)
  {
    this->window_width = w;
    this->window_height = h;
  }

  void reset()
  {
    // These should be sensible defaults.
    this->x_pos = this->default_x;
    this->y_pos = this->default_y;
    this->z_pos = this->default_z;

    this->rotation = 0;
    this->fov = 100;
  }

  // Set the default camera positions. Used for resetting
  void setDefaultPos (int x, int y, int z)
  {
    this->x_pos = x;
    this->default_x = x;
    this->y_pos = y;
    this->default_y = y;
    this->z_pos = z;
    this->default_z = z;
  }

  // Defines the model-view-projection matrix, which should never change.
  mat4 getMVPMatrix()
  {
    // First, we need to get the projection matrix...
    mat4 p = Perspective (this->fov, this->window_width / this->window_height, .1, 1000);

    // Projection * Model_View will get us down into world coordinates
    mat4 mvp = p*this->getMVMatrix();

    return mvp;
  }

  mat4 getMVMatrix()
  {
    point4 eye (0, 0, 2, 1);
    point4 at (0, 0, 0, 1);
    vec4   up (0, 1, 0, 1);

    return LookAt (eye, at, up);
  }

  // Returns the effective transformation matrix necessary to move the
  // world. The world has the notion of the camera being moved, but
  // we are actually moving the world in the opposite direction.
  mat4 getWorldTransformMatrix()
  {
    return this->buildRotationMatrix()*this->buildTranslateMatrix();
  }

private:
  // Transforms the world based on the camera's position.
  mat4 buildTranslateMatrix()
  {
    return Translate ((GLfloat) -this->x_pos, (GLfloat) -this->y_pos, (GLfloat) -this->z_pos);  
  }

  // Rotates the world based on the camera's position.
  mat4 buildRotationMatrix()
  {
    return Translate(0, 0, 2)*RotateY (-this->rotation)*Translate(0, 0, -2);
  }
} camera;