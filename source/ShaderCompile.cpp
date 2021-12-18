#include "NativeRendering/Unity/IUnityShaderCompilerAccess.h"
#include <iostream>
using namespace std;

const char* SHADER_KEYWORDS = "";

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API init_render_mode_editor(const char* path)
{

}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityShaderCompilerExtEvent(UnityShaderCompilerExtEventType event, void* data)
{
    switch (event)
    {
    case kUnityShaderCompilerExtEventCreateCustomSourceVariant:
    {
        UnityShaderCompilerExtCustomSourceVariantParams* pluginConfig = (UnityShaderCompilerExtCustomSourceVariantParams*)data;
        string str = (const char*)pluginConfig->inputSnippet;
        break;
    }

    case kUnityShaderCompilerExtEventCreateCustomBinaryVariant:
    {

        UnityShaderCompilerExtCustomBinaryVariantParams* pluginConfig = (UnityShaderCompilerExtCustomBinaryVariantParams*)data;
        string str = (const char*)pluginConfig->inputByteCode;

        break;
    }

    case kUnityShaderCompilerExtEventPluginConfigure:
        {
        //kUnityShaderCompilerExtGPUProgramTargetGLES31AEP = 2,
        //    kUnityShaderCompilerExtGPUProgramTargetGLES31 = 3,
        //    kUnityShaderCompilerExtGPUProgramTargetGLES3 = 4,
        //    kUnityShaderCompilerExtGPUProgramTargetGLES = 5,
        //    kUnityShaderCompilerExtGPUProgramTargetSPIRV = 25,


            unsigned int GPUCompilerMask = (1 << kUnityShaderCompilerExtGPUProgramTargetSPIRV)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetGLES)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetGLES3)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetGLES31)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetGLES31AEP)

                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11VertexSM50)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11PixelSM40)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11PixelSM50)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11GeometrySM40)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11GeometrySM50)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11HullSM50)
                | (1 << kUnityShaderCompilerExtGPUProgramTargetDX11DomainSM50);

            unsigned int ShaderMask = kUnityShaderCompilerExtGPUProgramVS | kUnityShaderCompilerExtGPUProgramPS | kUnityShaderCompilerExtGPUProgramDS;

            IUnityShaderCompilerExtPluginConfigure* pluginConfig = (IUnityShaderCompilerExtPluginConfigure*)data;
            pluginConfig->ReserveKeyword(SHADER_KEYWORDS);
            pluginConfig->SetGPUProgramCompilerMask(GPUCompilerMask);
            pluginConfig->SetShaderProgramMask(ShaderMask);
            break;
        }
    }
}