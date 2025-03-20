#include "SkyboxRenderPass.hpp"

#include "Assets/AssetManager.hpp"
#include "FileSystem.hpp"
#include "OpenGLRenderSystem.hpp"
#include "ShaderCompiler.hpp"
#include "TextureAssetData.hpp"
#include "glad/gl.h"

#include <array>

namespace Snowglobe::RenderOpenGL
{

void SkyboxRenderPass::Init()
{
    //Init cube map
    glGenTextures(1, &_cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTexture);

    // std::array cubeMapFaces = {
    //     Core::SnowFileHandle("Textures/skybox/water/skybox_right.jpg"), Core::SnowFileHandle("Textures/skybox/water/skybox_left.jpg"),
    //     Core::SnowFileHandle("Textures/skybox/water/skybox_top.jpg"),   Core::SnowFileHandle("Textures/skybox/water/skybox_bottom.jpg"),
    //     Core::SnowFileHandle("Textures/skybox/water/skybox_front.jpg"), Core::SnowFileHandle("Textures/skybox/water/skybox_back.jpg"),
    // };

    // std::array cubeMapFaces = {
    //     Core::SnowFileHandle("Textures/skybox/ulukai/corona_px.png"), Core::SnowFileHandle("Textures/skybox/ulukai/corona_nx.png"),
    //     Core::SnowFileHandle("Textures/skybox/ulukai/corona_py.png"), Core::SnowFileHandle("Textures/skybox/ulukai/corona_ny.png"),
    //     Core::SnowFileHandle("Textures/skybox/ulukai/corona_pz.png"), Core::SnowFileHandle("Textures/skybox/ulukai/corona_nz.png"),
    // };

    // std::array cubeMapFaces = {
    //     Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_px.png"), Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_nx.png"),
    //     Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_py.png"), Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_ny.png"),
    //     Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_pz.png"), Core::SnowFileHandle("Textures/skybox/ulukai/redeclipse_nz.png"),
    // };

    std::array cubeMapFaces = {
        Core::SnowFileHandle("Textures/skybox/clouds_stylized/px.png"), Core::SnowFileHandle("Textures/skybox/clouds_stylized/nx.png"),
        Core::SnowFileHandle("Textures/skybox/clouds_stylized/py.png"), Core::SnowFileHandle("Textures/skybox/clouds_stylized/ny.png"),
        Core::SnowFileHandle("Textures/skybox/clouds_stylized/pz.png"), Core::SnowFileHandle("Textures/skybox/clouds_stylized/nz.png"),
    };

    std::array<std::shared_ptr<Core::FileTexture>, 6> fTextures;

    for (uint32_t i = 0; i < cubeMapFaces.size(); ++i)
    {
        fTextures[i] = Core::FileSystem::LoadTexture(cubeMapFaces[i]);
        uint32_t target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        glTexImage2D(target, 0, GL_RGB, fTextures[i]->GetWidth(), fTextures[i]->GetHeight(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, fTextures[i]->GetData());
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    auto result = glGetError();
    assert(result == GL_NO_ERROR);

    //Init screen quad
    std::array positions{
        glm::vec2(-1, -1), glm::vec2(-1, 1), glm::vec2(1, 1),
        glm::vec2(-1, -1), glm::vec2(1, 1), glm::vec2(1, -1)
    };
    uint32_t quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &_quadVAO);
    glBindVertexArray(_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    auto shaderCompiler = OpenGLRenderSystem::GetInstance()->GetShaderCompiler();

    GraphicsPipelineCreateInfo params;
    params.VertexShader = Core::SnowFileHandle("skybox.vert");
    params.FragmentShader = Core::SnowFileHandle("skybox.frag");
    _quadProgram = shaderCompiler->GetOrCratePipeline(params);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void SkyboxRenderPass::Draw()
{
    _quadProgram->Bind();
    glDepthMask(GL_FALSE);
    glBindVertexArray(_quadVAO);
    BindSkybox(_quadProgram->GetProgramID(), 0);

    auto uniformSetter = _uniformLocations.GetSetter(_quadProgram->GetProgramID());
    uniformSetter.Set("invViewProjection",
                      OpenGLRenderSystem::GetInstance()->GetCamera().GetInvViewProjectionMatrix3());

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void SkyboxRenderPass::BindSkybox(uint32_t pipelineId, uint32_t textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTexture);
    glUniform1i(glGetUniformLocation(pipelineId, "skybox"), textureIndex);

}

} // namespace Snowglobe::RenderOpenGL