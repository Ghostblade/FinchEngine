#version 150

in vec3 fragColor; // from vertex shader
in vec4 tc;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    // a fragment shader is run once per pixel to render.
    // this simple fact means that efficiency is key: you don't
    // want it to be slow.
    //
    // the exact calls will depend on the vertex shader output (gl_Position)
    // a fragment shader can also get other sorts of input from the vertex
    // shader's output (texture coordinates, light calculations, fun stuff)
    //
    // here, we're just returning a rgba vec4 (floating point).

    // the depth of the pixel in window space. we can't just use z because of
    // the effect of the perspective matrix.
    float depth = gl_FragCoord.z / gl_FragCoord.w;

    float fogDensity = smoothstep(0, 30, depth);
    if (gl_FrontFacing)
        color = mix(texture(textureSampler, tc.xy), vec4(0), fogDensity);
    else
        color = vec4(1, 1, 1, 1);

}
