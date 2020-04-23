#version 330 core

layout (location = 0) out vec4 GBuffer0;
layout (location = 1) out vec4 GBuffer1;
layout (location = 2) out vec4 GBuffer2;
layout (location = 3) out vec4 GBuffer3;

// layout : light
//      x   y   z   w
// 0    [radiance]  0
// 1    0   0   0   0
// 2    0   0   0  1.5
// 3    0   0   0   0

in VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

uniform vec3 radiance_factor;
uniform sampler2D radiance_texture;

void main()
{
	vec3 radiance = radiance_factor * texture(radiance_texture, vs_out.TexCoord).rgb;
	
	GBuffer0 = vec4(radiance, 0);
	GBuffer1 = vec4(0, 0, 0, 0);
	GBuffer2 = vec4(0, 0, 0, 1.5);
	GBuffer3 = vec4(0, 0, 0, 0);
}
