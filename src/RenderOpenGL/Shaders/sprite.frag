#version 330 core

struct SceneParameters
{
    mat4 viewProjection;
    vec3 viewPosition;
};

struct Material {  
    sampler2D mainTexture;
    vec4 color;
    vec2 textureOffset;
    vec2 textureScale;
    int flipXY;
};

out vec4 FragColor;

uniform Material material;
in vec2 TexCoord;

void main()
{
    vec4 texColor = texture(material.mainTexture, TexCoord);
    if(texColor.a == 0)
        discard;
    FragColor = texColor * material.color;
}
