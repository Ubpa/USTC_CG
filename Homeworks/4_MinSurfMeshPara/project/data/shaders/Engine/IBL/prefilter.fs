#version 330 core

#include "../../Math/sample.h"
#include "../BRDF/FDG.h"

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;
uniform float resolution;

void main()
{
	// TODO
	vec3 prefilteredColor = vec3(1,1,1);

    FragColor = vec4(prefilteredColor, 1.0);
}
