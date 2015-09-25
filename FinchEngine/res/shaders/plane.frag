#version 430

uniform sampler2D rtt0;
uniform sampler2D rtt1;
uniform sampler2D rtt2;
uniform sampler2D rtt3;
uniform sampler2D rtt4;
uniform sampler2D rtt5;
uniform sampler2D rtt6;
uniform sampler2D rtt7;
uniform sampler2D rtt8;

uniform sampler2D dep0;
uniform sampler2D dep1;

in vec2 v_texCoord;
out vec4 fragColor;

uniform int process;

 int kernel[25]=int[25](1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,4,16,26,16,4,1,4,7,4,1);

void main(void)
{

    vec2 uv;
    uv.x=gl_FragCoord.x/1024;
    uv.y=gl_FragCoord.y/768;

    if(process==1){
        float u_fracture=1.0f/1024;
        float v_fracture=1.0f/768;
        fragColor=texture(rtt8, uv);
        vec4 marktex=texture(rtt1,uv);
        if(marktex.x>0.8){
            fragColor=vec4(0,0,0,0);
            for(int i=-2;i<=2;++i){
                for(int j=-2;j<=2;++j){
                    fragColor+=kernel[5*j+i+12]*texture(rtt8, uv+vec2(i*u_fracture, j*v_fracture));
                }
            }
            fragColor/=273.0f;
//            fragColor=vec4(1,0,0,1);

        }

        marktex=texture(rtt0, uv);
        fragColor=(1-marktex.w)*fragColor+marktex*0.6;
        return;
    }

    //fragColor = texture(dep1, v_texCoord);

    vec4 colaray[7];
    fragColor=texture(rtt7, uv);
    colaray[6]=texture(rtt6, uv);
    colaray[5]=texture(rtt5, uv);
    colaray[4]=texture(rtt4, uv);
    colaray[3]=texture(rtt3, uv);
    colaray[2]=texture(rtt2, uv);
    colaray[1]=texture(rtt1, uv);
    colaray[0]=texture(rtt0, uv);

    fragColor*=fragColor.w;
    for(int i=6;i>=0;--i){
        fragColor=fragColor*(1-colaray[i].w)+colaray[i].w*colaray[i];
        fragColor.w=1;
    }

}
