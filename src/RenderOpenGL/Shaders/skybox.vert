layout (location = 0) in vec2 aPos;

out vec3 SkyDir;

uniform mat4 invViewProjection;

const float epsilon = 0.000001;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 1 - epsilon, 1.0);

    vec4 near = invViewProjection * vec4(aPos, -1.0, 1.0);
    vec4 far = invViewProjection * vec4(aPos, 1.0, 1.0);
    SkyDir = far.xyz / far.w - near.xyz / near.w;
}