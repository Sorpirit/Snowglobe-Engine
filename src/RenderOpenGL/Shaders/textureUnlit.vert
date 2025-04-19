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
layout (location = 1) in vec2 aTexCoord;

uniform SceneParameters sceneParameters;
uniform RenderEntity rentity;

out vec2 TexCoord;

void main()
{
    // gl_Position = sceneParameters.projection * sceneParameters.view * rentity.model * vec4(aPos, 1.0);
    gl_Position = sceneParameters.viewProjection * rentity.model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
