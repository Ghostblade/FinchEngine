#version 430

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec4 diffnormal;

uniform mat4 mvp;

out vec3 n;
out vec2 tc;
out vec3 pos;
out vec3 diffn;

out vec4 projCoord;
uniform mat4 biasm;

void main()
{
    n=normalize(normal.xyz);
    pos=position.xyz;
    gl_Position = mvp * position;
    tc = texcoord;
    diffn=normalize(diffnormal.xyz);

    projCoord=biasm*gl_Position;
}
