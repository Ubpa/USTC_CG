#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D img;

void main()
{
	FragColor = texture(img, TexCoord);
}
