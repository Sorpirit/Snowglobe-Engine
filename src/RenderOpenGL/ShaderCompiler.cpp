#include "ShaderCompiler.hpp"

#include "FileSystem.hpp"
#include <glad/gl.h>
#include <iostream>
#include <string>

#include <ranges>

namespace Snowglobe::RenderOpenGL
{
void PipelineProgram::Init(uint32_t programId)
{
    if (_programId != UINT32_MAX && _programId != programId)
        glDeleteProgram(_programId);

    _programId = programId;
}
void PipelineProgram::Bind() const { glUseProgram(_programId); }

std::shared_ptr<PipelineProgram> ShaderCompiler::GetOrCratePipeline(GraphicsPipelineCreateInfo& info)
{
    auto pipelineKey = GetPipelineKey(info);
    auto it = _pipelineCache.find(pipelineKey);
    if (it != _pipelineCache.end())
        return it->second;

    uint32_t programId = CreatePipeline(info);

    if (programId == UINT32_MAX)
        return nullptr;

    auto pipelineProgram = std::make_shared<PipelineProgram>(info);
    pipelineProgram->Init(programId);
    _pipelineCache[pipelineKey] = pipelineProgram;
    return pipelineProgram;
}

void ShaderCompiler::RecompileAll()
{
    _shaderCache.clear();
    for (auto& pipeline : _pipelineCache | std::views::values)
    {
        uint32_t programId = CreatePipeline(pipeline->GetInfo());

        if (programId == UINT32_MAX)
            continue;

        pipeline->Init(programId);
    }
}

size_t ShaderCompiler::GetShaderKey(const Core::SnowFileHandle& shader, uint32_t shaderType, uint32_t version,
                                    const std::vector<ShaderDefine>& defines)
{
    size_t hashVal = std::hash<Core::SnowFileHandle>{}(shader);
    hashVal ^= std::hash<uint32_t>{}(shaderType) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    hashVal ^= std::hash<uint32_t>{}(version) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    for (const auto& define : defines)
    {
        hashVal ^= std::hash<ShaderDefine>{}(define) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    }
    return hashVal;
}

size_t ShaderCompiler::GetPipelineKey(const GraphicsPipelineCreateInfo& createInfo)
{
    size_t hashVal = std::hash<Core::SnowFileHandle>{}(createInfo.FragmentShader);
    hashVal ^=
        std::hash<Core::SnowFileHandle>{}(createInfo.VertexShader) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    hashVal ^=
        std::hash<Core::SnowFileHandle>{}(createInfo.GeometryShader) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    hashVal ^= std::hash<Core::SnowFileHandle>{}(createInfo.TesselationControlShader) + 0x9e3779b9 + (hashVal << 6) +
               (hashVal >> 2);
    hashVal ^= std::hash<uint32_t>{}(createInfo.version) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    for (const auto& define : createInfo.defines)
    {
        hashVal ^= std::hash<ShaderDefine>{}(define) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);
    }
    return hashVal;
}
uint32_t ShaderCompiler::GetOrCompileShader(const Core::SnowFileHandle& shader, uint32_t shaderType, uint32_t version,
                                            const std::vector<ShaderDefine>& defines)
{
    auto shaderKey = GetShaderKey(shader, shaderType, version, defines);
    auto it = _shaderCache.find(shaderKey);
    if (it != _shaderCache.end())
        return it->second;

    if (!shader.IsValid())
    {
        std::cerr << "Shader path invalid: " << shader.GetFullPath() << '\n';
        return UINT_MAX;
    }

    std::stringstream shaderSourceBuffer;

    shaderSourceBuffer << "#version " << version << " core\n";
    for (const auto& define : defines)
    {
        shaderSourceBuffer << "#define " << define.name << " " << define.value << '\n';
    }

    if(!Core::FileSystem::TryReadTextFile(shader, shaderSourceBuffer))
    {
        std::cerr << "Unable to read shader at: " << shader.GetFullPath() << '\n';
        return UINT_MAX;
    }

    std::string shaderSource = shaderSourceBuffer.str();
    const char* sourcePointer = shaderSource.c_str();
    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &sourcePointer, nullptr);
    glCompileShader(shaderId);

    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return UINT_MAX;
    }

    _shaderCache[shaderKey] = shaderId;
    return shaderId;
}

uint32_t ShaderCompiler::CreatePipeline(const GraphicsPipelineCreateInfo& info)
{
    if (!info.VertexShader.IsValid() || !info.FragmentShader.IsValid())
    {
        std::cerr << "Missing vertex or fragment shader for creation of graphics pipeline" << std::endl;
        return UINT32_MAX;
    }

    uint32_t vertexShader = GetOrCompileShader(info.VertexShader, GL_VERTEX_SHADER, info.version, info.defines);
    uint32_t fragmentShader = GetOrCompileShader(info.FragmentShader, GL_FRAGMENT_SHADER, info.version, info.defines);
    if (vertexShader == UINT_MAX || fragmentShader == UINT_MAX)
    {
        std::cerr << "Failed to compile vertex or fragment shader" << std::endl;
        return UINT32_MAX;
    }

    uint32_t pipelineId = glCreateProgram();
    glAttachShader(pipelineId, vertexShader);
    glAttachShader(pipelineId, fragmentShader);

    if (info.GeometryShader.IsValid())
    {
        uint32_t geometryShader =
            GetOrCompileShader(info.GeometryShader, GL_GEOMETRY_SHADER, info.version, info.defines);
        if (geometryShader == UINT_MAX)
        {
            std::cerr << "Failed to compile geometryShader" << std::endl;
            glDeleteProgram(pipelineId);
            return UINT32_MAX;
        }

        glAttachShader(pipelineId, geometryShader);
    }

    if (info.TesselationControlShader.IsValid())
    {
        uint32_t tesselationShader =
            GetOrCompileShader(info.TesselationControlShader, GL_TESS_CONTROL_SHADER, info.version, info.defines);
        if (tesselationShader == UINT_MAX)
        {
            std::cerr << "Failed to compile tesselationShader" << std::endl;
            glDeleteProgram(pipelineId);
            return UINT32_MAX;
        }

        glAttachShader(pipelineId, tesselationShader);
    }

    glLinkProgram(pipelineId);

    int success;
    glGetProgramiv(pipelineId, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(pipelineId, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteProgram(pipelineId);
        return UINT32_MAX;
    }

    return pipelineId;
}

} // namespace Snowglobe::RenderOpenGL
