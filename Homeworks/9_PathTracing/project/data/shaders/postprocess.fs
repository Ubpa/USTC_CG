#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D img;

void main()
{
	vec3 color = texture(img, TexCoord).rgb;
	color = sqrt(color);
	FragColor = vec4(color, 1);
}
