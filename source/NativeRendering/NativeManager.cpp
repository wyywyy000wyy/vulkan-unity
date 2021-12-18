#include "NativeManager.h"

NativeManager* NativeManager::ins()
{
	static NativeManager* nm = new NativeManager();
	return nm;
}