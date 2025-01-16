#version 330 core
struct Material {  
    sampler2D mainTexture;
    vec4 color;
};

out vec4 FragColor;

uniform Material material;

in vec2 TexCoord;

void main()
{
    vec4 texColor = texture(material.mainTexture, TexCoord);
    FragColor = texColor * material.color;
}
