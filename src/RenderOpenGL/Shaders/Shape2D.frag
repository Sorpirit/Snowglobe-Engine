struct Material {
    vec4 color;
};

out vec4 FragColor;

uniform Material material;

void main()
{
    FragColor = material.color;
}
