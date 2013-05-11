attribute  vec4 vPosition;
attribute vec2 vTexCoords;

uniform mat4 mvp;
uniform mat4 wt;
uniform mat4 translation;

void main()
{
  gl_TexCoord[0].xy = vTexCoords;
  gl_Position = mvp*translation*wt*vPosition;
}