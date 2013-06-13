attribute   vec4 vPosition;
attribute   vec3 vNormal;

// output values that will be interpretated per-fragment
varying  vec3 fN;
varying  vec3 fE;
varying  vec3 fL;
varying vec4 gourd_color;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;
uniform mat4 Translate;
uniform mat4 ModelTransform;
uniform float ShadeType;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

void main()
{
    fN = vNormal;
    fE = vPosition.xyz;
    fL = (ModelView*LightPosition).xyz;
    
    if( LightPosition.w != 0.0 ) {
  fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*Translate*ModelTransform*vPosition;

    // Gourd lighting, which is per-vertex
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec3 H = normalize( L + E );
    
    vec4 ambient = AmbientProduct;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    gourd_color = ambient + diffuse + specular;
}