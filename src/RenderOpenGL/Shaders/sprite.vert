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

struct Material {  
    sampler2D mainTexture;
    vec4 color;
    vec2 textureOffset;
    vec2 textureScale;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform SceneParameters sceneParameters;
uniform RenderEntity rentity;
uniform Material material;

out vec2 TexCoord;

void main()
{
    gl_Position = sceneParameters.viewProjection * rentity.model * vec4(aPos, 1.0);
    TexCoord = (material.textureOffset + aTexCoord) * material.textureScale;
}
