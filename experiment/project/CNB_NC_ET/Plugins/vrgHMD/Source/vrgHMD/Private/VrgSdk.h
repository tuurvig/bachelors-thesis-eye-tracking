#pragma once

#include "vrg_internal.h"

#include <stdexcept>
#include <vector>

// TODO: Remove.
typedef int (*pVRgInitRecordHMDEyeCameraImage)(int id, char* file);
typedef void (*pVRgStopRecordHMDEyeCameraImage)(int id);
typedef bool (*pVRgStartHMDEyeCameraCalibration)(int id);
typedef bool (*pVRgIsHMDEyeCameraCalibrating)(int id);
typedef int (*pVRgSetHMDEyeCameraRecordPose)(int id, float x, float y, float z, float qx, float qy, float qz, float qw);

typedef int (*pVRgLset_compensation)(float deg);

#define DLL_FUNCTION_NAMED(NAME, NEWNAME)                                                                                                                      \
    using NAME##Type = decltype(NAME);                                                                                                                         \
    NAME##Type* NEWNAME = FuncLoader<NAME##Type*>(TEXT(#NAME));

#define DLL_FUNCTION_TYPED(NAME, NEWNAME, TYPE) TYPE NEWNAME = FuncLoader<TYPE>(TEXT(#NAME));

#define DLL_FUNCTION(NAME) DLL_FUNCTION_NAMED(NAME, NAME)

class VrgSdk {
public:
    static const VrgInitInfo Version;

    static TUniquePtr<VrgSdk> Init(void* module) {
        using VrgInitApi = decltype(vrgInitApi);
        VrgInitApi* vrgInitApi = reinterpret_cast<VrgInitApi*>(FPlatformProcess::GetDllExport(module, TEXT("vrgInitApi")));
        if (vrgInitApi == nullptr) {
            return nullptr;
        }

        if (vrgInitApi(&Version) != VRG_ERROR_OK) {
            return nullptr;
        }

        auto sdk = MakeUnique<VrgSdk>(module);
        if (sdk->m_notComplete) {
            return nullptr;
        }

        auto res = sdk->vrgGetInternalApi_7(&sdk->InternalApi);
        if (res != VRG_ERROR_OK || sdk->InternalApi == nullptr) {
            return nullptr;
        }

        return sdk;
    }

    VrgSdk(void* module);
    ~VrgSdk();

    static FVector ConvertVector(const VrgVector3& position) {
        return FVector(-position.z, position.x, position.y);
    }

    static VrgVector3 ConvertVector(const FVector& vector) {
        VrgVector3 res;
        res.x = vector.Y;
        res.y = vector.Z;
        res.z = -vector.X;
        return res;
    }

    static FQuat ConvertQuaternion(const VrgQuaternion& orientation) {
        return FQuat(orientation.z, -orientation.x, -orientation.y, orientation.w);
    }

    static VrgQuaternion ConvertQuaternion(const FQuat& quat) {
        VrgQuaternion res;
        res.x = -quat.Y;
        res.y = -quat.Z;
        res.z = quat.X;
        res.w = quat.W;
        return res;
    }

    uint32_t GetControllerIds(uint32_t* ids, uint32_t maxControllers) const {
        auto count = maxControllers;
        if (vrgEnumerateControllers(&count, ids) != VRG_ERROR_OK) {
            return 0;
        }
        return count;
    }

    int32 GetControllerId(int32 controllerIndex, EControllerHand Hand) const {
        const auto handIndex = static_cast<int>(Hand);
        return handIndex >= 0 && handIndex < 2 ? m_controllerMap[controllerIndex][handIndex] : controllerIndex;
    }

    void SetControllerMap(int32 InUnrealControllerIdAndHandToDeviceIdMap[8][2]) {
        for (int32 UnrealControllerIndex = 0; UnrealControllerIndex < 8; ++UnrealControllerIndex) {
            for (int32 HandIndex = 0; HandIndex < 2; ++HandIndex) {
                m_controllerMap[UnrealControllerIndex][HandIndex] = InUnrealControllerIdAndHandToDeviceIdMap[UnrealControllerIndex][HandIndex];
            }
        }
    }

private:
    int32 m_controllerMap[8][2];

    // Module needs to be initialized first before member DLL functions.
    void* m_module = nullptr;
    bool m_notComplete = false;

    template <typename T> T FuncLoader(const TCHAR* name) {
        T func = reinterpret_cast<T>(FPlatformProcess::GetDllExport(m_module, name));
        if (func == nullptr) {
            m_notComplete = true;
        }

        return func;
    }

public:
    const VrgInternalApi_7* InternalApi = nullptr;
    bool m_lighthouseInited = false;

    DLL_FUNCTION(vrgFreeApi);

    DLL_FUNCTION(vrgEnumerateDevices);
    DLL_FUNCTION(vrgInitDevice);
    DLL_FUNCTION(vrgFreeDevice);
    DLL_FUNCTION(vrgAttachGraphicsDevice);
    DLL_FUNCTION(vrgGetInt);
    DLL_FUNCTION(vrgGetFloat);
    DLL_FUNCTION(vrgSetBool);
    DLL_FUNCTION(vrgGetFov);
    DLL_FUNCTION(vrgGetPoseEx);
    DLL_FUNCTION(vrgRunAutoInterpupillaryDistance);
    DLL_FUNCTION(vrgCalibrateImu);
    DLL_FUNCTION(vrgSetTrackingSystemOrigin);
    DLL_FUNCTION(vrgGetErrorMessage);
    DLL_FUNCTION(vrgEnumerateControllers)
    DLL_FUNCTION(vrgGetControllerData)
    DLL_FUNCTION(vrgControllerTriggerHapticPulse)

    DLL_FUNCTION(vrgBeginFrame);
    DLL_FUNCTION(vrgEndFrame);
    DLL_FUNCTION(vrgSubmitFrameCoordinates);
    DLL_FUNCTION(vrgSubmitFrameFov);
    DLL_FUNCTION(vrgSubmitFramePose);
    DLL_FUNCTION(vrgSubmitFrameLayer);

    DLL_FUNCTION(vrgGetInternalApi_7);

    // TODO: Remove. Use internal api.
    DLL_FUNCTION_TYPED(set_compensation, SetOpenVrCompensation, pVRgLset_compensation);

    DLL_FUNCTION_TYPED(initRecordHMDEyeCameraImage, initRecordHMDEyeCameraImage, pVRgInitRecordHMDEyeCameraImage);
    DLL_FUNCTION_TYPED(stopRecordHMDEyeCameraImage, stopRecordHMDEyeCameraImage, pVRgStopRecordHMDEyeCameraImage);
    DLL_FUNCTION_TYPED(startHMDEyeCameraCalibration, startHMDEyeCameraCalibration, pVRgStartHMDEyeCameraCalibration);
    DLL_FUNCTION_TYPED(setHMDEyeCameraRecordPose, setHMDEyeCameraRecordPose, pVRgSetHMDEyeCameraRecordPose);
    DLL_FUNCTION_TYPED(isHMDEyeCameraCalibrating, isHMDEyeCameraCalibrating, pVRgIsHMDEyeCameraCalibrating);
};
