#version 430

in vec2 coord;

uniform sampler2D textureSampler;

layout(location = 0)out vec4 color;
layout(location = 1)out vec4 depthcolor;

void main()
{

    color = texture(textureSampler, coord);
    color.a*=1.1;
    depthcolor=vec4(0,0,0,0);

    //color=vec4(0,0,0,1);
}
