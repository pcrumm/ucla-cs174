attribute vec4 vPosition;

void main()
{
    float PI = 3.14159265358979323846264;
    float angle = 45.0;
    float rad_angle = angle*PI/180.0;

    vec4 position = gl_Vertex;
    position.x = position.x*cos(rad_angle) - position.y*sin(rad_angle) - .3;
    position.y = position.x*sin(rad_angle) + position.y*cos(rad_angle);

    gl_Position = gl_ModelViewProjectionMatrix * position;
}