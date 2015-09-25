#version 430

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

uniform mat4 biasm;
uniform mat4 mvp;

out vec3 n;
out vec2 tc;
out vec3 pos;

out vec4 projCoord;

void main()
{
    n=normalize(normal);
    pos=position;
    gl_Position = mvp * vec4(pos,1.0);

    tc = texcoord;

    projCoord = biasm * gl_Position;
}
