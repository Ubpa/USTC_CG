#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthmap;
uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z=depth*2.-1.;// Back to NDC
    return(2.*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}

void main()
{
    float depthValue=texture(depthmap,TexCoords).r;
    FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    //FragColor=vec4(vec3(depthValue),1.);// orthographic
}