#version 330 core

struct SceneParameters
{
    mat4 viewProjection;
    vec3 viewPosition;
};

struct RenderEntity
{
    mat4 model;
    mat3 normalMatrix;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform SceneParameters sceneParameters;
uniform RenderEntity rentity;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    // gl_Position = sceneParameters.projection * sceneParameters.view * rentity.model * vec4(aPos, 1.0);
    vec4 modelPosition = rentity.model * vec4(aPos, 1.0);
    
    FragPos = vec3(modelPosition) / modelPosition.w;
    TexCoord = aTexCoord;
    Normal = rentity.normalMatrix * aNormal;

    gl_Position = sceneParameters.viewProjection * modelPosition;
}
