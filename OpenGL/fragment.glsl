#version 330 core

out vec4 FragColor;

uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;
in vec2 Tex;
in vec4 fragPosLightSpace;

uniform sampler2D depthMap;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec4 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

const int MAX_LIGHT_NUM = 12;
uniform int spotlightCount = 0;
uniform int dirlightCount = 0;
uniform int pointlightCount = 0;
uniform SpotLight spotlight[MAX_LIGHT_NUM];
uniform DirLight dirlight[MAX_LIGHT_NUM];
uniform PointLight pointlight[MAX_LIGHT_NUM];

float ShadowCalculation(vec4 fragPosLightSpace,vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.x>1.0 || projCoords.y>1.0) return 0.0;

    float bias = max(0.0003 * (1.0 - dot(normal, lightDir)), 0.00003);
    float shadow;
    float pcfDepth;
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);

    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }

    shadow /= 25;
    return shadow;
}

vec3 calcPointLight(PointLight light){
//    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0,Tex));

    vec3 lightDir = normalize(vec3(light.position) - fragPos);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse0,Tex));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 1);
    vec3 specular = spec * light.specular * texture(material.texture_specular0,Tex).r;

    float dist = length(vec3(light.position) - fragPos);
    float attenuation = 1.0 / (1.0 + 0.07 * dist + 
  			     0.017 * (dist * dist)); 

    return ((diffuse*attenuation) + (specular*attenuation));
}

vec3 calcDirLight(DirLight light){

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse0,Tex));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 1);
    vec3 specular = spec * light.specular *  vec3(texture(material.texture_specular0,Tex));

    return (diffuse + specular);
}

vec3 calcSpotLight(SpotLight light){
    vec3 result = vec3(0.0);
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse,Tex));
//    result += ambient;
    vec3 lightDir = normalize(vec3(light.position) - fragPos);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    if(light.outerCutOff<theta){
        float diff = max(dot(normal,lightDir),0.0);
        diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse0,Tex));
        
        vec3 viewDir = normalize(viewPos-fragPos);
        vec3 reflectDir = reflect(-lightDir,normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 1);
        specular = spec * light.specular * texture(material.texture_specular0,Tex).r;

        float dist = length(vec3(light.position) - fragPos);
        float attenuation = 1.0 / (1.0 + 0.0007 * dist + 
  			      0.00002 * (dist * dist));
        float shadow = ShadowCalculation(fragPosLightSpace,lightDir);
        result += (1.0 - shadow) * (diffuse + specular.r ) * intensity * attenuation;
    }
    return result;
}

void main()
{   
    vec3 result = spotlight[0].ambient * vec3(texture(material.texture_diffuse0,Tex)); //ambient
    //    result += calcDirLight(dirlight);

    for(int i = 0; i<pointlightCount; i++){
        result += calcPointLight(pointlight[i]);
    }

    for(int i = 0; i<spotlightCount; i++){
        result += calcSpotLight(spotlight[i]);
    }

    FragColor = vec4(result, 1.0);
};
