#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aPos;
	mat4 viewNoT = mat4(mat3(view));
    vec4 pos = projection * viewNoT * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
