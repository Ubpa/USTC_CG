#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D img;

void main()
{
	const float gamma = 2.2;
	
	vec3 hdrColor = texture(img, TexCoord).rgb;

    // Reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
	
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}
