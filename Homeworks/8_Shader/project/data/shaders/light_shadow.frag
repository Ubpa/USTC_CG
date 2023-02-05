#version 330 core

#define PI 3.1415926
#define EPSILON 0.000001

out vec4 FragColor;


uniform sampler2D diffuseTexture;
uniform sampler2D shadowmap;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;
uniform vec3 point_light_radiance;

uniform bool have_shadow;
// TODO: HW8 - 2_Shadow | uniforms
// add uniforms for mapping position in world space to position in shadowmap space
uniform mat4 light_space_matrix;
uniform float near_plane;
uniform float far_plane;

uniform vec3 ambient_irradiance;
uniform float roughness;
uniform float metalness;


in VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    vec3 Normal;
	vec4 WorldPosLightSpace;
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

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
	float z=depth*2.-1.;// Back to NDC
	return(2.*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}


float shadow_calculation(vec4 fragPosLightSpace){
	//透视投影
	// 当light_space_matrix使用正交投影矩阵，顶点w元素仍保持不变，所以这一步实际上毫无意义。
	// 可是，当使用透视投影矩阵的时候就是必须的了
	// 所以为了保证在两种投影矩阵下都有效就得留着这行。
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//转换到[0,1]范围
	projCoords = projCoords * 0.5 + 0.5;
	//获取深度值


	//获取当前片段的深度值
	float currentDepth = projCoords.z;
	//偏移量，防止阴影出现锯齿
	vec3 normal = normalize(vs_out.Normal);
	vec3 lightDir = normalize(point_light_pos - vs_out.WorldPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//比较当前片段的深度值和从深度贴图中采样的深度值
	// 柔光化：PCF
	float shadow = 0;
	vec2 texelSize = 1.0 / textureSize(shadowmap, 0);//单个纹理像素的大小
	for(int x = -1; x <= 1; ++x)
		for(int y = -1; y <= 1; ++y)
		{
			float closestDepth = texture(shadowmap, projCoords.xy + vec2(x, y) * texelSize).r;
			//float pcfDepth = LinearizeDepth(closestDepth);
			float pcfDepth = closestDepth;
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
		}
	shadow /= 9.0;

	//如果当前片段在光源后面，那么不进行阴影计算
	if(projCoords.z>1.)
		shadow = 0.;
	return shadow;
}

void main() {
	// vec3 albedo = texture(diffuseTexture, vs_out.TexCoord).rgb;
	// float alpha = roughness * roughness;
	
	// vec3 V = normalize(camera_pos - vs_out.WorldPos);
	// vec3 N = normalize(vs_out.Normal);
	// vec3 fragTolight = point_light_pos - vs_out.WorldPos; // frag to light
	// float dist2 = dot(fragTolight, fragTolight);
	// float dist = sqrt(dist2);
	// vec3 L = fragTolight / dist; // normalized
	// vec3 H = normalize(L + V);
	
	// float cos_theta = dot(N, L);
	
	// vec3 fr = fresnel(albedo, metalness, cos_theta);
	// float D = GGX_D(alpha, N, H);
	// float G = GGX_G(alpha, L, V, N);
	
	// vec3 diffuse = (1 - fr) * (1 - metalness) * albedo / PI;
	
	// vec3 specular = fr * D * G / (4 * max(dot(L, N)*dot(V, N), EPSILON));
	
	// vec3 brdf = diffuse + specular;
	// // TODO: HW8 - 2_Shadow | shadow
	// float visible;
	// if(have_shadow)
	// 	visible = 1 - shadow_calculation(vs_out.WorldPosLightSpace); // if the fragment is in shadow, set it to 0
	// else	
	// 	visible = 1;
	
	// vec3 Lo_direct = visible * brdf * point_light_radiance * max(cos_theta, 0) / dist2;
	// vec3 Lo_ambient = (1-metalness) * albedo / PI * ambient_irradiance;
	// vec3 Lo = Lo_direct + Lo_ambient;
	
	// FragColor = vec4(Lo, 1);
	//================================================================================================
	vec3 color=texture(diffuseTexture,vs_out.TexCoord).rgb;
	vec3 normal=normalize(vs_out.Normal);
	vec3 lightColor=vec3(.4);
	// Ambient
	vec3 ambient=.2*color;
	// Diffuse
	vec3 lightDir=normalize(point_light_pos-vs_out.WorldPos);
	float diff=max(dot(lightDir,normal),0.);
	vec3 diffuse=diff*lightColor;
	// Specular
	vec3 viewDir=normalize(camera_pos-vs_out.WorldPos);
	float spec=0.;
	vec3 halfwayDir=normalize(lightDir + viewDir);
	spec=pow(max(dot(normal,halfwayDir),0.),64.);
	vec3 specular=spec*lightColor;
	// Calculate shadow
	float shadow=have_shadow ? shadow_calculation(vs_out.WorldPosLightSpace):0.;
	shadow=min(shadow,.75);// reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
	vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;

	FragColor=vec4(lighting,1.f);
}
