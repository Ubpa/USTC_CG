#version 330 core

#define PI 3.1415926
#define EPSILON 0.000001

out vec3 FragColor;

in vec2 TexCoord;

// layout : stdBRDF
//      x   y   z   w
// 0    [albedo]  roughness
// 1    [normal]  metalness
// 2    [ pos  ]   0.5
// 3    0   0   0   0

// layout : light
//      x   y   z   w
// 0    [radiance]  0
// 1    0   0   0   0
// 2    0   0   0  1.5
// 3    0   0   0   0

uniform sampler2D gbuffer0;
uniform sampler2D gbuffer1;
uniform sampler2D gbuffer2;
uniform sampler2D gbuffer3;

uniform vec3 camera_pos;
uniform vec3 pointlight_pos;
uniform vec3 pointlight_radiance;

struct PointLight {
	vec3 position;
	vec3 radiance;
};

struct RectLight {
	vec3 position;
	vec3 dir;
	vec3 radiance;
	vec3 right;
	float width;
	float height;
};

#define MAX_POINT_LIGHT_NUM 8
#define MAX_RECT_LIGHT_NUM 8

uniform uint pointlight_num;
uniform uint rectlight_num;

uniform PointLight pointlights[MAX_POINT_LIGHT_NUM];
uniform RectLight rectlights[MAX_RECT_LIGHT_NUM];

vec3 MetalWorkflow_F0(vec3 albedo, float metalness) {
	float reflectance = 0.04;
	return mix(vec3(reflectance), albedo, metalness);
}

vec3 fresnel(vec3 F0, float cos_theta) {
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

// ========================== LTC ==========================

const float LUT_SIZE  = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS  = 0.5/LUT_SIZE;

// GGX m(0,0) m(2,0) m(0,2) m(2,2)
uniform sampler2D LTC_tsfm;
// GGX norm, fresnel, 0(unused), sphere
uniform sampler2D LTC_nf0s;

vec3 LTC_Spec(vec3 N, vec3 V, vec3 P, vec3 F0, float roughness, RectLight light);
vec3 LTC_Diffuse(vec3 N, vec3 V, vec3 P, float roughness, RectLight light);

void main() {
	vec4 data0 = texture(gbuffer0, TexCoord);
	vec4 data1 = texture(gbuffer1, TexCoord);
	vec4 data2 = texture(gbuffer2, TexCoord);
	vec4 data3 = texture(gbuffer3, TexCoord);
	
	vec3 Lo = vec3(0);
	
	float ID = data2.w;
	
	if(ID < 1.0) {// stdBRDF
		vec3 albedo = data0.xyz;
		float roughness = data0.w;
		vec3 N = data1.xyz;
		float metalness = data1.w;
		vec3 pos = data2.xyz;
		vec3 F0 = MetalWorkflow_F0(albedo, metalness);
		
		float alpha = roughness * roughness;
		vec3 V = normalize(camera_pos - pos); // frag to camera
		
		// point light
		for(uint i = 0u; i < pointlight_num; i++) {
			vec3 fragTolight = pointlights[i].position - pos; // frag to light
			float dist2 = dot(fragTolight, fragTolight);
			float dist = sqrt(dist2);
			vec3 L = fragTolight / dist; // normalized
			vec3 H = normalize(L + V);
			
			float cos_theta = dot(N, L);
			
			vec3 fr = fresnel(F0, cos_theta);
			float D = GGX_D(alpha, N, H);
			float G = GGX_G(alpha, L, V, N);
			
			vec3 diffuse = (1 - fr) * (1 - metalness) * albedo / PI;
			
			vec3 specular = fr * D * G / (4 * max(dot(L, N)*dot(V, N), EPSILON));
			
			vec3 brdf = diffuse + specular;
			Lo += brdf * pointlights[i].radiance * max(cos_theta, 0) / dist2;
		}
		
		for(uint i = 0u; i < rectlight_num; i++) {
			vec3 spec = LTC_Spec(N, V, pos, F0, roughness, rectlights[i]);
			vec3 diffuse = (1 - F0) * (1 - metalness) * albedo / PI * LTC_Diffuse(N, V, pos, roughness, rectlights[i]);
			Lo += diffuse + spec;
		}
	}
	else if ( ID < 2.0 ){ // light
		Lo = data0.xyz;
	}
	else { // error
		Lo = vec3(1, 0, 1);
	}
	
	FragColor = Lo;
}

// ================================= detail =================================

// p0 BL, p1 BR, p2 TR, p3 TL
void RectLight_Corners(RectLight light, out vec3 p0, out vec3 p1, out vec3 p2, out vec3 p3) {
	float a = light.width * 0.5;
	float b = light.height * 0.5;
	vec3 verticle = cross(light.dir, light.right);
	vec3 halfWidthVec = light.right * a;
	vec3 halfHeightVec = verticle * b;

	p0 = light.position - halfHeightVec - halfWidthVec;
	p1 = light.position - halfHeightVec + halfWidthVec;
	p2 = light.position + halfHeightVec + halfWidthVec;
	p3 = light.position + halfHeightVec - halfWidthVec;
}

vec2 _LTC_CorrectUV(vec2 uv) {
	return uv * LUT_SCALE + LUT_BIAS;
}

// _LTC_GetInvM_GGX, _LTC_GetNF_GGX
vec2 _LTC_GetUV(float roughness, float NoV) {
	vec2 uv = vec2(roughness, sqrt(1.0 - NoV));
	return _LTC_CorrectUV(uv);
}

// GGX inv M
mat3 _LTC_GetInvM_GGX(vec2 uv) {
	vec4 t1 = texture(LTC_tsfm, uv);
	return mat3(
		vec3(t1.x,    0, t1.y), // col0
		vec3(   0,    1,    0), // col1
		vec3(t1.z,    0, t1.w)  // col2
	);
}

// GGX norm, Fresnel
vec2 _LTC_GetNF_GGX(vec2 uv) {
	vec4 t2 = texture(LTC_nf0s, uv);
	return vec2(t2.x, t2.y);
}

float _LTC_FitSphereIntegral(float NoF, float lenF) {
	return max(0, (lenF * lenF + NoF) / (lenF + 1));
}

// projected solid angle of a spherical cap, clipped to the horizon
float _LTC_GetSphere(float NoF, float lenF) {
	vec2 uv = vec2(NoF * 0.5 + 0.5, lenF);
	uv = _LTC_CorrectUV(uv);

	return lenF * texture(LTC_nf0s, uv).w;
}

vec3 _LTC_IntegrateEdgeVec(vec3 v1, vec3 v2) {
    float x = dot(v1, v2);
    float y = abs(x);
	
	float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
    float b = 3.4175940 + (4.1616724 + y)*y;
    float v = a / b;
	
    float theta_sintheta = (x > 0.0) ? v : 0.5*inversesqrt(max(1.0 - x*x, 1e-7)) - v;
	
    return cross(v1, v2) * theta_sintheta;
}

float _LTC_IntegrateEdge(vec3 v1, vec3 v2) {
    return _LTC_IntegrateEdgeVec(v1, v2).z;
}

void _LTC_ClipQuadToHorizon(inout vec3 L[5], out int n) {
    // detect clipping config
    int config = 0;
    if (L[0].z > 0.0) config += 1;
    if (L[1].z > 0.0) config += 2;
    if (L[2].z > 0.0) config += 4;
    if (L[3].z > 0.0) config += 8;

    // clip
    n = 0;

    if (config == 0)
    {
        // clip all
    }
    else if (config == 1) // V1 clip V2 V3 V4
    {
        n = 3;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 2) // V2 clip V1 V3 V4
    {
        n = 3;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 3) // V1 V2 clip V3 V4
    {
        n = 4;
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 4) // V3 clip V1 V2 V4
    {
        n = 3;
        L[0] = -L[3].z * L[2] + L[2].z * L[3];
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
    }
    else if (config == 5) // V1 V3 clip V2 V4) impossible
    {
        n = 0;
    }
    else if (config == 6) // V2 V3 clip V1 V4
    {
        n = 4;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 7) // V1 V2 V3 clip V4
    {
        n = 5;
        L[4] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 8) // V4 clip V1 V2 V3
    {
        n = 3;
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[1] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] =  L[3];
    }
    else if (config == 9) // V1 V4 clip V2 V3
    {
        n = 4;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[2].z * L[3] + L[3].z * L[2];
    }
    else if (config == 10) // V2 V4 clip V1 V3) impossible
    {
        n = 0;
    }
    else if (config == 11) // V1 V2 V4 clip V3
    {
        n = 5;
        L[4] = L[3];
        L[3] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 12) // V3 V4 clip V1 V2
    {
        n = 4;
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
    }
    else if (config == 13) // V1 V3 V4 clip V2
    {
        n = 5;
        L[4] = L[3];
        L[3] = L[2];
        L[2] = -L[1].z * L[2] + L[2].z * L[1];
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
    }
    else if (config == 14) // V2 V3 V4 clip V1
    {
        n = 5;
        L[4] = -L[0].z * L[3] + L[3].z * L[0];
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
    }
    else if (config == 15) // V1 V2 V3 V4
    {
        n = 4;
    }

    if (n == 3)
        L[3] = L[0];
    if (n == 4)
        L[4] = L[0];
}

float _LTC_Evaluate(vec3 N, vec3 V, vec3 P, mat3 invM, RectLight light) {
	vec3 p0, p1, p2, p3;
	RectLight_Corners(light, p0, p1, p2, p3);
	
	// construct orthonormal basis around N
	vec3 T1, T2;
	T1 = normalize(V - N*dot(V, N));
	T2 = cross(N, T1);

	// rotate area light in (T1, T2, N) basis
	invM = invM * transpose(mat3(T1, T2, N));

	// polygon
	vec3 L[5];
	L[0] = invM * (p0 - P);
	L[1] = invM * (p1 - P);
	L[2] = invM * (p2 - P);
	L[3] = invM * (p3 - P);

	// integrate
	float sum = 0.0;

#if 0 // sphere approximation
	vec3 dir = p0.xyz - P;
	vec3 lightNormal = cross(p1 - p0, p3 - p0);
	bool behind = dot(dir, lightNormal) > 0.0;

	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);

	vec3 vsum = vec3(0.0);

	vsum += _LTC_IntegrateEdgeVec(L[1], L[0]);
	vsum += _LTC_IntegrateEdgeVec(L[2], L[1]);
	vsum += _LTC_IntegrateEdgeVec(L[3], L[2]);
	vsum += _LTC_IntegrateEdgeVec(L[0], L[3]);

	float len = length(vsum);
	float z = vsum.z/len;

	if (behind)
		z = -z;
	
	sum = _LTC_GetSphere(z, len);
	
	if (behind)
		sum = 0.0;
#else
	int n;
	_LTC_ClipQuadToHorizon(L, n);

	if (n == 0)
		return 0;
	// project onto sphere
	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);
	L[4] = normalize(L[4]);

	// integrate
	sum += _LTC_IntegrateEdge(L[1], L[0]);
    sum += _LTC_IntegrateEdge(L[2], L[1]);
    sum += _LTC_IntegrateEdge(L[3], L[2]);
    if (n >= 4)
        sum += _LTC_IntegrateEdge(L[4], L[3]);
    if (n == 5)
        sum += _LTC_IntegrateEdge(L[0], L[4]);

    sum = max(0.0, sum);
#endif

	return sum;
}

vec3 LTC_Spec(vec3 N, vec3 V, vec3 P, vec3 F0, float roughness, RectLight light) {
	float NoV = clamp(dot(N, V), 0, 1);
	vec2 uv = _LTC_GetUV(roughness, NoV);
	
	mat3 invM = _LTC_GetInvM_GGX(uv);
	vec2 nf = _LTC_GetNF_GGX(uv);
	
	vec3 Fr = F0 * nf.x + (1 - F0) * nf.y;
	vec3 L = light.radiance * Fr;
	
	float spec = _LTC_Evaluate(N, V, P, invM, light);
	return spec * L;
}

vec3 LTC_Diffuse(vec3 N, vec3 V, vec3 P, float roughness, RectLight light) {
	mat3 invM = mat3(1);
	
	float diffuse = _LTC_Evaluate(N, V, P, invM, light);
	return light.radiance * diffuse;
}

