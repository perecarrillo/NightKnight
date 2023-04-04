#version 330

uniform vec2 center;
uniform float radius;

in vec2 vtexCoord;

out vec4 outColor;

void main()
{
    float d = distance(vtexCoord, center);
    if (step(radius, d) == 0) discard;
    else outColor = vec4(0, 0, 0, 1);  
}

