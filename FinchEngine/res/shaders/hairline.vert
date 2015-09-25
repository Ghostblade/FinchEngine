#version 430

layout(location=0) in vec4 position;
layout(location=1) in vec4 vertag;

out vec4 tc;
out vec3 linepos;

void main()
{
    gl_Position = position;
    tc=vertag;
    linepos=position.xyz;
}
