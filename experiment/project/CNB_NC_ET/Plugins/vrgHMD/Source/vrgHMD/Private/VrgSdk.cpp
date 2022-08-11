#include "VrgSdk.h"

#include "Windows/WindowsPlatformProcess.h"

const VrgInitInfo VrgSdk::Version{ VRG_VERSION_MAJOR, VRG_VERSION_MINOR };

VrgSdk::VrgSdk(void* module)
    : m_module(module) {}

VrgSdk::~VrgSdk() {
    FPlatformProcess::FreeDllHandle(m_module);
    m_module = nullptr;
}
