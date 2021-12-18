/*
    [xingweizhu][add uiwidgets support][file]
*/
#pragma once

#include "IUnityInterface.h"
#include "IUnityGraphics.h"

namespace UnityUIWidgets {
typedef void (*VoidCallback)();
typedef void (*VoidCallbackLong)(long);
typedef void (*VoidCallbackDouble)(double);

UNITY_DECLARE_INTERFACE(IUnityUIWidgets) {
  virtual ~IUnityUIWidgets() {}

  virtual void SetUpdateCallback(VoidCallback callback) = 0;
  virtual void SetVSyncCallback(VoidCallbackDouble callback) = 0;
  virtual void SetWaitCallback(VoidCallbackLong callback) = 0;
  virtual void SetWakeUpCallback(VoidCallback callback) = 0;
  virtual void IssuePluginEventAndData(UnityRenderingEventAndData callback,
                                       int eventId, void* data) = 0;
};
}  // namespace UnityUIWidgets

UNITY_REGISTER_INTERFACE_GUID_IN_NAMESPACE(0x4C8BE8056B3C41D7ULL,
                                           0xBC8BF5F2F0AC3532ULL,
                                           IUnityUIWidgets, UnityUIWidgets)