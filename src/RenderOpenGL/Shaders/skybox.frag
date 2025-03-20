out vec4 FragColor;

in vec3 SkyDir;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, SkyDir);
}