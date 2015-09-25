#version 430
#define PEEL_BIAS 0.001

in vec2 tc;
in vec3 n;
in vec3 pos;

in vec4 projCoord;


layout(location = 0)out vec4 color;
layout(location = 1)out vec4 depthcolor;


uniform sampler2D textureSampler;

uniform sampler2D u_depth0;
uniform sampler2D u_depth1;

uniform int curpass;
uniform int curdepth;

uniform vec3 camPos;
uniform vec3 camDir;
uniform float viewRange;

uniform int process;


void main(){
    if(process==1){
        color=vec4(0,0,0,0);

        depthcolor=vec4(0.5,0.5,0.5,1);
        return;
    }

    vec3 distocam=pos-camPos;
    vec3 camvec=normalize(camDir);
    float disnorm=max(0, dot(distocam, camvec));
    disnorm/=viewRange;

    vec2 uv;
    uv.x=gl_FragCoord.x/1024.0f;
    uv.y=gl_FragCoord.y/768.f;

    if(curpass>0){
        vec4 depval;
        if(curdepth==0){
            depval=texture(u_depth0, uv);
        }else{
            depval=texture(u_depth1, uv);
        }
        if(depval.x==0) depval.x=1;
        if(disnorm<=depval.x){
            discard;
        }
    }
    color=texture(textureSampler, tc);
    //gl_FragDepth = projCoord.z / projCoord.w + PEEL_BIAS;
    disnorm+=0.001f;
    depthcolor=vec4( disnorm,disnorm,disnorm, disnorm);
}
