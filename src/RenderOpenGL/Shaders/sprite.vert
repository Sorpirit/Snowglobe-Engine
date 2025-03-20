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

#define FLIP_X 1
#define FLIP_Y 2

struct Material {  
    sampler2D mainTexture;
    vec4 color;
    vec2 textureOffset;
    vec2 textureScale;
    int flipXY;
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
    vec2 uv = aTexCoord;
    if((material.flipXY & FLIP_X) > 0)
        uv.x = 1 - uv.x;

    if((material.flipXY & FLIP_Y) > 0)
        uv.y = 1 - uv.y;

    TexCoord = (material.textureOffset + uv) * material.textureScale;
}
