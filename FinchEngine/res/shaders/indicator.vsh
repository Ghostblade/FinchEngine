#version 430 core

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texcoord;

uniform mat4 mvp;
uniform vec3 trans;

void main(void)
{
        vec4 pos=position;
        pos.xyz*=2;
        pos+=vec4(trans,0);
    gl_Position = mvp * pos;
}



