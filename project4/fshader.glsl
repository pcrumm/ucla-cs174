uniform sampler2D Tex;
void main() 
{ 
    gl_FragColor = texture2D(Tex, vec2(gl_TexCoord[0]));
} 
