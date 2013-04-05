uniform vec3 colorUniform;

void main()
{
    gl_FragColor = vec4(colorUniform.x, colorUniform.y, colorUniform.z, 1.0);
}