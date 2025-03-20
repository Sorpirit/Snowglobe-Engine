struct SceneParameters
{
    mat4 viewProjection;
    vec3 viewPosition;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform SceneParameters sceneParameters;

out vec4 VertexColor;

void main()
{
    // gl_Position = sceneParameters.projection * sceneParameters.view * rentity.model * vec4(aPos, 1.0);
    gl_Position = sceneParameters.viewProjection * vec4(aPos, 1.0);
    VertexColor = aColor;
}
