#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out;

in vec4 tc[3];
in vec3 linepos[3];

out vec2 coord;

uniform vec3 cam_pos;
uniform mat4 mvp;

void main()
{
       // vec2 win_scale=vec2(200, 200);
        float line_width=0.02;
        float line_offset=0.01;
        int edge_start=-1, edge_end;
        for(int i=0;i<3;++i){
            int ee_index=(i+1)%3;
            if((tc[i].z==tc[ee_index].z)&&(tc[i].w==tc[ee_index].w)&&tc[i].x>0&&tc[ee_index].x>0){
                edge_start=i;
                edge_end=ee_index;
                break;
            }
        }

        if(edge_start<0||edge_end<0||tc[edge_start].x<=2) return;
	
        vec3 line_dir=normalize(linepos[edge_end]-linepos[edge_start]);

        vec3 eye_dir_start=normalize(cam_pos-linepos[edge_start]);
        vec3 line_start_right=normalize(cross(eye_dir_start, line_dir));
        vec3 eye_dir_end=normalize(cam_pos-linepos[edge_end]);
        vec3 line_end_right=normalize(cross(eye_dir_end, line_dir));

        vec3 start_line_pos=linepos[edge_start]+eye_dir_start*line_offset;
        vec3 end_line_pos=linepos[edge_end]+eye_dir_end*line_offset;

        vec3 poslt=start_line_pos-line_width*line_start_right;
        vec3 poslb=end_line_pos-line_width*line_end_right;

        vec3 posrt=start_line_pos+line_width*line_start_right;
        vec3 posrb=end_line_pos+line_width*line_end_right;

        float vinterval=1.0f/tc[edge_start].x;
        float curv=(tc[edge_start].x+tc[edge_start].y-15)*vinterval;

        float curdir=tc[edge_end].y-tc[edge_start].y;

        gl_Position=mvp*vec4(poslt,1);
        coord=vec2(0,curv);
	EmitVertex();
	
        gl_Position=mvp*vec4(poslb,1);
        coord=vec2(0,curv+vinterval*curdir);
	EmitVertex();
	
        gl_Position=mvp*vec4(posrb,1);
        coord=vec2(1,curv+vinterval*curdir);
	EmitVertex();
	
	EndPrimitive();

        gl_Position=mvp*vec4(poslt,1);
        coord=vec2(0, curv);
        EmitVertex();

        gl_Position=mvp*vec4(posrb,1);
        coord=vec2(1, curv+vinterval*curdir);
        EmitVertex();

        gl_Position=mvp*vec4(posrt,1);
        coord=vec2(1,curv);
        EmitVertex();

        EndPrimitive();
}
