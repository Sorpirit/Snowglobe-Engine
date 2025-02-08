#version 330 core

struct SceneParameters
{
    mat4 viewProjection;
    vec3 viewPosition;
};

struct LightParameters
{
    uint pointLightsCount;
    uint spotLightsCount;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float ambientIntensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float ambientIntensity;
    vec3 attenuationCoefficients;
    float maxDistance;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float ambientIntensity;
    float innerCutoffAngle;
    float outerCutoffAngle;
    vec3 attenuationCoefficients;
    float maxDistance;
};


struct Material {  
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D emissiveTexture;
    vec4 baseColor;
    int specularPower;
    float emissivePower;
};

out vec4 FragColor;

uniform SceneParameters sceneParameters;
uniform Material material;

//Lighting
uniform LightParameters lightParameters;
uniform DirectionalLight directionalLight;
#define POINT_LIGHTS_COUNT 4
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
#define SPOT_LIGHTS_COUNT 4
uniform SpotLight spotLights[SPOT_LIGHTS_COUNT];

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

vec3 LightingPass(vec3 normal, vec3 viewDir)
{
    vec3 fragColor = vec3(0.0);

    vec3 diffuseColor = (material.baseColor * texture(material.diffuseTexture, TexCoord)).rgb;
    vec3 specularColor = texture(material.specularTexture, TexCoord).rgb;

    // Directional
    {
        //Diffuse
        vec3 lightDir = normalize(-directionalLight.direction);
        float lambert = clamp(dot(normal, lightDir), 0.0, 1.0);
        vec3 diffuseLight = lambert * directionalLight.color;

        //Specular
        // vec3 reflectDir = reflect(-lightDir, normal); // Phong
        vec3 halfDir = normalize(lightDir + viewDir); // Blinn-Phong
        float specular = max(dot(halfDir, normal), 0.0) * ((lambert > 0) ? 1 : 0);
        specular = pow(specular, material.specularPower);
        vec3 specularLight = specular * directionalLight.color;

        //Combine
        vec3 ambientLight = directionalLight.ambientIntensity * directionalLight.color;
        fragColor += vec3((ambientLight + diffuseLight) * diffuseColor + specularLight * specularColor);
    }

    // Point
    for (uint i = 0u; i < lightParameters.pointLightsCount; i++)
    {
        vec3 lightDistance = pointLights[i].position - FragPos;
        float distance = length(lightDistance);
        if(distance > pointLights[i].maxDistance)
            continue;

        float attenuation = 1.0 / (pointLights[i].attenuationCoefficients.x + pointLights[i].attenuationCoefficients.y * distance + pointLights[i].attenuationCoefficients.z * distance * distance);

        vec3 lightDir = normalize(lightDistance);
        float lambert = clamp(dot(normal, lightDir), 0.0, 1.0);
        
        // vec3 reflectDir = reflect(-lightDir, normal); // Phong
        vec3 halfDir = normalize(lightDir + viewDir); // Blinn-Phong
        float specular = pow(max(dot(halfDir, normal), 0.0) * ((lambert > 0) ? 1 : 0), material.specularPower);
        
        vec3 ambientLight = pointLights[i].ambientIntensity * pointLights[i].color;
        vec3 diffuseLight = lambert * pointLights[i].color;
        vec3 specularLight = specular * pointLights[i].color;
        fragColor += vec3((ambientLight + diffuseLight) * diffuseColor + specularLight * specularColor) * attenuation;
    }

    // Spot
    for (uint i = 0u; i < lightParameters.spotLightsCount; i++)
    {
        vec3 lightDistance = spotLights[i].position - FragPos;
        float distance = length(lightDistance);
        if(distance > spotLights[i].maxDistance)
            continue;

        float attenuation = 1.0 / (spotLights[i].attenuationCoefficients.x + spotLights[i].attenuationCoefficients.y * distance + spotLights[i].attenuationCoefficients.z * distance * distance);
        vec3 ambientLight = spotLights[i].ambientIntensity * spotLights[i].color;
        
        vec3 lightDir = normalize(lightDistance);
        float theta = dot(lightDir, normalize(-spotLights[i].direction));

        float innerCos = cos(spotLights[i].innerCutoffAngle);
        float outerCos = cos(spotLights[i].outerCutoffAngle);
        float epsilon = innerCos - outerCos;
        float intensity = clamp((theta - outerCos) / epsilon, 0.0, 1.0);
        if(intensity > 0.001)
        {
            float lambert = clamp(dot(normal, lightDir), 0.0, 1.0);
            
            // vec3 reflectDir = reflect(-lightDir, normal); // Phong
            vec3 halfDir = normalize(lightDir + viewDir); // Blinn-Phong
            float specular = pow(max(dot(halfDir, normal), 0.0) * ((lambert > 0) ? 1 : 0), material.specularPower);
            
            vec3 diffuseLight = lambert * spotLights[i].color;
            vec3 specularLight = specular * spotLights[i].color;
            fragColor += attenuation * (ambientLight * diffuseColor + (diffuseLight * diffuseColor + specularLight * specularColor) * intensity);
        }
        else    
            fragColor += attenuation * ambientLight * diffuseColor;
    }

    return fragColor;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(sceneParameters.viewPosition - FragPos);
    vec3 litColor = LightingPass(normal, viewDir);
    
    vec4 emissiveColor = texture(material.emissiveTexture, TexCoord);
    FragColor = emissiveColor + vec4(litColor, 1.0);
}
