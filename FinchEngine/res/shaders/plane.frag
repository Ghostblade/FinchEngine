#version 430

uniform sampler2D rtt0;
uniform sampler2D rtt1;

in vec2 v_texCoord;
layout(location = 0)out vec4 color;

void main(void)
{
    color=texture(rtt0, v_texCoord);
    //color=vec4(1,0,0,1);
}
