#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoords;
layout(location=2) in vec3 aNormal;


out vec2 TexCoords;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 light_space_matrix;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.FragPos = worldPos.xyz/ worldPos.w;
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * aNormal);
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = light_space_matrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * worldPos;


    // vs_out.FragPos=vec3(model*vec4(aPos,1.));
    // vs_out.Normal=transpose(inverse(mat3(model)))*aNormal;
    // vs_out.TexCoords=aTexCoords;
    // vs_out.FragPosLightSpace=light_space_matrix*vec4(vs_out.FragPos,1.);
    // gl_Position=projection*view*model*vec4(aPos,1.);
}