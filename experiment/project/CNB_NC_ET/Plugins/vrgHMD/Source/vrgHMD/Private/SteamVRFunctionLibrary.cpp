// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
//

#include "../../vrgHMD/Classes/SteamVRFunctionLibrary.h"
#include "VRgPrivatePCH.h"
// for STEAMVR_SUPPORTED_PLATFORMS, keep at top
#include "../../vrgHMD/Public/IVRgPlugin.h"

#include "Engine/Engine.h"

#include "IHeadMountedDisplay.h"
#include "Runtime/Engine/Public/ScreenRendering.h"

#if PLATFORM_WINDOWS
#include "../Core/Public/Windows/WindowsHWrapper.h"
#endif

#if 1 //STEAMVR_SUPPORTED_PLATFORMS
#include "../../../openvr.h" //418
#endif // STEAMVR_SUPPORTED_PLATFORMS

#include "../../vrgHMD/Private/VRgHMD.h" //418

namespace {

FVRgHMD* GetVrgHmd() {

    static FName SystemName(TEXT("VRgHMD"));
    if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName)) {
        return static_cast<FVRgHMD*>(GEngine->XRSystem.Get());
    }

    return nullptr;
}

VrgSdk* GetVrgSdk() {
    auto hmd = GetVrgHmd();
    return hmd ? hmd->GetSdk() : nullptr;
}

bool GetDevicePositionAndOrientation(VrgSdk* sdk, int32 DeviceId, FVector& OutPosition, FRotator& OutOrientation) {
    FQuat DeviceOrientation = FQuat::Identity;

    VrgControllerData ctrlData;
    if (sdk->vrgGetControllerData(DeviceId, &ctrlData) != VRG_ERROR_OK) {
        return false;
    }

    OutPosition = VrgSdk::ConvertVector(ctrlData.position) * 100;
    DeviceOrientation = VrgSdk::ConvertQuaternion(ctrlData.orientation);

    //chng
    //POSE p;
    //SteamVRHMD->GetTrackedData(DeviceId, p);
    //OutPosition.X = p.z * 100;
    //OutPosition.Y = p.x * 100;
    //OutPosition.Z = p.y * 100;
    //DeviceOrientation.X = p.qz;
    //DeviceOrientation.Y = p.qx;
    //DeviceOrientation.Z = p.qy;
    //DeviceOrientation.W = p.qw;

    //RetVal = SteamVRHMD->GetTrackedObjectOrientationAndPosition(DeviceId, DeviceOrientation, OutPosition);
    OutOrientation = DeviceOrientation.Rotator();

    return true;
}

} // namespace

USteamVRFunctionLibrary::USteamVRFunctionLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void USteamVRFunctionLibrary::GetValidTrackedDeviceIds(ESteamVRTrackedDeviceType DeviceType, TArray<int32>& OutTrackedDeviceIds) {
    OutTrackedDeviceIds.Empty();

    // TODO: DeviceType
    auto Sdk = GetVrgSdk();
    if (Sdk) {
        uint32_t count = 0;
        Sdk->vrgEnumerateControllers(&count, nullptr);

        if (count > 0) {
            std::vector<uint32_t> controllers(count);
            Sdk->vrgEnumerateControllers(&count, controllers.data());

            for (const auto id : controllers) {
                OutTrackedDeviceIds.Add(id);
            }
        }
    }

    //FVRgHMD* VrgHmd = GetVrgHmd();
    //if (VrgHmd) {
    //    int* ids;
    //    int c;
    //    int t = 0;
    //    if (DeviceType == ESteamVRTrackedDeviceType::Controller) {
    //        t = 2;
    //    }
    //
    //    VrgHmd->GetTrackedIds(t, ids, c);
    //
    //    for (int a = 0; a < c; a++)
    //        OutTrackedDeviceIds.Add(ids[a]);
    //}
}

bool USteamVRFunctionLibrary::GetTrackedDevicePositionAndOrientation(int32 DeviceId, FVector& OutPosition, FRotator& OutOrientation) {
    bool RetVal = false;

    FVRgHMD* VrgHmdPlugin = GetVrgHmd();
    if (VrgHmdPlugin) {
        RetVal = GetDevicePositionAndOrientation(VrgHmdPlugin->GetSdk(), DeviceId, OutPosition, OutOrientation);
    }

    return RetVal;
}

bool USteamVRFunctionLibrary::GetHandPositionAndOrientation(int32 ControllerIndex, EControllerHand Hand, FVector& OutPosition, FRotator& OutOrientation) {
    bool RetVal = false;

    FVRgHMD* VrgHmdPlugin = GetVrgHmd();
    if (VrgHmdPlugin) {
        //chng BACHA TADY MOZNA BUDE POTREBA PRACOVAT S MAP
        const int32 DeviceId = VrgHmdPlugin->GetSdk()->GetControllerId(ControllerIndex, Hand);

        RetVal = GetDevicePositionAndOrientation(VrgHmdPlugin->GetSdk(), DeviceId, OutPosition, OutOrientation);
    }

    return RetVal;
}
