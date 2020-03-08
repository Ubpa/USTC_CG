#version 330 core

#include "../../Math/sample.h"
#include "../BRDF/FDG.h"
#include "../../Math/quat.h"

out vec3 FragColor;
in vec2 TexCoords;

// ----------------------------------------------------------------------------
vec3 IntegrateBRDF(float NdotV, float roughness)
{
	// TODO
	return vec3(1,1,1);
}
// ----------------------------------------------------------------------------
void main() 
{
	FragColor = IntegrateBRDF(TexCoords.x, TexCoords.y);
}
