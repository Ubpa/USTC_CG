#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.WorldPos = worldPos.xyz / worldPos.w;
	
    vs_out.TexCoord = aTexCoord;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = mat3(model) * aTangent;
    T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * worldPos;
}
