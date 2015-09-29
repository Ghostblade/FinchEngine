#version 430

#define PEEL_BIAS 0.001


in vec2 tc;
in vec3 n;
in vec3 pos;
in vec3 diffn;
in vec4 projCoord;

layout(location = 0)out vec4 color;
layout(location = 1)out vec4 depthcolor;

uniform sampler2D textureSampler;
uniform sampler2D hairlineSampler;

uniform sampler2D u_depth0;
uniform sampler2D u_depth1;

uniform int curpass;
uniform int curdepth;

uniform vec3 camPos;
uniform vec3 camDir;
uniform float viewRange;

uniform int process;


void main()
{
    color=texture(textureSampler, tc);
    depthcolor=vec4(1,1,1,1);
}
