attribute  vec4 vPosition;

uniform mat4 mvp;
uniform mat4 wt;
uniform mat4 translation;

void main()
{
  gl_Position = mvp*translation*wt*vPosition;
}