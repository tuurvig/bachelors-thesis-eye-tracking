// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// 2016-8 vrgineers, Inc.

// TODO: Remove
#define DO_CHECK 1
#define DO_GUARD_SLOW 0

#pragma once

#include "../Public/IVRgPlugin.h"

#include "HeadMountedDisplay.h"
#include "IHeadMountedDisplay.h"

#if PLATFORM_WINDOWS
#include "../Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include <d3d11.h>
#include "../Core/Public/Windows/HideWindowsPlatformTypes.h"
#endif

#include "SceneViewExtension.h"
#include "Runtime/Renderer/Private/RendererPrivate.h"
#include "HeadMountedDisplayBase.h"
#include "XRRenderTargetManager.h"

#include "VrgSdk.h"

#define VRG_FOV_RENDERING (UVER >= 422)

struct VrgResolution {
    int width = -1;
    int height = -1;
};

struct VrgTexCoords {
    float u = 0;
    float v = 0;
    float um = 1;
    float vm = 1;
};

struct VrgLayer {
    VrgResolution resolution;
    VrgTexCoords left;
    VrgTexCoords right;
};

class FVRgConsole : private FSelfRegisteringExec {
private:
    void* GetVRgHMD() const {
        static FName SystemName(TEXT("VRgHMD"));
        if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName)) {
            return (GEngine->XRSystem.Get());
        }

        return nullptr;
    }
    bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
};

/**
 * VRg Head Mounted Display
 */
class FVRgHMD :
#if UVER >= 417
    public FHeadMountedDisplayBase,
#else
    public IHeadMountedDisplay,
#endif

#if UVER >= 418
    public FXRRenderTargetManager,
#endif
    public TSharedFromThis<FVRgHMD, ESPMode::ThreadSafe> {

public:
    explicit FVRgHMD(IVRgPlugin* m_vrgPlugin);
    ~FVRgHMD();

    /** IHeadMountedDisplay interface */
#if UVER < 418
    virtual void RefreshPoses() override {}

    virtual FName GetDeviceName() const override //v14
    {
        static FName DefaultName(TEXT("VRgHMD"));
        return DefaultName;
    }
#endif

#if UVER >= 426
    virtual int32 GetXRSystemFlags() const override {
        return EXRSystemFlags::IsHeadMounted;
    }
#endif

#if UVER >= 418
    virtual FName GetSystemName() const override {
        static FName DefaultName(TEXT("VRgHMD"));
        return DefaultName;
    }
    virtual FString GetVersionString() const override;

    virtual class IHeadMountedDisplay* GetHMDDevice() override {
        return this;
    }

    virtual class TSharedPtr<class IStereoRendering, ESPMode::ThreadSafe> GetStereoRenderingDevice() override {
        return AsShared();
    }

    virtual bool
    OnStartGameFrame(FWorldContext& WorldContext) override;
#endif

    virtual bool IsHMDConnected() override {
        return true;
    }

    virtual bool IsHMDEnabled() const override;
    virtual EHMDWornState::Type GetHMDWornState() override;
    virtual void EnableHMD(bool allow = true) override;
    virtual bool GetHMDMonitorInfo(MonitorInfo&) override;
    virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;
    virtual void RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const override;
    virtual bool DoesSupportPositionalTracking() const override {
        return true;
    }

    virtual bool HasValidTrackingPosition() override;
    virtual bool EnumerateTrackedDevices(TArray<int32>& TrackedIds, EXRTrackedDeviceType DeviceType = EXRTrackedDeviceType::Any) override;
    virtual bool GetTrackingSensorProperties(int32 InDeviceId, FQuat& OutOrientation, FVector& OutOrigin, FXRSensorProperties& OutSensorProperties) override;

    virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
    virtual float GetInterpupillaryDistance() const override;

    virtual bool IsChromaAbCorrectionEnabled() const override;

    virtual bool HasHiddenAreaMesh() const override {
        return false;
    }
    virtual bool HasVisibleAreaMesh() const override {
        return false;
    }

    virtual bool IsHeadTrackingAllowed() const override;

    virtual void ResetOrientationAndPosition(float yaw = 0.f) override;
    virtual void ResetOrientation(float Yaw = 0.f) override;
    virtual void ResetPosition() override;

    virtual void SetClippingPlanes(float NCP, float FCP) override;

    virtual FRotator GetBaseRotation() const override;
    virtual void SetBaseRotation(const FRotator& BaseRot) override;
    virtual FQuat GetBaseOrientation() const override;
    virtual void SetBaseOrientation(const FQuat& BaseOrient) override;

    virtual void DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) override;
    virtual void UpdateScreenSettings(const FViewport* InViewport) override {}
    virtual void OnEndPlay(FWorldContext& InWorldContext) override; //v14

    virtual bool DoesAppUseVRFocus() const override {
        return false;
    }
    virtual bool DoesAppHaveVRFocus() const override {
        return false;
    }

    /** IStereoRendering interface */
    virtual bool IsStereoEnabled() const override;
    virtual bool EnableStereo(bool stereo = true) override;
    virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;

    virtual void CalculateStereoViewOffset(
        const EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float MetersToWorld, FVector& ViewLocation) override;
    virtual FMatrix GetStereoProjectionMatrix(const enum EStereoscopicPass StereoPassType) const override;

    virtual void InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) override;

    virtual void RenderTexture_RenderThread(
        FRHICommandListImmediate& RHICmdList, FRHITexture2D* BackBuffer, FRHITexture2D* SrcTexture, FVector2D WindowSize) const override;

    //virtual void GetEyeRenderParams_RenderThread(
    //    const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;

    virtual IStereoRenderTargetManager* GetRenderTargetManager() override {
        return this;
    }

    /** FXRRenderTargetManager interface */
    virtual void UpdateViewportRHIBridge(bool bUseSeparateRenderTarget, const class FViewport& Viewport, FRHIViewport* const ViewportRHI) override;
    virtual bool ShouldUseSeparateRenderTarget() const override {
        check(IsInGameThread());
        return IsStereoEnabled();
    }
    virtual void UpdateViewport(bool bUseSeparateRenderTarget, const class FViewport& Viewport, class SViewport* ViewportWidget) override;

#if UVER >= 419
    virtual FIntPoint GetIdealRenderTargetSize() const override;

    virtual bool GetHMDDistortionEnabled(EShadingPath ShadingPath) const override;
#else
    void CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;

    virtual bool GetHMDDistortionEnabled() const override;

    virtual void RefreshPoses() override {}

    virtual EHMDDeviceType::Type GetHMDDeviceType() const override {
        return EHMDDeviceType::Type::DT_SteamVR; // TODO: ?
    }
#endif

#if UVER >= 426
    virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags InTexFlags,
        ETextureCreateFlags InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture,
        uint32 NumSamples = 1) override;
#else
    virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 InTexFlags,
        uint32 InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
#endif

#if UVER >= 421
    virtual void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* SrcTexture, FIntRect SrcRect, FRHITexture2D* DstTexture,
        FIntRect DstRect, bool bClearBlack, bool bNoAlpha) const override;
#else
    virtual void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* SrcTexture, FIntRect SrcRect, FRHITexture2D* DstTexture,
        FIntRect DstRect, bool bClearBlack) const override;
#endif

    bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);

    class FVRgCustomPresent : public FRHICustomPresent {
    public:
        FVRgCustomPresent(FVRgHMD* plugin)
#if UVER >= 420
            : FRHICustomPresent()
#else
            : FRHICustomPresent(nullptr)
#endif
            , Plugin(plugin)
            , bNeedReinitRendererAPI(true)
            , bInitialized(false) {
        }

        bool IsInitialized() const {
            return bInitialized;
        }

        virtual void BeginRendering() = 0;
        virtual void FinishRendering() = 0;
        virtual void UpdateViewport(const FViewport& Viewport, FRHIViewport* ViewportRHI, const VrgLayer* baseLayer, const VrgLayer* detailLayer = nullptr) = 0;
        virtual void Reset() = 0;
        virtual void Shutdown() = 0;

        virtual void SetPose(
            const VrgPose& pose, const VrgPose& leftCamPose, const VrgPose& rightCamPose, const VrgPose& leftDetailCamPose, const VrgPose& rightDetailCamPose)
            = 0;
        virtual FIntPoint TextureSizeLimit() const = 0;

        virtual bool NeedsNativePresent() override {
            return true;
        }

        virtual void SetNeedReinitRendererAPI() {
            bNeedReinitRendererAPI = true;
        }

    protected:
        FVRgHMD* Plugin;
        bool bNeedReinitRendererAPI;
        bool bInitialized;
    };

    FVRgCustomPresent* GetActiveRHIBridgeImpl();
    const FVRgCustomPresent* GetActiveRHIBridgeImpl() const;

    int32 GetWindowMirrorMode() const {
        return m_windowMirrorMode;
    }

    bool IsInitialized() const;

    virtual bool GetCurrentPose(int32 DeviceId, FQuat& CurrentOrientation, FVector& CurrentPosition) override;
    virtual bool GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) override;
    virtual bool IsTracking(int32 DeviceId) override;

    bool StartRecordEyes(FString name);
    void StopRecordEyes();
    bool CalibrateEyes();
    void AutoIPD();
    int IsCalibrating();

    VrgSdk* GetSdk() {
        return m_vrgSdk.Get();
    }

#if VRG_FOV_RENDERING
    virtual int32 GetDesiredNumberOfViews(bool bStereoRequested) const override;
    virtual EStereoscopicPass GetViewPassForIndex(bool bStereoRequested, uint32 ViewIndex) const override;
    virtual uint32 GetViewIndexForPass(EStereoscopicPass StereoPassType) const override;
#endif // VRG_FOV_RENDERING

private:
    void CreateSpectatorScreenController();

    bool Startup();
    void Shutdown();
    bool LoadModule();
    void UnloadModule();

    float GetWorldToMetersScale() const;

    void UpdateProjectionMatrix(FMatrix& matrix, const VrgFov& fov) const;
    void ReloadProjectionMatrices() const;

    void UpdateResolution(bool forceUpdate = false);

    bool IsInstancedStereoEnabled() const;
    void EnableFovRendering(bool enable);

    FVector2D GetViewportSize(FVector2D WindowSize) const;

    // Flags.
    bool m_initialized = false;
    bool m_hmdEnabled = true;
    bool m_stereoEnabled = false;
    bool m_infinitusSimulationMode = false;
    bool m_hmdPosTracking = true;
    bool m_disableInternalTracking = false;

    // Sdk.
    int m_hmdId = -1;

    TUniquePtr<VrgSdk> m_vrgSdk;

    // Config.
    int32 m_windowMirrorMode = 1;

    FVRgConsole m_console;
    UCanvas* m_canvas = nullptr;
    FQuat m_baseOrientation = FQuat::Identity;

    IRendererModule* m_rendererModule = nullptr;
    IVRgPlugin* m_vrgPlugin = nullptr;

    // Rendering.
    VrgResolution m_resolution;
    VrgLayer m_baseLayer;
    VrgLayer m_detailLayer;

    TRefCountPtr<FVRgCustomPresent> m_rhiBridge;

    // Projection.
    mutable VrgFov m_fovLeft;
    mutable VrgFov m_fovRight;
    mutable VrgFov m_fovLeftDetail;
    mutable VrgFov m_fovRightDetail;

    mutable FMatrix m_matrixLeft;
    mutable FMatrix m_matrixRight;
    mutable FMatrix m_matrixLeftDetail;
    mutable FMatrix m_matrixRightDetail;

    VrgPose m_leftCameraPose;
    VrgPose m_rightCameraPose;
    VrgPose m_leftDetailCameraPose;
    VrgPose m_rightDetailCameraPose;

    float m_worldToMetersScale = 100.0f;

    // Lighthouse.
    void* m_vrgLighthouse = nullptr;
    bool m_lighthouseEnabled = false;
    bool m_lighthouseMerging = false;

    bool m_fovRendering = false;
};
