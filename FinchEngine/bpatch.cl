#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_extended_atomics : enable



#define ONE_F1                 (1.0f)
#define ZERO_F1                (0.0f)

#define USE_IMAGES_FOR_RESULTS (0)  // NOTE: It may be faster to use buffers instead of images

__constant float4 ZERO_F4 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
__constant float4 ONE_F4 = (float4)(1.0f, 1.0f, 1.0f, 1.0f);

////////////////////////////////////////////////////////////////////////////////////////////////////

__constant int P_MASK = 255;
__constant int P_SIZE = 256;
__constant int P[512] = {151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  };

////////////////////////////////////////////////////////////////////////////////////////////////////

__constant int G_MASK = 15;
__constant int G_SIZE = 16;
__constant int G_VECSIZE = 4;
__constant float G[16*4] = {
          +ONE_F1,  +ONE_F1, +ZERO_F1, +ZERO_F1,
          -ONE_F1,  +ONE_F1, +ZERO_F1, +ZERO_F1,
          +ONE_F1,  -ONE_F1, +ZERO_F1, +ZERO_F1,
          -ONE_F1,  -ONE_F1, +ZERO_F1, +ZERO_F1,
          +ONE_F1, +ZERO_F1,  +ONE_F1, +ZERO_F1,
          -ONE_F1, +ZERO_F1,  +ONE_F1, +ZERO_F1,
          +ONE_F1, +ZERO_F1,  -ONE_F1, +ZERO_F1,
          -ONE_F1, +ZERO_F1,  -ONE_F1, +ZERO_F1,
         +ZERO_F1,  +ONE_F1,  +ONE_F1, +ZERO_F1,
         +ZERO_F1,  -ONE_F1,  +ONE_F1, +ZERO_F1,
         +ZERO_F1,  +ONE_F1,  -ONE_F1, +ZERO_F1,
         +ZERO_F1,  -ONE_F1,  -ONE_F1, +ZERO_F1,
          +ONE_F1,  +ONE_F1, +ZERO_F1, +ZERO_F1,
          -ONE_F1,  +ONE_F1, +ZERO_F1, +ZERO_F1,
         +ZERO_F1,  -ONE_F1,  +ONE_F1, +ZERO_F1,
         +ZERO_F1,  -ONE_F1,  -ONE_F1, +ZERO_F1
};

float mix1d(float a, float b, float t)
{
        float ba = b - a;
        float tba = t * ba;
        float atba = a + tba;
        return atba;
}


int mod(int x, int a)
{
        int n = (x / a);
        int v = v - n * a;
        if ( v < 0 )
                v += a;
        return v;
}

float smooth(float t)
{
        return t*t*t*(t*(t*6.0f-15.0f)+10.0f);
}

float4 normalized(float4 v)
{
        float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    d = d > 0.0f ? d : 1.0f;
        float4 result = (float4)(v.x, v.y, v.z, 0.0f) / d;
        result.w = 1.0f;
    return result;
}


float2 mix2d(float2 a, float2 b, float t)
{
        float2 ba = b - a;
        float2 tba = t * ba;
        float2 atba = a + tba;
        return atba;
}

float4 mix3d(float4 a, float4 b, float t)
{
        float4 ba = b - a;
        float4 tba = t * ba;
        float4 atba = a + tba;
        return atba;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int lattice1d(int i)
{
        return P[i];
}

int lattice2d(int2 i)
{
        return P[i.x + P[i.y]];
}

int lattice3d(int4 i)
{
        return P[i.x + P[i.y + P[i.z]]];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float gradient1d(int i, float v)
{
        int index = (lattice1d(i) & G_MASK) * G_VECSIZE;
        float g = G[index + 0];
        return (v * g);
}

float gradient2d(int2 i, float2 v)
{
        int index = (lattice2d(i) & G_MASK) * G_VECSIZE;
        float2 g = (float2)(G[index + 0], G[index + 1]);
        return dot(v, g);
}

float gradient3d(int4 i, float4 v)
{
        int index = (lattice3d(i) & G_MASK) * G_VECSIZE;
        float4 g = (float4)(G[index + 0], G[index + 1], G[index + 2], 1.0f);
        return dot(v, g);
}

float sgnoise3d(float4 position)
{

        float4 p = position;
        float4 pf = floor(p);
        int4 ip = (int4)((int)pf.x, (int)pf.y, (int)pf.z, 0.0);
        float4 fp = p - pf;
    ip &= P_MASK;

    int4 I000 = (int4)(0, 0, 0, 0);
    int4 I001 = (int4)(0, 0, 1, 0);
    int4 I010 = (int4)(0, 1, 0, 0);
    int4 I011 = (int4)(0, 1, 1, 0);
    int4 I100 = (int4)(1, 0, 0, 0);
    int4 I101 = (int4)(1, 0, 1, 0);
    int4 I110 = (int4)(1, 1, 0, 0);
    int4 I111 = (int4)(1, 1, 1, 0);

    float4 F000 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 F001 = (float4)(0.0f, 0.0f, 1.0f, 0.0f);
    float4 F010 = (float4)(0.0f, 1.0f, 0.0f, 0.0f);
    float4 F011 = (float4)(0.0f, 1.0f, 1.0f, 0.0f);
    float4 F100 = (float4)(1.0f, 0.0f, 0.0f, 0.0f);
    float4 F101 = (float4)(1.0f, 0.0f, 1.0f, 0.0f);
    float4 F110 = (float4)(1.0f, 1.0f, 0.0f, 0.0f);
    float4 F111 = (float4)(1.0f, 1.0f, 1.0f, 0.0f);

        float n000 = gradient3d(ip + I000, fp - F000);
        float n001 = gradient3d(ip + I001, fp - F001);

        float n010 = gradient3d(ip + I010, fp - F010);
        float n011 = gradient3d(ip + I011, fp - F011);

        float n100 = gradient3d(ip + I100, fp - F100);
        float n101 = gradient3d(ip + I101, fp - F101);

        float n110 = gradient3d(ip + I110, fp - F110);
        float n111 = gradient3d(ip + I111, fp - F111);

        float4 n40 = (float4)(n000, n001, n010, n011);
        float4 n41 = (float4)(n100, n101, n110, n111);

        float4 n4 = mix3d(n40, n41, smooth(fp.x));
        float2 n2 = mix2d(n4.xy, n4.zw, smooth(fp.y));
        float n = mix1d(n2.x, n2.y, smooth(fp.z));
        return n * (1.0f / 0.7f);
}


float turbulence3d(
        float4 position,
        float frequency,
        float lacunarity,
        float increment,
        float octaves)
{
        int i = 0;
        float fi = 0.0f;
        float remainder = 0.0f;
        float sample = 0.0f;
        float value = 0.0f;
        int iterations = (int)octaves;

        for (i = 0; i < iterations; i++)
        {
                fi = (float)i;
                sample = (1.0f - 2.0f * sgnoise3d(position * frequency));
                sample *= pow( lacunarity, -fi * increment );
                value += fabs(sample);
                frequency *= lacunarity;
        }

        remainder = octaves - (float)iterations;
        if ( remainder > 0.0f )
        {
                sample = remainder * (1.0f - 2.0f * sgnoise3d(position * frequency));
                sample *= pow( lacunarity, -fi * increment );
                value += fabs(sample);
        }

        return value;
}

float4 evalCubicCurve(float4 b00, float4 b01, float4 b02, float4 b03, float u) {
        return (1-u) * (1-u) * (1-u) *     b00 +
                        3    * (1-u) * (1-u) * u * b01 +
                        3    * (1-u) *    u  * u * b02 +
                        u    *     u *    u  *     b03;
}

float4 evalCubicNormal(float4 b00, float4 b01, float4 b02, float4 b03, float v){
    return (1-v)*(v-1)*b00+(3*v-1)*(v-1)*b01+v*(2-3*v)*b02+v*v*b03;
}

float4 distanceConstraint(float4 pos, float4 dir, float curdis, float initdis, float k, float delta){
    return (float4)(pos.xyz+(curdis-initdis)*delta*k*dir.xyz, 1.0);;
}


__kernel void clbpatch(__global float4* position, __global float4* norm, __global float2* coord, __global uint* indices, __global float4* ctlposes, float offsetu, float offsetv, int xspan, int width, int vertnum, __global float4* diffnormbuf)
{
    int g=get_global_id(0);
    __local float4 ctl_points[16];

    int groupid=g>>4;
    int groupleft=g-(groupid<<4);

    ctl_points[groupleft]=ctlposes[groupleft];
    barrier(CLK_LOCAL_MEM_FENCE);


    int yid=groupid/xspan;
    int xid=groupid-yid*xspan;
    int y_offset_in_G=groupleft>>2;
    int x_offset_in_G=groupleft-(y_offset_in_G<<2);
    yid=yid*3+y_offset_in_G;
    xid=xid*3+x_offset_in_G;
    int id=yid*width+xid;

    float2 uv=(float2)(yid*offsetu, xid*offsetv);

    float4 b00 = evalCubicCurve(ctl_points[0], ctl_points[1], ctl_points[2],ctl_points[3], uv.x);
    float4 b01 = evalCubicCurve(ctl_points[4], ctl_points[5], ctl_points[6],ctl_points[7], uv.x);
    float4 b02 = evalCubicCurve(ctl_points[8], ctl_points[9], ctl_points[10],ctl_points[11], uv.x);
    float4 b03 = evalCubicCurve(ctl_points[12], ctl_points[13], ctl_points[14],ctl_points[15], uv.x);

    float4 b10 = evalCubicNormal(ctl_points[0], ctl_points[1], ctl_points[2],ctl_points[3], uv.x);
    float4 b20 = evalCubicNormal(ctl_points[4], ctl_points[5], ctl_points[6],ctl_points[7], uv.x);
    float4 b30 = evalCubicNormal(ctl_points[8], ctl_points[9], ctl_points[10],ctl_points[11], uv.x);
    float4 b40 = evalCubicNormal(ctl_points[12], ctl_points[13], ctl_points[14],ctl_points[15], uv.x);

    float4 cur_p=evalCubicCurve(b00, b01, b02, b03, uv.y);
    float4 cur_n=evalCubicNormal(b00, b01, b02, b03, uv.y);
    float4 cur_bn =evalCubicCurve(b10, b20, b30, b40, uv.y);
    float4 cur_dn = cross(cur_n, cur_bn);

    position[id]=cur_p;
    position[id+vertnum]=cur_p;

    norm[id]=cur_n;
    norm[id+vertnum]=cur_n;
    norm[id+vertnum*2]=cur_n;

    coord[id]=uv;
    coord[id+vertnum]=uv;
    coord[id+vertnum*2]=uv;

    diffnormbuf[id]=cur_dn;
    diffnormbuf[id+vertnum]=cur_dn;
    diffnormbuf[id+vertnum*2]=cur_dn;


    if(x_offset_in_G==3||y_offset_in_G==0){
    }else{
        int tri_id=(yid-1)*((width-1)<<1)+(xid<<1);
        indices[tri_id*3]=id;
        indices[tri_id*3+1]=id-width;
        indices[tri_id*3+2]=id+1;
    }

    if(x_offset_in_G==0||y_offset_in_G==3){
    }else{
        int tri_id=yid*((width-1)<<1)+((xid-1)<<1)+1;
        indices[tri_id*3]=id;
        indices[tri_id*3+1]=id+width;
        indices[tri_id*3+2]=id-1;
    }
}

__kernel void caldis(__global float4* pos, __global float4* norm, __global float2* interdis, float vinterval){
    int g=get_global_id(0);
    __local float4 strandpos[17];

    int groupid=g>>4;
    int localid=g-(groupid<<4);

    float4 curpos=pos[g];
    if(localid==0){
        float4 rootpos=curpos-norm[g]*vinterval;
        rootpos.w=1;
        strandpos[0]=rootpos;
    }
    strandpos[localid+1]=curpos;
    barrier(CLK_LOCAL_MEM_FENCE);

    float2 dis=(float2)(0,0);

    if(localid>0){
        dis.x=length(curpos-strandpos[localid]);
        dis.y=length(curpos-strandpos[localid-1]);
    }

    interdis[g]=dis;
}

__kernel void simulate(__global float4* pos, __global float4* prepos,__global float4* norm, __global float2* interdis, float vinterval, uint time, float timestep, int numiter, float kstreching, float kbending, float dp, __global float2* uv, float4 windscaler){
    if(timestep>0.06f||timestep<0.01f){
        timestep=0.02f;
    }
    int g=get_global_id(0);
    int worksize=get_global_size(0);
    __local float4 strandpos[17];
    __local float2 dis[16];

    int groupid=g>>4;
    int localid=g-(groupid<<4);
    float4 gravity=(float4)(0,-9.8,0,0);
    float4 cur_pos=pos[g];

    float4 pre_pos=prepos[g];
    float4 velocity;
    if(timestep<0.0001)
        velocity=(float4)(0,0,0,0);
    else velocity=(cur_pos-pre_pos)/timestep;
    dis[localid]=interdis[g];

    float4 wind=(float4)(8,1,8,0);
    wind*=windscaler;

    if(localid==0){
        float4 rootpos=cur_pos-norm[g]*vinterval;
        rootpos.w=1;
        strandpos[0]=rootpos;
        strandpos[1]=cur_pos;
    }else{

        //calculate wind here;
        float factor=length(cross(normalize(wind), normalize(norm[g])));

        float turbulence=turbulence3d(cur_pos+velocity*timestep, 100000000.0f, 20.0f, time*2.0f, 4.0f);
        float turbulence1=turbulence3d(cur_pos+velocity*timestep, 1000.0f, 10.0f, time, 4.0f);
        float turbulence2=turbulence3d(cur_pos+velocity*timestep, 10000.0f, 10.0f, time*0.2f,4.0f);
        float4 turbvec=(float4)(turbulence, turbulence1, turbulence2,0);

       /*float4 refpos=pos[g+worksize];
        float4 refdir=refpos-cur_pos;
        float dirstrength=length(refdir);
        refdir/=dirstrength;
        float delta=uv[g].y;*/

        /*float cur_v_sub=dot(refdir,velocity);
        float forcestrength=2.0f*(dirstrength-cur_v_sub*timestep)/pow(timestep, 2.0f);
        forcestrength*=(1-pow(delta,2.0f));
        refdir*=forcestrength;*/

        /*if(timestep*(+0.5f*forcestrength)*timestep>dirstrength){
            refdir=-gravity;
            cur_pos=refpos;
        }else{
            refdir*=forcestrength;
        }*/

        /*if(dirstrength<0.01*(1.2-delta)){
            refdir=-gravity;
            cur_pos=refpos;
        }else{
            refdir/=dirstrength;
            if(timestep<0.0001f) timestep=0.0001f;
            float vneeded=dirstrength/timestep;
           // float curvd=fabs(dot(velocity, refdir));
            //vneeded-=curvd;
            if(vneeded>20) vneeded=20;
            refdir*=vneeded*(1.0f-delta*delta);
        }*/

        float4 drag = dp*velocity*length(velocity);
        float4 accel=gravity+factor*wind*turbvec-drag;
        //accel-=drag;
        strandpos[localid+1]=2*cur_pos-pre_pos+accel*timestep*timestep;
    }

    float4 refpos=pos[g+worksize];
    float4 refdir=refpos-cur_pos;
    //float dirstrength=length(refdir);
    //refdir/=dirstrength;
    float delta=uv[g].y;
    //float bendingfactor=kbending*3/(localid+1);
    kbending*=pow((1.02f-delta),3.0f);//2*pow(delta,3.0f)-3*pow(delta, 2.0f)+1;
    strandpos[localid+1]+=kbending*refdir;

    for(int i=0; i<numiter; ++i){
        barrier(CLK_LOCAL_MEM_FENCE);

        float4 curdir=strandpos[localid]-strandpos[localid+1];
        float curdis=length(curdir);
        curdir/=curdis;
        if(localid!=0&&localid%2==0){
            float alpha=(curdis-dis[localid].x)*0.5*kstreching;
            strandpos[localid+1]+=curdir*alpha;
            strandpos[localid]-=curdir*alpha;
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        if(localid%2==1){
            if(localid==1){
                strandpos[2]+=(curdis-dis[localid].x)*kstreching*curdir;
            }else{
                float alpha=(curdis-dis[localid].x)*0.5*kstreching;
                strandpos[localid+1]+=curdir*alpha;
                strandpos[localid]-=curdir*alpha;
            }
        }
    }


    pos[g]=strandpos[localid+1];
    prepos[g]=cur_pos;
}

__kernel void shaper(__global float4* pos, __global uint4* feedback, float hstep, uint splitstart){
    int g=get_global_id(0);
    int groupid=g>>4;
    int localid=g-(groupid<<4);
    float4 curpos=pos[g];

    if(groupid==0||groupid==15){
        feedback[g]=(uint4)(0,0,0,0);
    }else{
        __local uint tags[16];
        tags[localid]=0;
        if(localid>splitstart){
            float disleft=length(curpos-pos[g-16]);
            float disright=length(curpos-pos[g+16]);
            if(disleft>hstep||disright>hstep){
                tags[localid]=1;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        for(int i=splitstart;i<=localid;++i){
            if(tags[i]>0){
                tags[localid]=15-i;
                break;
            }
        }
        feedback[g]=(uint4)(tags[localid],localid,groupid,0);
        feedback[g+512]=(uint4)(tags[localid],localid,groupid,1);
    }
}

__kernel void hairmaker(__global float4* pos, __global uint4* feedback, __global uint* indices){
    int g=get_global_id(0);
    int groupid=g>>4;
    int localid=g-(groupid<<4);
    int worksize=get_global_size(0);


/*    pos[g+worksize*2]=pos[g];
    int tri_idx=groupid*30+(localid<<1);
    indices[tri_idx*3]=g+worksize*2;
    indices[tri_idx*3+1]=g+17;
    indices[tri_idx*3+2]=g+16;*/

    if(groupid<15&&localid<15){
        uint fb_lt=feedback[g].x;
        uint fb_lb=feedback[g+1].x;
        uint fb_rt=feedback[g+16].x;
        uint fb_rb=feedback[g+17].x;

        uint idx_lt, idx_lb;
        if(fb_lt>0) idx_lt=g+worksize*2;
        else idx_lt=g;
        if(fb_lb>0) idx_lb=g+1+worksize*2;
        else idx_lb=g+1;
        uint idx_rt=g+16;
        uint idx_rb=g+17;

        if(fb_lt>0){
            float start_delta= 0.333f*(localid+fb_lt-15.0f)/fb_lt;
            pos[idx_lt]=pos[g]*(1-start_delta)+start_delta*pos[g+16];
        }

        if(fb_lb>0){
            float start_delta=0.333f*(localid+fb_lb-14.0f)/fb_lb;
            pos[idx_lb]=pos[g+1]*(1-start_delta)+start_delta*pos[g+17];
        }

        int tri_idx=groupid*30+(localid<<1);
        indices[tri_idx*3]=idx_lt;
        indices[tri_idx*3+1]=idx_lb;
        indices[tri_idx*3+2]=idx_rb;

        tri_idx++;
        indices[tri_idx*3]=idx_lt;
        indices[tri_idx*3+1]=idx_rb;
        indices[tri_idx*3+2]=idx_rt;
    }
}
