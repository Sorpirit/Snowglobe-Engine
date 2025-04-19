#pragma once

#include "FileSystem.hpp"

namespace Snowglobe::RenderOpenGL
{
struct ShaderDefine
{
    std::string name = "";
    std::string value = "";
};

struct GraphicsPipelineCreateInfo
{
    Core::SnowFileHandle VertexShader;
    Core::SnowFileHandle FragmentShader;
    Core::SnowFileHandle GeometryShader;
    Core::SnowFileHandle TesselationControlShader;

    int version = 420;
    std::vector<ShaderDefine> defines;
};

struct PipelineProgram
{
public:
    PipelineProgram(GraphicsPipelineCreateInfo info) : _info(std::move(info)) {}
    void Init(uint32_t programId);
    void Bind() const;
    const GraphicsPipelineCreateInfo& GetInfo() { return _info; }
    uint32_t GetProgramID() const { return _programId; }

  private:
    uint32_t _programId = UINT32_MAX;
    GraphicsPipelineCreateInfo _info;
};

class ShaderCompiler
{
  public:
    std::shared_ptr<PipelineProgram> GetOrCratePipeline(GraphicsPipelineCreateInfo& info);

    void RecompileAll();

  private:
    static size_t GetShaderKey(const Core::SnowFileHandle& shader, uint32_t shaderType, uint32_t version,
                               const std::vector<ShaderDefine>& defines);
    static size_t GetPipelineKey(const GraphicsPipelineCreateInfo& createInfo);

    uint32_t GetOrCompileShader(const Core::SnowFileHandle& shader, uint32_t shaderType, uint32_t version,
                                const std::vector<ShaderDefine>& defines);

    uint32_t CreatePipeline(const GraphicsPipelineCreateInfo& info);

    std::unordered_map<size_t, uint32_t> _shaderCache;
    std::unordered_map<size_t, std::shared_ptr<PipelineProgram>> _pipelineCache;
};

} // namespace Snowglobe::RenderOpenGL

namespace std
{
template <> struct hash<Snowglobe::RenderOpenGL::ShaderDefine>
{
    size_t operator()(const Snowglobe::RenderOpenGL::ShaderDefine& s) const
    {
        size_t hashVal = hash<string>{}(s.name);
        hashVal ^= hash<string>{}(s.value) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
        return hashVal;
    }
};
} // namespace std
