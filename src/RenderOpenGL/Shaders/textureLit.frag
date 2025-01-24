#version 330 core

struct SceneParameters
{
    mat4 viewProjection;
    vec3 viewPosition;
};

struct Material {  
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    vec4 baseColor;
    int specularPower;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float ambientIntensity;
};

out vec4 FragColor;

uniform SceneParameters sceneParameters;
uniform Material material;
uniform PointLight pointLight;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

void main()
{
    float ambientStrength = pointLight.ambientIntensity;
    vec3 ambientLight = ambientStrength * pointLight.color;

    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(pointLight.position - FragPos);
    float lambert = clamp(dot(normal, lightDir), 0.0, 1.0);
    vec3 diffuseLight = lambert * pointLight.color;

    vec3 viewDir = normalize(sceneParameters.viewPosition - FragPos);

    // vec3 reflectDir = reflect(-lightDir, normal); // Phong
    vec3 halfDir = normalize(lightDir + viewDir); // Blinn-Phong
    float specular = max(dot(halfDir, normal), 0.0) * ((lambert > 0) ? 1 : 0);
    specular = pow(specular, material.specularPower);
    vec3 specularLight = specular * pointLight.color;

    vec4 diffuseColor = material.baseColor * texture(material.diffuseTexture, TexCoord);
    vec4 specularColor = texture(material.specularTexture, TexCoord);

    FragColor = vec4(diffuseLight + ambientLight, 1.0) * diffuseColor + vec4(specularLight, 1.0) * specularColor;
}
