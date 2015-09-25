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
    vec3 distocam=pos-camPos;
    vec3 camvec=normalize(camDir);
    float disnorm=max(0, dot(distocam, camvec));
    disnorm/=viewRange;

    vec2 uv;
    uv.x=gl_FragCoord.x/1024.0f;
    uv.y=gl_FragCoord.y/768.f;
    if(curpass>0){
/*        float testres;
        if(curdepth==0) testres = textureProj(u_depth0, projCoord);
        else testres = textureProj(u_depth1, projCoord);
        if(testres<1.0) discard;*/
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

    vec3 lightPos=vec3(1,6,-2);
    vec3 lightDir=normalize(lightPos-pos);
    vec3 eyeDir=normalize(camPos-pos);

    //vec4 texColor = texture(textureSampler, tc);
    //diffuse
    float TdotL=dot(n,lightDir);
    float TdotV=dot(n, eyeDir);

    float kajiyaDiff=sin(acos(TdotL));
    if(kajiyaDiff<0) kajiyaDiff=0;
    kajiyaDiff=pow(kajiyaDiff,10);

    float kajiyaSpec=cos(abs(acos(TdotL)-acos(-TdotV)));
    if(kajiyaSpec<0) kajiyaSpec=0;
    kajiyaSpec=pow(kajiyaSpec,100);

    float vkajiyaSpec=0.7f+0.2f*kajiyaSpec;
    float vkajiyaDiff=0.8*kajiyaDiff;

    vec4 diff=texture(textureSampler, vec2(tc.x,vkajiyaDiff));
    vec4 spec=texture(textureSampler, vec2(tc.x,vkajiyaSpec));

    if(process==1){
        spec.a=kajiyaSpec;
        color=spec*0.7f;
        if(color.a<0.2) color=vec4(0,0,0,0);
        else {
            color.xyz*=5;
            color.a=0.2;
        }
        depthcolor=vec4(1,1,1,1);
        return;
    }

    color=diff*0.3+spec*0.7;
    if(tc.y<0.8){
        color.w=1;
    }else{
        float t=5*tc.y-4;
        color.w=2*pow(t, 3)-3*pow(t, 2)+1;
    }

    vec4 hairline=texture(hairlineSampler, tc);

    float diffval=abs(dot(eyeDir, diffn));
    float diffmul=2*tc.y-1;
    if(tc.y<0.5){
        diffmul=1;
    }else{
        diffmul=2 * pow(diffmul,3)-3*pow(diffmul,2)+1;
    }
    diffval*=diffmul;
    diffval=(1-diffval)*hairline.w;
    color.xyz=color.xyz*(1-diffval)+hairline.xyz*diffval;
//    color=vec4(diffval, diffval, diffval, 1);
    //color=color*0.5f+ hairline*0.5f;

    disnorm+=0.001f;
    depthcolor=vec4( disnorm,disnorm,disnorm,disnorm);
}
