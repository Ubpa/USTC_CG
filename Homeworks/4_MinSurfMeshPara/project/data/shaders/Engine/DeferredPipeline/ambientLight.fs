#version 330 core

#include "../BRDF/MetalWorkflow.h"
#include "../BRDF/Frostbite.h"
#include "../BRDF/Diffuse.h"

out vec3 FragColor;
in vec2 TexCoords;

#define MAX_REFLECTION_LOD 4

// 160
layout (std140) uniform Camera {
	mat4 view;			// 64	0	64
	mat4 projection;	// 64	64	64
	vec3 viewPos;		// 12	128	144
	float nearPlane;	// 4	144	148
	float farPlane;		// 4	148	152
	float fov;			// 4	152	156
	float ar;			// 4	156	160
};

// 32
layout (std140) uniform Environment {
	vec3 colorFactor;     // 12     0
	float intensity;      //  4    12
	bool haveSkybox;      //  4    16
	bool haveEnvironment; //  4    20
};

uniform sampler2D GBuffer0;
uniform sampler2D GBuffer1;
uniform sampler2D GBuffer2;
uniform sampler2D GBuffer3;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

vec3 EvaluateAmbient(int ID, vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao);

void main() {
	if( !haveEnvironment ) {
		FragColor = vec3(0, 0, 0);
		return;
	}
	
	// prepare
	vec4 data0 = texture(GBuffer0, TexCoords);
	vec4 data1 = texture(GBuffer1, TexCoords);
	vec4 data2 = texture(GBuffer2, TexCoords);
	vec4 data3 = texture(GBuffer3, TexCoords);
	
	int ID = int(data0.w);
	vec3 pos = data0.xyz;
	vec3 norm = data1.xyz;
	vec3 albedo = data2.xyz;
	float roughness = data1.w;
	float metallic = data2.w;
	float ao = data3.w;
	
	vec3 wo = normalize(viewPos - pos);
	
	vec3 ambient = EvaluateAmbient(ID, wo, norm, albedo, roughness, metallic, ao);
	
	FragColor = ambient;
}

vec3 EvaluateAmbient(int ID, vec3 wo, vec3 norm, vec3 albedo, float roughness, float metallic, float ao) {
	return vec3(0);
}
