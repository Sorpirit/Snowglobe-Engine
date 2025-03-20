out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float pixelOffset = 1.0 / 300.0;

void PostProcessing()
{
    vec2 kernelPixelOffsets[9] = vec2[](
    vec2(-pixelOffset, pixelOffset),  vec2(0, pixelOffset),  vec2(pixelOffset, pixelOffset),
    vec2(-pixelOffset, 0),            vec2(0, 0),            vec2(pixelOffset, 0),
    vec2(-pixelOffset, -pixelOffset), vec2(0, -pixelOffset), vec2(pixelOffset, -pixelOffset)
    );

    // Sharpen kernel
    //    float kernel[9] = float[](
    //        -1, -1, -1,
    //        -1,  9, -1,
    //        -1, -1, -1
    //    );
    // Blur
    //    float kernel[9] = float[](
    //        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
    //        2.0 / 16.0, 8.0 / 16.0, 2.0 / 16.0,
    //        1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
    //    );
    //
    // Edge Detection
    float kernel[9] = float[](
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
    );

    vec3 samples[9];
    for(int i = 0; i < 9; i++)
    {
        samples[i] = texture(screenTexture, TexCoords + kernelPixelOffsets[i]).rgb;
    }

    vec3 result = vec3(0);
    for(int i = 0; i < 9; i++)
    {
        result += samples[i] * kernel[i];
    }
}

void main()
{
    FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
}