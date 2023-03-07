#version 330 core

#define PI 3.1415926

out vec3 FragColor;

in vec3 TexCoord;

uniform vec3 EnvLight_color;
uniform float EnvLight_intensity;
uniform sampler2D EnvLight_texture;

void main()
{
    vec3 nTexCoord = normalize(TexCoord);
	float u = (atan(-nTexCoord.x, -nTexCoord.z) / PI + 1) / 2;
	float v = acos(nTexCoord.y) / PI; // reverse: 1 - acos(nTexCoord.y) / PI;
	vec3 env = EnvLight_color * EnvLight_intensity * texture(EnvLight_texture, vec2(u, v)).rgb;
	FragColor = env;
}
