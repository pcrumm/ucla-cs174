attribute   vec4 vPosition;
attribute   vec3 vNormal;

// output values that will be interpretated per-fragment
varying  vec3 fN;
varying  vec3 fE;
varying  vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;
uniform mat4 Translate;
uniform mat4 ModelTransform;

void main()
{
    fN = vNormal;
    fE = vPosition.xyz;
    fL = (ModelView*LightPosition).xyz;
    
    if( LightPosition.w != 0.0 ) {
  fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*Translate*ModelTransform*vPosition;
}