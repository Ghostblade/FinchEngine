#version 150

uniform mat4 mvp; // our matrix
in vec3 position;
in vec3 normal;
in vec4 texcoord;

out vec3 n;
out vec4 tc;

void main()
{
    // vertex shaders MUST write to gl_Position which is a vec4
    // the vec4 constructor (vec3, double) creates a vec4 using
    // the first three components of the vec3 and the double.
    //
    // this is where we make use of our vertices: the order in the buffer
    // doesn't actually matter so much (we can create one using them
    // individually like so: vec4(p[2], p[1], p[0], 1.0)).
    //
    // gl_Position wants x, y, z though, so we've used that for simplicity
    // the reason gl_Position is a vec4 is a Long Story.
    gl_Position = mvp * vec4(position, 1.0);
    tc = texcoord;
}
