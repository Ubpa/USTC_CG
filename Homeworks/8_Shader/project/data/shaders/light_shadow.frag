#version 330 core

#define PI 3.1415926
#define EPSILON 0.000001

out vec4 FragColor;

uniform vec3 point_light_pos;
uniform vec3 point_light_radiance;
uniform sampler2D shadowmap;
uniform bool have_shadow;
// TODO: HW8 - 2_Shadow | uniforms
// add uniforms for mapping position in world space to position in shadowmap space

uniform vec3 ambient_irradiance;
uniform sampler2D albedo_texture;
uniform float roughness;
uniform float metalness;

uniform vec3 camera_pos;

in VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    vec3 Normal;
} vs_out;

vec3 fresnel(vec3 albedo, float metalness, float cos_theta) {
	float reflectance = 0.04;
	vec3 F0 = mix(vec3(reflectance), albedo, metalness);
	float x = 1 - cos_theta;
	float x2 = x * x;
	float x5 = x2 * x2 * x;
	return F0 + (1-F0)*x5;
}

float GGX_G(float alpha, vec3 L, vec3 V, vec3 N) {
	float alpha2 = alpha * alpha;
	
	float cos_sthetai = dot(L, N);
	float cos_sthetao = dot(V, N);
	
	float tan2_sthetai = 1 / (cos_sthetai * cos_sthetai) - 1;
	float tan2_sthetao = 1 / (cos_sthetao * cos_sthetao) - 1;
	
	return step(cos_sthetai, 0) * step(cos_sthetai, 0) * 2 / (sqrt(1 + alpha2*tan2_sthetai) + sqrt(1 + alpha2*tan2_sthetao));
}

float GGX_D(float alpha, vec3 N, vec3 H) {
	float alpha2 = alpha * alpha;
	float cos_stheta = dot(H, N);
	float x = 1 + (alpha2 - 1) * cos_stheta * cos_stheta;
	float denominator = PI * x * x;
	return step(cos_stheta, 0) * alpha2 / denominator;
}

void main() {
	vec3 albedo = texture(albedo_texture, vs_out.TexCoord).rgb;
	float alpha = roughness * roughness;
	
	vec3 V = normalize(camera_pos - vs_out.WorldPos);
	vec3 N = normalize(vs_out.Normal);
	vec3 fragTolight = point_light_pos - vs_out.WorldPos; // frag to light
	float dist2 = dot(fragTolight, fragTolight);
	float dist = sqrt(dist2);
	vec3 L = fragTolight / dist; // normalized
	vec3 H = normalize(L + V);
	
	float cos_theta = dot(N, L);
	
	vec3 fr = fresnel(albedo, metalness, cos_theta);
	float D = GGX_D(alpha, N, H);
	float G = GGX_G(alpha, L, V, N);
	
	vec3 diffuse = (1 - fr) * (1 - metalness) * albedo / PI;
	
	vec3 specular = fr * D * G / (4 * max(dot(L, N)*dot(V, N), EPSILON));
	
	vec3 brdf = diffuse + specular;
	// TODO: HW8 - 2_Shadow | shadow
	float visible = 1.0; // if the fragment is in shadow, set it to 0
	vec3 Lo_direct = visible * brdf * point_light_radiance * max(cos_theta, 0) / dist2;
	vec3 Lo_ambient = (1-metalness) * albedo / PI * ambient_irradiance;
	vec3 Lo = Lo_direct + Lo_ambient;
	
	FragColor = vec4(Lo, 1);
}
