#pragma once

#include "FileSystem.hpp"

namespace Snowglobe::RenderOpenGL
{
    struct PipelineSetupParams
    {
        unsigned int vertexShader = UINT_MAX;
        unsigned int fragmentShader = UINT_MAX;
        unsigned int geometryShader = UINT_MAX;
        unsigned int tessControlShader = UINT_MAX;

        unsigned int computeShader = UINT_MAX;
    };
    
    enum PipelineType
    {
        None,
        Graphics,
        Compute
    };

    class ShaderCompiler
    {
    public:
        ShaderCompiler();
        ~ShaderCompiler();

        unsigned int GetOrCompileShader(const Core::SnowFileHandle& shader);
        unsigned int GetOrCratePipeline(const PipelineSetupParams& params);
    private:
        static int GetShaderType(const Core::SnowFileHandle& shader);
        PipelineType GetPipelineType(const PipelineSetupParams& params);
    };
    
} // namespace Snowglobe::RenderOpenGL
