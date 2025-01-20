#include "ShaderCompiler.hpp"

#include <iostream>
#include <string>
#include <glad/gl.h>
#include "SnowFileSystem.hpp"

namespace Snowglobe::RenderOpenGL
{
    
ShaderCompiler::ShaderCompiler()
{
}

ShaderCompiler::~ShaderCompiler()
{

}

unsigned int ShaderCompiler::GetOrCompileShader(const SnowCore::SnowFileHandle& shader)
{
    auto shaderSource = SnowCore::SnowFileSystem::GetInstance().ReadTextFile(shader);
    if(shaderSource == nullptr)
    {
        return UINT_MAX;
    }

    int shaderType = GetShaderType(shader);

    if(shaderType == 0)
    {
        return UINT_MAX;
    }

    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
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

    return shaderId;
}

unsigned int ShaderCompiler::GetOrCratePipeline(const PipelineSetupParams& params)
{
    auto pipelineType = GetPipelineType(params);

    if(pipelineType == PipelineType::None)
    {
        return UINT_MAX;
    }

    unsigned int pipelineId = glCreateProgram();

    if(pipelineType == PipelineType::Graphics)
    {
        glAttachShader(pipelineId, params.vertexShader);
        glAttachShader(pipelineId, params.fragmentShader);

        if(params.geometryShader != UINT_MAX)
        {
            glAttachShader(pipelineId, params.geometryShader);
        }

        if(params.tessControlShader != UINT_MAX)
        {
            glAttachShader(pipelineId, params.tessControlShader);
        }
    } 
    else if(pipelineType == PipelineType::Compute)
    {
        glAttachShader(pipelineId, params.computeShader);
    }

    glLinkProgram(pipelineId);

    int success;
    glGetProgramiv(pipelineId, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(pipelineId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return UINT_MAX;
    }

    return pipelineId;
}

int ShaderCompiler::GetShaderType(const SnowCore::SnowFileHandle &shader)
{
    auto extension = shader.path.extension().string();

    if(extension.empty())
    {
        std::cout << "No extension found at" << shader.path << std::endl;
        return 0;
    }

    if(extension == ".frag")
    {
        return GL_FRAGMENT_SHADER;
    }

    if(extension == ".vert")
    {
        return GL_VERTEX_SHADER;
    }

    if(extension == ".geom")
    {
        return GL_GEOMETRY_SHADER;
    }

    if(extension == ".tesc")
    {
        return GL_TESS_CONTROL_SHADER;
    }

    if(extension == ".comp")
    {
        return GL_COMPUTE_SHADER;
    }

    std::cout << "Unknown shader type at" << shader.path << std::endl;
    return 0;
}

PipelineType ShaderCompiler::GetPipelineType(const PipelineSetupParams &params)
{
    if(params.computeShader != UINT_MAX)
    {
        return PipelineType::Compute;
    }

    if(params.fragmentShader != UINT_MAX && params.vertexShader != UINT_MAX)
    {
        return PipelineType::Graphics;
    }

    std::cout << "Unknown pipeline type" << std::endl;
    return PipelineType::None;
}

} // namespace Snowglobe::RenderOpenGL
