#pragma once
#include "RenderAPI.h"
#include "Unity/IUnityInterface.h"
#include "NativeMesh.h"
#include <vector>

class NativeManager
{
public :
	IUnityGraphics* graphics = NULL;
	IUnityInterfaces* unityInterfaces = NULL;
	RenderAPI* renderApi = NULL;
	UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;
	static NativeManager* ins();
	std::vector<NativeMesh*> meshs;
};

#define NM (NativeManager::ins())