#version 330 core

struct SceneParameters
{
    mat4 viewProjection;
};

struct RenderEntity
{
    mat4 model;
};

layout (location = 0) in vec3 aPos;

uniform SceneParameters sceneParameters;
uniform RenderEntity rentity;

void main()
{
    // gl_Position = sceneParameters.projection * sceneParameters.view * rentity.model * vec4(aPos, 1.0);
    gl_Position = sceneParameters.viewProjection * rentity.model * vec4(aPos, 1.0);
}
