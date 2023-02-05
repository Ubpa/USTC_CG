

#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform float near_plane;
uniform float far_plane;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool shadows;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z=depth*2.-1.;// Back to NDC
    return(2.*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
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
    vec3 lightDir=normalize(lightPos-fs_in.FragPos);
    float bias=max(.05*(1.-dot(normal,lightDir)),.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow=0.;
    vec2 texelSize=1./textureSize(shadowMap,0);
    for(int x=-1;x<=1;++x)
    {
        for(int y=-1;y<=1;++y)
        {
            float closestDepth=texture(shadowMap,projCoords.xy+vec2(x,y)*texelSize).r;
            float pcfDepth = LinearizeDepth(closestDepth)/far_plane;
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
    vec3 color=texture(diffuseTexture,fs_in.TexCoords).rgb;
    vec3 normal=normalize(fs_in.Normal);
    vec3 lightColor=vec3(.4);
    // Ambient
    vec3 ambient=.2*color;
    // Diffuse
    vec3 lightDir=normalize(lightPos-fs_in.FragPos);
    float diff=max(dot(lightDir,normal),0.);
    vec3 diffuse=diff*lightColor;
    // Specular
    vec3 viewDir=normalize(viewPos-fs_in.FragPos);
    float spec=0.;
    vec3 halfwayDir=normalize(lightDir+viewDir);
    spec=pow(max(dot(normal,halfwayDir),0.),64.);
    vec3 specular=spec*lightColor;
    // Calculate shadow
    float shadow=shadows?ShadowCalculation(fs_in.FragPosLightSpace):0.;
    shadow=min(shadow,.75);// reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
    
    FragColor=vec4(lighting,1.f);
}

