#version 330 core
out vec4 FragColor;


#define PI 3.1415926
#define EPSILON .000001



uniform sampler2D shadowmap;
uniform sampler2D diffuseTexture;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;
uniform vec3 point_light_radiance;
// TODO: HW8 - 2_Shadow | uniforms
// add uniforms for mapping position in world space to position in shadowmap space
uniform bool have_shadow;
uniform mat4 light_space_matrix;
uniform float near_plane;
uniform float far_plane;

uniform vec3 ambient_irradiance;
uniform float roughness;
uniform float metalness;



in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}fs_in;


vec3 fresnel(vec3 albedo,float metalness,float cos_theta){
    float reflectance=.04;
    vec3 F0=mix(vec3(reflectance),albedo,metalness);
    float x=1-cos_theta;
    float x2=x*x;
    float x5=x2*x2*x;
    return F0+(1-F0)*x5;
}

float GGX_G(float alpha,vec3 L,vec3 V,vec3 N){
    float alpha2=alpha*alpha;
    
    float cos_sthetai=dot(L,N);
    float cos_sthetao=dot(V,N);
    
    float tan2_sthetai=1/(cos_sthetai*cos_sthetai)-1;
    float tan2_sthetao=1/(cos_sthetao*cos_sthetao)-1;
    
    return step(cos_sthetai,0)*step(cos_sthetai,0)*2/(sqrt(1+alpha2*tan2_sthetai)+sqrt(1+alpha2*tan2_sthetao));
}

float GGX_D(float alpha,vec3 N,vec3 H){
    float alpha2=alpha*alpha;
    float cos_stheta=dot(H,N);
    float x=1+(alpha2-1)*cos_stheta*cos_stheta;
    float denominator=PI*x*x;
    return step(cos_stheta,0)*alpha2/denominator;
}


// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z=depth*2.-1.;// Back to NDC
    return(2.*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}

float ShadowCalculation(vec4 fragPosLightSpace){
    // perform perspective divide
    vec3 projCoords=fragPosLightSpace.xyz/fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords=projCoords*.5+.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // float closestDepth=texture(shadowMap,projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth=projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal=normalize(fs_in.Normal);
    vec3 lightDir=normalize(point_light_pos-fs_in.FragPos);
    float bias=max(0.06*(1.-dot(normal,lightDir)),0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow=0.;
    vec2 texelSize=1./textureSize(shadowmap,0);
    for(int x=-1;x<=1;++x)
    {
        for(int y=-1;y<=1;++y)
        {
            float closestDepth=texture(shadowmap,projCoords.xy+vec2(x,y)*texelSize).r;
            float pcfDepth = LinearizeDepth(closestDepth);
            pcfDepth = closestDepth;
            shadow+=currentDepth-bias>pcfDepth?1.:0.;
        }
    }
    shadow/=9.;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z>1.)
    shadow=0.;
    
    return shadow;
}
void main()
{   
    vec3 albedo=texture(diffuseTexture,fs_in.TexCoords).rgb;
    float alpha=roughness*roughness;

    vec3 V=normalize(camera_pos-fs_in.FragPos);
    vec3 N=normalize(fs_in.Normal);
    vec3 fragTolight=point_light_pos-fs_in.FragPos;// frag to light
    float dist2=dot(fragTolight,fragTolight);
    float dist=sqrt(dist2);
    vec3 L=fragTolight/dist;// normalized
    vec3 H=normalize(L+V);

    float cos_theta=dot(N,L);

    vec3 fr=fresnel(albedo,metalness,cos_theta);
    float D=GGX_D(alpha,N,H);
    float G=GGX_G(alpha,L,V,N);

    vec3 diffuse=(1-fr)*(1-metalness)*albedo/PI;

    vec3 specular=fr*D*G/(4*max(dot(L,N)*dot(V,N),EPSILON));

    vec3 brdf=diffuse+specular;
    // TODO: HW8 - 2_Shadow | shadow
    float visible=1;
    if(have_shadow)
        visible=1-ShadowCalculation(fs_in.FragPosLightSpace);// if the fragment is in shadow, set it to 0
    else
        visible=1;

    vec3 Lo_direct=visible*brdf*point_light_radiance*max(cos_theta,0)/dist2;
    vec3 Lo_ambient=(1-metalness)*albedo/PI*ambient_irradiance;
    vec3 Lo=Lo_direct+Lo_ambient;

    FragColor=vec4(Lo,1);

}

