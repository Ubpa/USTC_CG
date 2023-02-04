#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    vec3 Normal;
    vec4 WorldPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_space_matrix;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
	
	vs_out.WorldPos = worldPos.xyz / worldPos.w;
    vs_out.TexCoord = aTexCoord;
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * aNormal);
	vs_out.WorldPosLightSpace = light_space_matrix * worldPos;
    gl_Position = projection * view * worldPos;
}
