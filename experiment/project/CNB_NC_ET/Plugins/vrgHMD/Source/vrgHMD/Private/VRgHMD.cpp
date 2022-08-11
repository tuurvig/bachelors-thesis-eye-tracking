// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// 2016 VRg Global, Ltd. v1.0(8e)

#include "VRgHMD.h"
#include "Frustum.h"

#include "VRgPrivatePCH.h"

#include "Runtime/Renderer/Private/RendererPrivate.h"
#include "Runtime/Renderer/Private/ScenePrivate.h"
#include "Runtime/Renderer/Private/PostProcess/PostProcessHMD.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"

#include "DefaultSpectatorScreenController.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#endif

#if UVER >= 417
#include "../Public/PipelineStateCache.h"
#endif

#include <algorithm>

namespace {

inline bool Equal(const VrgFov& f1, const VrgFov& f2) {
    return f1.leftTan == f2.leftTan && f1.rightTan == f2.rightTan && f1.topTan == f2.topTan && f1.bottomTan == f2.bottomTan;
}

inline bool Equal(const VrgResolution& res1, const VrgResolution& res2) {
    return res1.width == res2.width && res1.height == res2.height;
}

inline bool IsStereoPass(EStereoscopicPass Eye) {
#if VRG_FOV_RENDERING
    if (Eye == eSSP_LEFT_EYE_SIDE || Eye == eSSP_RIGHT_EYE_SIDE) {
        return true;
    }
#endif

    return Eye == eSSP_LEFT_EYE || Eye == eSSP_RIGHT_EYE;
}

FSceneViewport* FindSceneViewport() {
    if (!GIsEditor) {
        UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
        return GameEngine->SceneViewport.Get();
    }
#if WITH_EDITOR
    else {
        UEditorEngine* EditorEngine = CastChecked<UEditorEngine>(GEngine);
        FSceneViewport* PIEViewport = (FSceneViewport*)EditorEngine->GetPIEViewport();
        if (PIEViewport != nullptr && PIEViewport->IsStereoRenderingAllowed()) {
            return PIEViewport;
        } else {
            FSceneViewport* EditorViewport = (FSceneViewport*)EditorEngine->GetActiveViewport();
            if (EditorViewport != nullptr && EditorViewport->IsStereoRenderingAllowed()) {
                return EditorViewport;
            }
        }
    }
#endif
    return nullptr;
}

void ShowError(const TCHAR* message) {
    UE_LOG(LogHMD, Error, TEXT("VRG SDK error: %s"), message);
    MessageBoxW(0, message, TEXT("VRG HMD Plugin"), 0);
}

void ShowErrorStr(const FString& message) {
    ShowError(*message);
}

void HandleError(VrgSdk* sdk, VrgErrorCode error) {
    const char* message = nullptr;
    if (sdk->vrgGetErrorMessage(error, &message) != VRG_ERROR_OK) {
        message = "Unknow error";
    }

    MessageBoxA(0, message, "VRG HMD Plugin", 0);
}

} // namespace

class FVRgPlugin : public IVRgPlugin {
    TSharedPtr<class IXRTrackingSystem, ESPMode::ThreadSafe> CreateTrackingSystem() override {
        auto VRgHMD = MakeShared<FVRgHMD, ESPMode::ThreadSafe>(this);
        if (VRgHMD->IsInitialized()) {
            return VRgHMD;
        }

        return nullptr;
    }

    FString GetModuleKeyName() const {
        return FString(TEXT("VRgHMD"));
    }

    FString GetModulePriorityKeyName() const {
        return FString(TEXT("VRgHMD"));
    }

public:
    FVRgPlugin() {}

    void StartupModule() override {
        IHeadMountedDisplayModule::StartupModule();
    }

    void ShutdownModule() override {
        IHeadMountedDisplayModule::ShutdownModule();
    }

    bool IsHMDConnected() override {
        return true;
    }

    uint64 GetGraphicsAdapterLuid() override {
        return 0;
    }

    TSharedPtr<IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe> GetVulkanExtensions() override {
        return nullptr;
    }
};

IMPLEMENT_MODULE(FVRgPlugin, vrgHMD)

#if PLATFORM_WINDOWS
class FVrgD3D11CustomPresent : public FVRgHMD::FVRgCustomPresent {
public:
    FVrgD3D11CustomPresent(FVRgHMD* plugin, VrgSdk* vrgSdk, int hmdId)
        : FVRgCustomPresent(plugin)
        , m_vrgSdk(vrgSdk)
        , m_hmdId(hmdId) {}

    void OnBackBufferResize() override {}

    bool Present(int& SyncInterval) override {
        check(IsInRenderingThread());

        FinishRendering();

        return true;
    }

    void PostPresent() override {}

    void BeginRendering() override {
        check(IsInRenderingThread());

        //CheckInited();
    }

    void FinishRendering() override {
        check(IsInRenderingThread());

        //CheckInited();

        if (m_render && m_inited) {
            m_vrgSdk->vrgBeginFrame(m_hmdId);

            // TODO: Submit camera poses.
            //m_vrgSdk->vrgSubmitFramePose(m_hmdId, VRG_LAYER_LEFT, &m_leftCamPose);
            //m_vrgSdk->vrgSubmitFramePose(m_hmdId, VRG_LAYER_RIGHT, &m_rightCamPose);

            m_vrgSdk->vrgSubmitFrameCoordinates(m_hmdId, VRG_LAYER_LEFT, m_baseLayer.left.u, m_baseLayer.left.v, m_baseLayer.left.um, m_baseLayer.left.vm);
            m_vrgSdk->vrgSubmitFrameCoordinates(m_hmdId, VRG_LAYER_RIGHT, m_baseLayer.right.u, m_baseLayer.right.v, m_baseLayer.right.um, m_baseLayer.right.vm);

            m_vrgSdk->vrgSubmitFrameLayer(m_hmdId, VRG_LAYER_LEFT, m_renderTargetTexture);
            m_vrgSdk->vrgSubmitFrameLayer(m_hmdId, VRG_LAYER_RIGHT, m_renderTargetTexture);

            if (m_fovRendering) {
                //m_vrgSdk->vrgSubmitFramePose(m_hmdId, VRG_LAYER_LEFT_DETAIL, &m_leftCamPose);
                //m_vrgSdk->vrgSubmitFramePose(m_hmdId, VRG_LAYER_RIGHT_DETAIL, &m_rightCamPose);

                m_vrgSdk->vrgSubmitFrameCoordinates(
                    m_hmdId, VRG_LAYER_LEFT_DETAIL, m_detailLayer.left.u, m_detailLayer.left.v, m_detailLayer.left.um, m_detailLayer.left.vm);
                m_vrgSdk->vrgSubmitFrameCoordinates(
                    m_hmdId, VRG_LAYER_RIGHT_DETAIL, m_detailLayer.right.u, m_detailLayer.right.v, m_detailLayer.right.um, m_detailLayer.right.vm);

                m_vrgSdk->vrgSubmitFrameLayer(m_hmdId, VRG_LAYER_LEFT_DETAIL, m_renderTargetTexture);
                m_vrgSdk->vrgSubmitFrameLayer(m_hmdId, VRG_LAYER_RIGHT_DETAIL, m_renderTargetTexture);
            }

            m_vrgSdk->vrgEndFrame(m_hmdId);
        }
    }

    void UpdateViewport(const FViewport& Viewport, FRHIViewport* ViewportRHI, const VrgLayer* baseLayer, const VrgLayer* detailLayer) override {
        check(IsInGameThread());
        check(ViewportRHI);

        const FTexture2DRHIRef& RT = Viewport.GetRenderTargetTexture();
        check(IsValidRef(RT));

        if (m_renderTargetTexture != nullptr) {
            m_renderTargetTexture->Release();
        }

        m_renderTargetTexture = reinterpret_cast<ID3D11Texture2D*>(RT->GetNativeResource());
        m_renderTargetTexture->AddRef();

        m_render = GWorld->HasBegunPlay();
        ViewportRHI->SetCustomPresent(this);

        bool wasEnabled = m_fovRendering;
        if (baseLayer) {
            m_baseLayer = *baseLayer;
        }
        if (detailLayer) {
            m_detailLayer = *detailLayer;
            m_fovRendering = true;
        } else {
            m_fovRendering = false;
        }

        if (m_fovRendering != wasEnabled) {
            m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_FOVEATED_RENDER, m_fovRendering ? 1 : 0);
        }
    }

    void SetPose(const VrgPose& hmdPose, const VrgPose& leftCamPose, const VrgPose& rightCamPose, const VrgPose& leftDetailCamPose,
        const VrgPose& rightDetailCamPose) override {
        // TODO: Calculate absolute position based on hmdPose and left/right camera pose.
        //auto vec = VrgSdk::ConvertVector(hmdPose.position);
        //auto dir = VrgSdk::ConvertQuaternion(hmdPose.orientation);

        //auto leftPos = VrgSdk::ConvertVector(leftCamPose.position);
        //auto leftDir = VrgSdk::ConvertQuaternion(leftCamPose.orientation);
        //auto rightPos = VrgSdk::ConvertVector(rightCamPose.position);
        //auto rightDir = VrgSdk::ConvertQuaternion(rightCamPose.orientation);

        m_pose = hmdPose;
        m_leftCamPose = leftCamPose;
        m_rightCamPose = rightCamPose;
        m_leftDetailCamPose = leftDetailCamPose;
        m_rightDetailCamPose = rightDetailCamPose;

        // TODO: DEBUG
        CheckInited();
    }

    void Reset() override {}
    void Shutdown() override {}

    FIntPoint TextureSizeLimit() const override {
        return FIntPoint(16 * 1024, 16 * 1024);
    }

protected:
    ID3D11Texture2D* m_renderTargetTexture = nullptr;

private:
    void CheckInited() {
        if (!m_inited && !m_initializing) {
            m_initializing = true;
            auto res = m_vrgSdk->vrgAttachGraphicsDevice(m_hmdId, VRG_API_DIRECTX_11, RHIGetNativeDevice());
            if (res == VRG_ERROR_OK) {
                m_inited = true;
            } else {
                if (m_firstError) {
                    ShowError(TEXT("Failed to init graphics."));
                    m_firstError = false;
                }
                m_inited = false;
                // Can't use outside game thread.
                // Plugin->EnableStereo(false);
            }
            m_initializing = false;
        }
    }

    VrgSdk* m_vrgSdk = nullptr;
    int m_hmdId = 0;
    bool m_render = true;

    bool m_inited = false;
    bool m_initializing = false;

    VrgPose m_pose;
    VrgPose m_leftCamPose;
    VrgPose m_rightCamPose;
    VrgPose m_leftDetailCamPose;
    VrgPose m_rightDetailCamPose;
    bool m_firstError = true;

    VrgLayer m_baseLayer;
    VrgLayer m_detailLayer;
    bool m_fovRendering = false;
};
#endif // PLATFORM_WINDOWS

#if UVER >= 419
FIntPoint FVRgHMD::GetIdealRenderTargetSize() const {
    return FIntPoint(m_resolution.width, m_resolution.height);
}
#else
void FVRgHMD::CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) {
    InOutSizeX = m_resolution.width;
    InOutSizeY = m_resolution.height;
}
#endif

#if UVER >= 426
bool FVRgHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags Flags,
    ETextureCreateFlags TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
#else
bool FVRgHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 InTexFlags,
    uint32 InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
#endif
{
#if UVER > 417
    if (!IsStereoEnabled()) {
        return false;
    }

    FRHIResourceCreateInfo CreateInfo;
    RHICreateTargetableShaderResource2D(
        SizeX, SizeY, PF_B8G8R8A8, 1, TexCreate_None, TexCreate_RenderTargetable, false, CreateInfo, OutTargetableTexture, OutShaderResourceTexture);
    return true;
#endif

    return false;
}

bool FVRgHMD::OnStartGameFrame(FWorldContext& WorldContext) {
    static const double kShutdownTimeout = 4.0;

    if (m_hmdId < 0) {
        return false;
    }

    FQuat Orientation;
    FVector Position;

    GetCurrentPose(IXRTrackingSystem::HMDDeviceId, Orientation, Position);
    return true;
}

bool FVRgHMD::EnumerateTrackedDevices(TArray<int32>& TrackedIds, EXRTrackedDeviceType DeviceType) {
    return false;
}

bool FVRgHMD::GetTrackingSensorProperties(int32 SensorId, FQuat& OutOrientation, FVector& OutOrigin, FXRSensorProperties& OutSensorProperties) {
    OutOrigin = FVector::ZeroVector;
    OutOrientation = FQuat::Identity;
    OutSensorProperties = FXRSensorProperties();

    // TODO: HMD count?
    return m_hmdId >= 0;
}

#if UVER >= 418
FString FVRgHMD::GetVersionString() const {
    uint32_t count = 1;
    VrgDevice device;

    auto res = m_vrgSdk->vrgEnumerateDevices(&count, &device);
    if ((res != VRG_ERROR_OK && res != VRG_ERROR_INCOMPLETE) || count < 1) {
        return FString();
    }

    // TODO: Read real information.
    const FString Manufacturer = FString("VRgineers");
    const FString Model = FString(device.name);
    const FString Serial = FString("1");
    const FString DriverId = FString("VRgHMD");
    const FString DriverVersion = FString("2.25"); // getVersion here

    return FString::Printf(TEXT("%s, Driver: %s, Serial: %s, HMD Device: %s %s, Driver version: %s"), *FEngineVersion::Current().ToString(), *DriverId, *Serial,
        *Manufacturer, *Model, *DriverVersion);
}

#endif

void FVRgHMD::RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const {}

bool FVRgHMD::IsHMDEnabled() const {
    return m_hmdEnabled;
}

void FVRgHMD::EnableHMD(bool enable) {
    m_hmdEnabled = enable;

    if (!m_hmdEnabled) {
        EnableStereo(false);
    }
}

bool FVRgHMD::GetHMDMonitorInfo(MonitorInfo& MonitorDesc) {
    if (IsInitialized() && m_hmdId > -1) {
        // TODO: Tohle se nikdy nezavola.
        UpdateResolution();

        MonitorDesc.MonitorName = FString("");
        MonitorDesc.MonitorId = 0;
        MonitorDesc.DesktopX = 0;
        MonitorDesc.DesktopY = 0;
        MonitorDesc.ResolutionX = m_baseLayer.resolution.width;
        MonitorDesc.ResolutionY = m_baseLayer.resolution.height;

        return true;
    } else {
        MonitorDesc.MonitorName = "";
        MonitorDesc.MonitorId = 0;
        MonitorDesc.DesktopX = MonitorDesc.DesktopY = MonitorDesc.ResolutionX = MonitorDesc.ResolutionY = 0;
    }

    return false;
}

void FVRgHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const {
    OutHFOVInDegrees = 0.0f;
    OutVFOVInDegrees = 0.0f;
}

bool FVRgHMD::HasValidTrackingPosition() {
    return m_hmdPosTracking;
}

void FVRgHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance) {
    // TODO:
}

float FVRgHMD::GetInterpupillaryDistance() const {
    if (IsInitialized() && m_hmdId > -1) {
        float left = 0;
        float right = 0;

        m_vrgSdk->vrgGetFloat(m_hmdId, VRG_FLOAT_PUPILLARY_DISTANCE_LEFT_M, &left);
        m_vrgSdk->vrgGetFloat(m_hmdId, VRG_FLOAT_PUPILLARY_DISTANCE_LEFT_M, &right);

        return left + right;
    } else {
        return 0.064f;
    }
}

float FVRgHMD::GetWorldToMetersScale() const {
    return m_worldToMetersScale;
}

void FVRgHMD::UpdateProjectionMatrix(FMatrix& matrix, const VrgFov& fov) const {
    const float nearP = 3.0f;
    const float farP = 10000.0f;

    float vFOVhalf = atanf(fov.topTan);
    CreateOurFrustumUnreal(-atanf(-fov.leftTan), atanf(fov.rightTan), vFOVhalf, nearP, farP, 0, matrix);
}

void FVRgHMD::ReloadProjectionMatrices() const {
    VrgFov fovLeft;
    VrgFov fovRight;
    VrgFov fovLeftDetail;
    VrgFov fovRightDetail;

    m_vrgSdk->vrgGetFov(m_hmdId, VRG_FOV_LEFT, &fovLeft);
    if (!Equal(fovLeft, m_fovLeft)) {
        m_fovLeft = fovLeft;
        UpdateProjectionMatrix(m_matrixLeft, m_fovLeft);
    }

    m_vrgSdk->vrgGetFov(m_hmdId, VRG_FOV_RIGHT, &fovRight);
    if (!Equal(fovRight, m_fovRight)) {
        m_fovRight = fovRight;
        UpdateProjectionMatrix(m_matrixRight, m_fovRight);
    }

    m_vrgSdk->vrgGetFov(m_hmdId, VRG_FOV_LEFT_DETAIL, &fovLeftDetail);
    if (!Equal(fovLeftDetail, m_fovLeftDetail)) {
        m_fovLeftDetail = fovLeftDetail;
        UpdateProjectionMatrix(m_matrixLeftDetail, m_fovLeftDetail);
    }

    m_vrgSdk->vrgGetFov(m_hmdId, VRG_FOV_RIGHT_DETAIL, &fovRightDetail);
    if (!Equal(fovRightDetail, m_fovRightDetail)) {
        m_fovRightDetail = fovRightDetail;
        UpdateProjectionMatrix(m_matrixRightDetail, m_fovRightDetail);
    }
}

#if UVER >= 419
bool FVRgHMD::GetHMDDistortionEnabled(EShadingPath) const {
#else
bool FVRgHMD::GetHMDDistortionEnabled() const {
#endif
    return false;
}

bool FVRgHMD::IsTracking(int32 DeviceId) {
    return m_hmdPosTracking;
}

bool FVRgHMD::GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) {
    if (DeviceId != IXRTrackingSystem::HMDDeviceId || !IsStereoPass(Eye)) {
        return false;
    }

    VrgPose* pose = nullptr;
    switch (Eye) {
    case eSSP_LEFT_EYE:
        pose = &m_leftCameraPose;
        break;
    case eSSP_RIGHT_EYE:
        pose = &m_rightCameraPose;
        break;
#if VRG_FOV_RENDERING
    case eSSP_LEFT_EYE_SIDE:
        pose = &m_leftDetailCameraPose;
        break;
    case eSSP_RIGHT_EYE_SIDE:
        pose = &m_rightDetailCameraPose;
        break;
#endif
    }

    if (!pose) {
        return false;
    }

    OutOrientation = VrgSdk::ConvertQuaternion(pose->orientation);
    OutPosition = VrgSdk::ConvertVector(pose->position) * m_worldToMetersScale;

    OutOrientation.Normalize();

    return true;
}

bool FVRgHMD::GetCurrentPose(int32 DeviceId, FQuat& CurrentOrientation, FVector& CurrentPosition) {
    if (!m_stereoEnabled) {
        return false;
    }

    VrgPose pose;
    if (m_vrgSdk->vrgGetPoseEx(m_hmdId, VRG_POSE_HMD, &pose) != VRG_ERROR_OK) {
        return false;
    }

    if (IsInRenderingThread()) {
        if (m_vrgSdk->vrgGetPoseEx(m_hmdId, VRG_POSE_CAMERA_LEFT, &m_leftCameraPose) != VRG_ERROR_OK) {
            return false;
        }

        if (m_vrgSdk->vrgGetPoseEx(m_hmdId, VRG_POSE_CAMERA_RIGHT, &m_rightCameraPose) != VRG_ERROR_OK) {
            return false;
        }

#if VRG_FOV_RENDERING
        if (m_vrgSdk->vrgGetPoseEx(m_hmdId, VRG_POSE_CAMERA_LEFT_DETAIL, &m_leftDetailCameraPose) != VRG_ERROR_OK) {
            return false;
        }

        if (m_vrgSdk->vrgGetPoseEx(m_hmdId, VRG_POSE_CAMERA_RIGHT_DETAIL, &m_rightDetailCameraPose) != VRG_ERROR_OK) {
            return false;
        }
#endif

        auto bridge = GetActiveRHIBridgeImpl();
        if (bridge) {
            bridge->SetPose(pose, m_leftCameraPose, m_rightCameraPose, m_leftDetailCameraPose, m_rightDetailCameraPose);
        }

        CurrentOrientation = VrgSdk::ConvertQuaternion(pose.orientation);
        CurrentPosition = VrgSdk::ConvertVector(pose.position) * m_worldToMetersScale;

        APlayerCameraManager* act = UGameplayStatics::GetPlayerCameraManager(GWorld, 0);
        if (act) {
            FVector loc = act->GetCameraLocation();
            FQuat ori = act->GetCameraRotation().Quaternion();

            // TODO:
            //VRgSetHMDEyeCameraRecordPose(0, loc.X, loc.Y, loc.Z, ori.X, ori.Y, ori.Z, ori.W);
        }
    } else {
        CurrentOrientation = VrgSdk::ConvertQuaternion(pose.orientation);
        CurrentPosition = VrgSdk::ConvertVector(pose.position) * m_worldToMetersScale;
    }

    CurrentOrientation.Normalize();

    return true;
}

bool FVRgHMD::IsChromaAbCorrectionEnabled() const {
    return true;
}

bool FVRgHMD::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) {
    if (FParse::Command(&Cmd, TEXT("vrg"))) {
        int value;

        if (FParse::Value(Cmd, TEXT("fov="), value)) {
            EnableFovRendering(value != 0);
        }

        if (FParse::Value(Cmd, TEXT("mirror="), value)) {
            switch (value) {
            case 0:
                m_windowMirrorMode = 0;
                break;
            case 2:
                m_windowMirrorMode = 2;
                break;
            default:
                m_windowMirrorMode = 1;
                break;
            }
        }

        return true;
    } else if (FParse::Command(&Cmd, TEXT("xhmd"))) {
        if (FParse::Command(&Cmd, TEXT("off"))) {
            EnableStereo(false);
            EnableHMD(false);
            return true;
        }
        EnableHMD(true);
        EnableStereo(true);
        float val;

        if (FParse::Command(&Cmd, TEXT("nofov"))) {
            EnableFovRendering(false);
        }

        if (FParse::Value(Cmd, TEXT("R="), val)) {
            ReloadProjectionMatrices();
            return true;
        }
        int setting = 0;
        if (FParse::Value(Cmd, TEXT("T="), setting)) {
            // TODO: New tracking.
            //switch (setting) {
            //case -1:
            //    m_lighthouseEnabled = false;
            //    m_lighthouseMerging = false;
            //    m_hmdPosTracking = false;
            //    m_disableInternalTracking = true;
            //    break;
            //case 0:
            //    VRgInitHMDPositionTracker(0, -1);
            //    m_lighthouseEnabled = false;
            //    m_lighthouseMerging = false;
            //    m_hmdPosTracking = false;
            //    m_disableInternalTracking = false;
            //    break;
            //case 1:
            //    if (m_vrgLighthouse) {
            //        if (!m_lighthouseInited) {

            //            m_lighthouseInited = VRgLinit();
            //        }
            //        VRgInitHMDPositionTracker(0, 0);
            //        m_lighthouseEnabled = m_lighthouseInited;
            //        m_hmdPosTracking = m_lighthouseInited;
            //        m_disableInternalTracking = false;
            //        m_lighthouseMerging = false;
            //    }
            //    break;
            //case 2:
            //    if (VRgInitHMDPositionTracker(0, 1) == 0) {
            //        m_lighthouseEnabled = true;
            //        m_lighthouseMerging = true;
            //        m_hmdPosTracking = true;
            //        m_disableInternalTracking = false;
            //    } else {
            //        MessageBox(0, L"Cannot init merging method.", L"Warning", 0);
            //    }
            //    break;
            //case 3:
            //    if (VRgInitHMDPositionTracker(0, 2) == 0) {
            //        m_lighthouseEnabled = false;
            //        m_lighthouseMerging = false;
            //        m_hmdPosTracking = true;
            //        m_disableInternalTracking = false;
            //    }
            //    break;
            //case 4:
            //    if (VRgInitHMDPositionTracker(0, 3) == 0) {
            //        m_lighthouseEnabled = false;
            //        m_lighthouseMerging = false;
            //        m_hmdPosTracking = true;
            //        m_disableInternalTracking = false;
            //    }
            //    break;
            //}

            return true;
        }

        if (FParse::Value(Cmd, TEXT("rc="), val)) {
            m_vrgSdk->SetOpenVrCompensation(-val);
            //if (VRgLset_compensation) {
            //    VRgLset_compensation(-val);
            //}
        }

        if (FParse::Value(Cmd, TEXT("wt="), setting)) {
            // TODO: Warp wait.
            //if (VRgsetWarpWait) {
            //    switch (setting) {
            //    case 0:
            //        VRgsetWarpWait(false, false);
            //        break;
            //    case 1:
            //        VRgsetWarpWait(true, false);
            //        break;
            //    case 2:
            //        VRgsetWarpWait(true, true);
            //        break;
            //    }
            //}
        }

        if (FParse::Value(Cmd, TEXT("pred="), setting)) {
            // TODO: Imu prediction.
            switch (setting) {
            case 0:
                //VRgIMUPrediction(false);
                MessageBox(0, L"Prediction disabled.", L"Message", 0);

                break;
            case 1:
                //VRgIMUPrediction(true);
                MessageBox(0, L"Prediction enabled.", L"Message", 0);
                break;
            }
            return true;
        }

        if (FParse::Value(Cmd, TEXT("s="), setting)) {
            // TODO: Test sync.
            return true;
        }
        if (FParse::Value(Cmd, TEXT("f="), setting)) {
            // TODO: Set focus.
            return true;

            //            if (VRgsetFocus) {
            //                switch (setting) {
            //                case 0:
            //                    VRgsetFocus(0, false);
            //#if UVER > 416
            //                    UE_LOG(LogHMD, Log, TEXT("Focus compensation disabled."));
            //#else
            //                    Ar.Logf(TEXT("Focus compensation disabled."));
            //#endif
            //                    break;
            //                case 1:
            //                    VRgsetFocus(0, true);
            //#if UVER > 416
            //                    UE_LOG(LogHMD, Log, TEXT("Focus compensation enabled."));
            //#else
            //                    Ar.Logf(TEXT("Focus compensation enabled."));
            //#endif
            //                    break;
            //                }
            //                return true;
            //            }
        }
        if (FParse::Value(Cmd, TEXT("timew="), setting)) {
            // TODO: Advanced timewarp settings?
            m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_ASYNC_TIMEWARP, setting == 0 ? 0 : 1);
            ReloadProjectionMatrices();
            return true;
        }
        if (FParse::Command(&Cmd, TEXT("c"))) {
            m_vrgSdk->vrgSetTrackingSystemOrigin(m_hmdId);
            return true;
        }
        if (FParse::Command(&Cmd, TEXT("ci"))) {
            m_vrgSdk->vrgCalibrateImu(m_hmdId);
            return true;
        }
        if (FParse::Command(&Cmd, TEXT("co"))) {
            if (!m_disableInternalTracking || m_lighthouseMerging) {
                m_vrgSdk->vrgCalibrateImu(m_hmdId);
            }
            if (m_lighthouseEnabled) {
                m_vrgSdk->vrgSetTrackingSystemOrigin(m_hmdId);
            }
            return true;
        }
        if (FParse::Command(&Cmd, TEXT("autoipd"))) {
            auto err = m_vrgSdk->vrgRunAutoInterpupillaryDistance(m_hmdId);
            if (err != VRG_ERROR_OK) {
                switch (err) {
                case VRG_ERROR_HMD_NOT_PRESENT:
                    Ar.Logf(TEXT("HMD not present."));
                    break;
                case VRG_ERROR_NOT_SUPPORTED:
                    Ar.Logf(TEXT("Auto IPD calibration not supported."));
                    break;
                case VRG_ERROR_IN_PROGRESS:
                    Ar.Logf(TEXT("IPD calibration in progress."));
                    break;
                default:
                    Ar.Logf(TEXT("Auto IPD calibration unknown error."));
                    break;
                }
            }
        }
        if (FParse::Value(Cmd, TEXT("il="), val)) {
            float val2;
            if (FParse::Value(Cmd, TEXT("ir="), val2)) {
                float ipd = val / 1000.0f;
                float ipdr = val2 / 1000.0f;
                //VRgsetHMDIPDinM(0, ipd, ipdr);
                // TODO: Set ipd.
            }
        }
    }
    if (FParse::Command(&Cmd, TEXT("STEREO"))) {
        if (FParse::Command(&Cmd, TEXT("ON"))) {
            if (!IsHMDEnabled()) {
                Ar.Logf(TEXT("vrgHMD is disabled. Use 'hmd enable' to re-enable it."));
            }
            EnableStereo(true);
            return true;
        } else if (FParse::Command(&Cmd, TEXT("OFF"))) {
            EnableStereo(false);
            return true;
        }

        float val;
        if (FParse::Value(Cmd, TEXT("E="), val)) {
            // TODO: IPD
        }
    } else if (FParse::Command(&Cmd, TEXT("HMD"))) {
        if (FParse::Command(&Cmd, TEXT("ENABLE"))) {
            EnableHMD(true);
            return true;
        } else if (FParse::Command(&Cmd, TEXT("DISABLE"))) {
            EnableHMD(false);
            return true;
        }

    } else if (FParse::Command(&Cmd, TEXT("UNCAPFPS"))) {
        GEngine->bSmoothFrameRate = false;
        return true;
    }

    return false;
}

bool FVRgHMD::IsHeadTrackingAllowed() const {
    return m_hmdId > -1 && !m_disableInternalTracking;
}

void FVRgHMD::ResetOrientationAndPosition(float yaw) {
    ResetOrientation(yaw);
    ResetPosition();
}

void FVRgHMD::ResetOrientation(float Yaw) {
    FRotator ViewRotation;
    ViewRotation = FRotator();
    ViewRotation.Pitch = 0;
    ViewRotation.Roll = 0;

    if (Yaw != 0.f) {
        ViewRotation.Yaw -= Yaw;
        ViewRotation.Normalize();
    }

    m_baseOrientation = ViewRotation.Quaternion();
}
void FVRgHMD::ResetPosition() {}

void FVRgHMD::SetClippingPlanes(float NCP, float FCP) {}

void FVRgHMD::SetBaseRotation(const FRotator& BaseRot) {}
FRotator FVRgHMD::GetBaseRotation() const {
    return FRotator::ZeroRotator;
}

void FVRgHMD::SetBaseOrientation(const FQuat& BaseOrient) {
    m_baseOrientation = BaseOrient;
}

FQuat FVRgHMD::GetBaseOrientation() const {
    return m_baseOrientation;
}

bool FVRgHMD::IsStereoEnabled() const {
    return m_stereoEnabled && m_hmdEnabled;
}

bool FVRgHMD::EnableStereo(bool enable) {
    if (!IsHMDEnabled()) {
        enable = false;
    }

    // Foveated rendering doesn't play nice with instanced stereo.
    EnableFovRendering(m_fovRendering);

    if (m_stereoEnabled != enable) {
        if (enable) {
            if (!Startup()) {
                return false;
            }
        } else {
            Shutdown();
        }

        m_stereoEnabled = enable;
    }

    auto* SceneVP = FindSceneViewport();
    if (SceneVP) {
        if (m_stereoEnabled) {
            UpdateResolution();

            SceneVP->SetViewportSize(static_cast<uint32>(m_baseLayer.resolution.width), static_cast<uint32>(m_baseLayer.resolution.height));

            FVector2D view;
            GEngine->GameViewport->GetViewportSize(view);
            FSystemResolution::RequestResolutionChange(view.X, view.Y, EWindowMode::Windowed);
        } else {
            TSharedPtr<SWindow> Window = SceneVP->FindWindow();
            if (Window.IsValid()) {
                FVector2D size = SceneVP->FindWindow()->GetSizeInScreen();
                SceneVP->SetViewportSize(size.X, size.Y);
                Window->SetViewportSizeDrivenByWindow(true); // true
            }
        }
    }

    return m_stereoEnabled;
}

void FVRgHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const {
    switch (StereoPass) {
    case eSSP_LEFT_EYE:
    case eSSP_RIGHT_EYE:
        SizeX = m_baseLayer.resolution.width;
        SizeY = m_baseLayer.resolution.height;
        if (StereoPass == eSSP_RIGHT_EYE) {
            X += SizeX;
        }
        break;
#if VRG_FOV_RENDERING
    case eSSP_LEFT_EYE_SIDE:
    case eSSP_RIGHT_EYE_SIDE:
        if (!m_fovRendering) {
            SizeX = 0;
            SizeY = 0;
            X = 0;
            Y = 0;
        } else {
            SizeX = m_detailLayer.resolution.width;
            SizeY = m_detailLayer.resolution.height;
            Y += m_baseLayer.resolution.height;
            if (StereoPass == eSSP_RIGHT_EYE_SIDE) {
                X += SizeX;
            }
        }
        break;
#endif // VRG_FOV_RENDERING
    }
}

void FVRgHMD::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) {}

void FVRgHMD::RenderTexture_RenderThread(
    FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer, FRHITexture2D* SrcTexture, FVector2D WindowSize) const {
    check(IsInRenderingThread());

    if (m_windowMirrorMode == 0) {
        return;
    }

    auto ViewportSize = GetViewportSize(WindowSize);
    const uint32 ViewportWidth = ViewportSize.X;
    const uint32 ViewportHeight = ViewportSize.Y;

#if UVER >= 422
    FRHIRenderPassInfo RPInfo(BackBuffer, ERenderTargetActions::DontLoad_Store, FTextureRHIRef());
    RHICmdList.BeginRenderPass(RPInfo, TEXT("VrgHmdRenderWindow"));
#else
    SetRenderTarget(RHICmdList, BackBuffer, FTextureRHIRef());
#endif
    {
        RHICmdList.SetViewport(0, 0, 0, ViewportWidth, ViewportHeight, 1.0f);

        const auto FeatureLevel = GMaxRHIFeatureLevel;
        auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

        // Mirror mode.
        TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
        TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

        FGraphicsPipelineStateInitializer GraphicsPSOInit;
        RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
        GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
        GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
        GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
        GraphicsPSOInit.PrimitiveType = PT_TriangleList;

#if UVER >= 422
        GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
#else
        GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = m_rendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI;
#endif

#if UVER >= 425
        GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
        GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
#else
        GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
        GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
#endif

        SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

        PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);

        float u = 0.2f;
        float um = 0.3f;
        float v = 0.2f;
        float vm = 0.6f;

        const float width = m_baseLayer.right.um - m_baseLayer.left.u;
        const float height = m_baseLayer.left.vm - m_baseLayer.left.v;

        u = m_baseLayer.left.u + u * width;
        um = m_baseLayer.left.u + um * width;
        v = m_baseLayer.left.v + v * height;
        vm = m_baseLayer.left.v + vm * height;

#if UVER >= 425
        if (m_windowMirrorMode == 1) {
            m_rendererModule->DrawRectangle(RHICmdList, 0, 0, ViewportWidth, ViewportHeight, u, v, um, vm, FIntPoint(ViewportWidth, ViewportHeight),
                FIntPoint(1, 1), VertexShader, EDRF_Default);
        } else if (m_windowMirrorMode == 2) {
            m_rendererModule->DrawRectangle(RHICmdList, 0, 0, ViewportWidth, ViewportHeight, 0.0f, 0.0f, 1.0f, 1.0f, FIntPoint(ViewportWidth, ViewportHeight),
                FIntPoint(1, 1), VertexShader, EDRF_Default);
        }
#else
        if (m_windowMirrorMode == 1) {
            m_rendererModule->DrawRectangle(RHICmdList, 0, 0, ViewportWidth, ViewportHeight, u, v, um, vm, FIntPoint(ViewportWidth, ViewportHeight),
                FIntPoint(1, 1), *VertexShader, EDRF_Default);
        } else if (m_windowMirrorMode == 2) {
            m_rendererModule->DrawRectangle(RHICmdList, 0, 0, ViewportWidth, ViewportHeight, 0.0f, 0.0f, 1.0f, 1.0f, FIntPoint(ViewportWidth, ViewportHeight),
                FIntPoint(1, 1), *VertexShader, EDRF_Default);
        }
#endif
    }

#if UVER >= 422
    RHICmdList.EndRenderPass();
#endif
}

void FVRgHMD::CalculateStereoViewOffset(const enum EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation) {
    FHeadMountedDisplayBase::CalculateStereoViewOffset(StereoPassType, ViewRotation, WorldToMeters, ViewLocation);
}

#if UVER >= 418
void FVRgHMD::UpdateViewportRHIBridge(bool, const class FViewport& Viewport, FRHIViewport* const ViewportRHI) {
    check(IsInGameThread());

    GetActiveRHIBridgeImpl()->UpdateViewport(Viewport, ViewportRHI, &m_baseLayer, m_fovRendering ? &m_detailLayer : nullptr);
}
#endif

void FVRgHMD::UpdateViewport(bool bUseSeparateRenderTarget, const class FViewport& Viewport, class SViewport* ViewportWidget) {
    UpdateResolution();

    FXRRenderTargetManager::UpdateViewport(bUseSeparateRenderTarget, Viewport, ViewportWidget);
}

FMatrix FVRgHMD::GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType) const {
    check(IsStereoEnabled());

    ReloadProjectionMatrices();

    switch (StereoPassType) {
    case EStereoscopicPass::eSSP_LEFT_EYE:
        return m_matrixLeft;
    case EStereoscopicPass::eSSP_RIGHT_EYE:
        return m_matrixRight;
#if VRG_FOV_RENDERING
    case EStereoscopicPass::eSSP_LEFT_EYE_SIDE:
        return m_matrixLeftDetail;
    case EStereoscopicPass::eSSP_RIGHT_EYE_SIDE:
#endif // VRG_FOV_RENDERING
    default:
        return m_matrixRightDetail;
    }
}

void FVRgHMD::InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) {
    if (m_infinitusSimulationMode) {
        m_canvas = Canvas;
    }

    if (m_canvas) {
        FText SomeText = FText::FromString("VRg test mode.");
        FLinearColor TheFontColour = FLinearColor(1.0f, 1.0f, 1.0f);
        FCanvasTextItem NewText(FVector2D(0, 0), SomeText, GEngine->GetSubtitleFont(), TheFontColour);
        NewText.Scale.Set(10, 10);
        m_canvas->DrawItem(NewText);
    }
}

FVRgHMD::FVRgCustomPresent* FVRgHMD::GetActiveRHIBridgeImpl() {
    return m_rhiBridge;
}

const FVRgHMD::FVRgCustomPresent* FVRgHMD::GetActiveRHIBridgeImpl() const {
    return m_rhiBridge;
}

FVRgHMD::FVRgHMD(IVRgPlugin* m_vrgPlugin)
#if UVER >= 422
    : FHeadMountedDisplayBase(nullptr)
    ,
#else
    :
#endif
    m_vrgPlugin(m_vrgPlugin) {

#if VRG_FOV_RENDERING
    m_fovRendering = true;
#else
    m_fovRendering = false;
#endif // VRG_FOV_RENDERING

    m_initialized = LoadModule();
    m_infinitusSimulationMode = false;
}

FVRgHMD::~FVRgHMD() {
    if (m_initialized) {
        UnloadModule();
    }
}

bool FVRgHMD::IsInitialized() const {
    return m_initialized;
}

bool FVRgHMD::Startup() {
    static const FName RendererModuleName("Renderer");
    m_rendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);

    uint32_t hmdCount = 1;
    VrgDevice vrgDevice;

    auto error = m_vrgSdk->vrgEnumerateDevices(&hmdCount, &vrgDevice);
    if ((error != VRG_ERROR_OK && error != VRG_ERROR_INCOMPLETE) || hmdCount == 0) {
        ShowError(TEXT("No HMD found."));
        return false;
    }

    m_hmdId = vrgDevice.id;
    error = m_vrgSdk->vrgInitDevice(m_hmdId);

    if (error != VRG_ERROR_OK) {
        HandleError(m_vrgSdk.Get(), error);
        m_hmdId = -1;
        return false;
    }

    ReloadProjectionMatrices();

    m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_TRAY_MENU, 1);
    m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_ASYNC_TIMEWARP, 1);
    m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_STATUS_MESSAGES, 1);
    m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_DYNAMIC_RENDER_TARGET_RESOLUTION, 1);
    //m_vrgSdk->vrgSetBool(m_hmdId, VRG_BOOL_TRACKERS_AS_CONTROLLERS, 1);

    UpdateResolution();

    float IdealScreenPercentage = 100.0f;
    static IConsoleVariable* CScrPercVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));

    if (FMath::RoundToInt(CScrPercVar->GetFloat()) != FMath::RoundToInt(IdealScreenPercentage)) {
        CScrPercVar->Set(IdealScreenPercentage);
    }

    static IConsoleVariable* CVSyncVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
    CVSyncVar->Set(false);

    static IConsoleVariable* CFCFVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.finishcurrentframe"));
    CFCFVar->Set(false);

    GEngine->bSmoothFrameRate = false;

#if PLATFORM_WINDOWS
    if (IsPCPlatform(GMaxRHIShaderPlatform) && !IsOpenGLPlatform(GMaxRHIShaderPlatform)) {
        m_rhiBridge = new FVrgD3D11CustomPresent(this, m_vrgSdk.Get(), m_hmdId);
    }
#endif

    CreateSpectatorScreenController();

    return true;
}

void FVRgHMD::CreateSpectatorScreenController() {
    SpectatorScreenController = MakeUnique<FDefaultSpectatorScreenController>(this);
}

#if UVER >= 421
void FVRgHMD::CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* SrcTexture, FIntRect SrcRect, FRHITexture2D* DstTexture,
    FIntRect DstRect, bool bClearBlack, bool bNoAlpha) const {
#else
void FVRgHMD::CopyTexture_RenderThread(
    FRHICommandListImmediate& RHICmdList, FRHITexture2D* SrcTexture, FIntRect SrcRect, FRHITexture2D* DstTexture, FIntRect DstRect, bool bClearBlack) const {
#endif
    check(IsInRenderingThread());

    const uint32 ViewportWidth = DstRect.Width();
    const uint32 ViewportHeight = DstRect.Height();
    const FIntPoint TargetSize(ViewportWidth, ViewportHeight);

    const float SrcTextureWidth = SrcTexture->GetSizeX();
    const float SrcTextureHeight = SrcTexture->GetSizeY();
    float U = 0.f, V = 0.f, USize = 1.f, VSize = 1.f;
    if (!SrcRect.IsEmpty()) {
        U = SrcRect.Min.X / SrcTextureWidth;
        V = SrcRect.Min.Y / SrcTextureHeight;
        USize = SrcRect.Width() / SrcTextureWidth;
        VSize = SrcRect.Height() / SrcTextureHeight;
    }

#if UVER >= 422
    FRHIRenderPassInfo RPInfo(DstTexture, ERenderTargetActions::DontLoad_Store, FTextureRHIRef());
    RHICmdList.BeginRenderPass(RPInfo, TEXT("VrgHmdCopyTexture"));
#else
    SetRenderTarget(RHICmdList, DstTexture, FTextureRHIRef());
#endif

    {
        if (bClearBlack) {
            const FIntRect ClearRect(0, 0, DstTexture->GetSizeX(), DstTexture->GetSizeY());
            RHICmdList.SetViewport(ClearRect.Min.X, ClearRect.Min.Y, 0, ClearRect.Max.X, ClearRect.Max.Y, 1.0f);
        }

        RHICmdList.SetViewport(DstRect.Min.X, DstRect.Min.Y, 0, DstRect.Max.X, DstRect.Max.Y, 1.0f);

        FGraphicsPipelineStateInitializer GraphicsPSOInit;
        RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
        GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
        GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
        GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
        GraphicsPSOInit.PrimitiveType = PT_TriangleList;

        const auto FeatureLevel = GMaxRHIFeatureLevel;
        auto ShaderMap = GetGlobalShaderMap(FeatureLevel);

        TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
        TShaderMapRef<FScreenPS> PixelShader(ShaderMap);

#if UVER >= 422
        GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
#else
        GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = m_rendererModule->GetFilterVertexDeclaration().VertexDeclarationRHI;
#endif

#if UVER >= 425
        GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
        GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
#else
        GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
        GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
#endif

        const bool bSameSize = DstRect.Size() == SrcRect.Size();
        if (bSameSize) {
            PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Point>::GetRHI(), SrcTexture);
        } else {
            PixelShader->SetParameters(RHICmdList, TStaticSamplerState<SF_Bilinear>::GetRHI(), SrcTexture);
        }

#if UVER >= 425
        m_rendererModule->DrawRectangle(
            RHICmdList, 0, 0, ViewportWidth, ViewportHeight, U, V, USize, VSize, TargetSize, FIntPoint(1, 1), VertexShader, EDRF_Default);
#else
        m_rendererModule->DrawRectangle(
            RHICmdList, 0, 0, ViewportWidth, ViewportHeight, U, V, USize, VSize, TargetSize, FIntPoint(1, 1), *VertexShader, EDRF_Default);
#endif
    }

#if UVER >= 422
    RHICmdList.EndRenderPass();
#endif
}

void FVRgHMD::Shutdown() {
    if (m_hmdId > -1) {
        m_vrgSdk->vrgFreeDevice(m_hmdId);
        m_hmdId = -1;
    }
}

bool FVRgHMD::LoadModule() {
    void* vrgModuleHandle = nullptr;

#if PLATFORM_WINDOWS && PLATFORM_64BITS
    FString RootPath = FPaths::ProjectPluginsDir() + "/vrgHMD/ThirdParty/";

    FPlatformProcess::PushDllDirectory(*RootPath);

    vrgModuleHandle = FPlatformProcess::GetDllHandle(*(RootPath + "hmd64.dll"));
    m_vrgLighthouse = nullptr;

    FPlatformProcess::PopDllDirectory(*RootPath);
#endif // PLATFORM_WINDOWS && PLATFORM_64BITS

    if (!vrgModuleHandle) {
        ShowError(TEXT("Failed to load hmd64.dll or it's dependencies."));
        return false;
    }

    m_vrgSdk = VrgSdk::Init(vrgModuleHandle);
    if (!m_vrgSdk) {

        ShowErrorStr(FString::Printf(TEXT("Failed to initialize VRG API %d.%d."), VrgSdk::Version.majorVersion, VrgSdk::Version.minorVersion));
        return false;
    }

    // Pro controllery je nutne inicializovat rovnou.
    m_vrgSdk->m_lighthouseInited = m_vrgSdk->InternalApi->InitOpenVr() == VRG_ERROR_OK;
    if (!m_vrgSdk->m_lighthouseInited) {
        UE_LOG(LogHMD, Error, TEXT("Failed to initialize OpenVR."));
    }

    return true;
}

void FVRgHMD::UnloadModule() {
    if (m_vrgSdk) {
        m_vrgSdk->vrgFreeApi();
        m_vrgSdk = nullptr;
        m_initialized = false;
    }
}

void FVRgHMD::OnEndPlay(FWorldContext& InWorldContext) {
    EnableStereo(false);
}

EHMDWornState::Type FVRgHMD::GetHMDWornState() {
    return EHMDWornState::Worn;
}

bool FVRgConsole::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) {
    FVRgHMD* hmd = static_cast<FVRgHMD*>(GetVRgHMD());
    return hmd ? hmd->Exec(InWorld, Cmd, Ar) : false;
}

bool FVRgHMD::StartRecordEyes(FString name) {
    return m_vrgSdk->initRecordHMDEyeCameraImage(0, TCHAR_TO_ANSI(*name)) == 0;
}

void FVRgHMD::StopRecordEyes() {
    m_vrgSdk->stopRecordHMDEyeCameraImage(0);
}

bool FVRgHMD::CalibrateEyes() {
    return m_vrgSdk->startHMDEyeCameraCalibration(0);
}

int FVRgHMD::IsCalibrating() {
    return m_vrgSdk->isHMDEyeCameraCalibrating(0);
}

void FVRgHMD::AutoIPD() {
    m_vrgSdk->vrgRunAutoInterpupillaryDistance(m_hmdId);
}

void FVRgHMD::UpdateResolution(bool forceUpdate) {
    VrgResolution baseResolution;

    m_vrgSdk->vrgGetInt(m_hmdId, VRG_INT_RENDER_TARGET_WIDTH, &baseResolution.width);
    m_vrgSdk->vrgGetInt(m_hmdId, VRG_INT_RENDER_TARGET_HEIGHT, &baseResolution.height);

    if (m_fovRendering) {
        VrgResolution detailResolution;

        m_vrgSdk->vrgGetInt(m_hmdId, VRG_INT_RENDER_TARGET_DETAIL_WIDTH, &detailResolution.width);
        m_vrgSdk->vrgGetInt(m_hmdId, VRG_INT_RENDER_TARGET_DETAIL_HEIGHT, &detailResolution.height);

        if (!forceUpdate && Equal(m_baseLayer.resolution, baseResolution) && Equal(m_detailLayer.resolution, detailResolution)) {
            return;
        }

        m_baseLayer.resolution = baseResolution;
        m_detailLayer.resolution = detailResolution;

        const auto maxWidth = std::max(m_baseLayer.resolution.width, m_detailLayer.resolution.width);

        m_resolution.width = maxWidth * 2;
        m_resolution.height = m_baseLayer.resolution.height + m_detailLayer.resolution.height;

        m_baseLayer.left.u = 0;
        m_baseLayer.left.um = m_baseLayer.right.u = static_cast<float>(m_baseLayer.resolution.width) / static_cast<float>(m_resolution.width);
        m_baseLayer.right.um = 2 * m_baseLayer.left.um;
        m_baseLayer.left.v = m_baseLayer.right.v = 0;
        m_baseLayer.left.vm = m_baseLayer.right.vm = static_cast<float>(m_baseLayer.resolution.height) / static_cast<float>(m_resolution.height);

        m_detailLayer.left.u = 0;
        m_detailLayer.left.um = m_detailLayer.right.u = static_cast<float>(m_detailLayer.resolution.width) / static_cast<float>(m_resolution.width);
        m_detailLayer.right.um = 2 * m_detailLayer.left.um;
        m_detailLayer.left.v = m_detailLayer.right.v = m_baseLayer.left.vm;
        m_detailLayer.left.vm = m_detailLayer.right.vm = 1;

    } else if (forceUpdate || !Equal(m_baseLayer.resolution, baseResolution)) {
        m_baseLayer.resolution = baseResolution;

        m_resolution.width = m_baseLayer.resolution.width * 2;
        m_resolution.height = m_baseLayer.resolution.height;

        m_baseLayer.left.u = 0;
        m_baseLayer.left.v = 0;
        m_baseLayer.left.um = 0.5f;
        m_baseLayer.left.vm = 1;

        m_baseLayer.right.u = 0.5f;
        m_baseLayer.right.v = 0;
        m_baseLayer.right.um = 1;
        m_baseLayer.right.vm = 1;
    }
}

bool FVRgHMD::IsInstancedStereoEnabled() const {
    static const auto InstancedStereoCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.InstancedStereo"));
    return (InstancedStereoCVar && InstancedStereoCVar->GetValueOnAnyThread() != 0);
}

void FVRgHMD::EnableFovRendering(bool enable) {
#if VRG_FOV_RENDERING
    bool wasEnabled = m_fovRendering;

    if (enable && IsInstancedStereoEnabled()) {
        UE_LOG(LogHMD, Warning,
            TEXT("VR instanced stereo is enabled which prevents foveated rendering to function properly."
                 "Please turn off foveated rendering or instanced stereo (Project Settings -> Rendering -> VR -> Instanced stereo)."));
        ShowError(TEXT("Instanced stereo detected, disabling foveated rendering."));
        m_fovRendering = false;
    } else {
        m_fovRendering = enable;

        if (m_fovRendering) {
            UE_LOG(LogHMD, Log, TEXT("Foveated rendering on."));
        } else {
            UE_LOG(LogHMD, Log, TEXT("Foveated rendering off."));
        }
    }

    if (wasEnabled != enable) {
        UpdateResolution(true);
        ReloadProjectionMatrices();
    }
#endif // VRG_FOV_RENDERING
}

#if VRG_FOV_RENDERING
EStereoscopicPass FVRgHMD::GetViewPassForIndex(bool bStereoRequested, uint32 ViewIndex) const {
    if (bStereoRequested) {
        switch (ViewIndex) {
        case 0:
            return eSSP_LEFT_EYE;
        case 1:
            return eSSP_RIGHT_EYE;
        case 2:
            return eSSP_LEFT_EYE_SIDE;
        case 3:
            return eSSP_RIGHT_EYE_SIDE;
        }
    }

    return EStereoscopicPass::eSSP_FULL;
}

uint32 FVRgHMD::GetViewIndexForPass(EStereoscopicPass StereoPassType) const {
    switch (StereoPassType) {
    case eSSP_LEFT_EYE:
    case eSSP_FULL:
        return 0;
    case eSSP_RIGHT_EYE:
        return 1;
    case eSSP_LEFT_EYE_SIDE:
        return 2;
    case eSSP_RIGHT_EYE_SIDE:
        return 3;
    default:
        check(0);
        return -1;
    }
}

int32 FVRgHMD::GetDesiredNumberOfViews(bool bStereoRequested) const {
    return bStereoRequested ? 4 : 1;
}

#endif // VRG_FOV_RENDERING

FVector2D FVRgHMD::GetViewportSize(FVector2D WindowSize) const {
#if UVER >= 424
    return FVector2D{ WindowSize.X, WindowSize.Y };
#else // UVER >= 418
    return FVector2D{ static_cast<float>(m_baseLayer.resolution.width), static_cast<float>(m_baseLayer.resolution.height) };
#endif
}
