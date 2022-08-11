// Copyright 1998 - 2017 Epic Games, Inc.All Rights Reserved.

#include "../../SteamVRController/Public/ISteamVRControllerPlugin2.h"

#include "../../vrgHMD/Public/IVRgPlugin.h"
#include "../InputDevice/Public/IInputDevice.h"

#include "../InputDevice/Public/IHapticDevice.h"
#include "IMotionController.h"
#include "Engine/Engine.h"
#include "EngineGlobals.h"
#include "GenericPlatform/IInputInterface.h"

#include "../../vrgHMD/Private/VRgHMD.h"

#if UVER >= 419
#include "XRMotionControllerBase.h"
#endif

#if UVER >= 424
#include "../InputCore/Classes/InputCoreTypes.h"
#endif

#define STEAMVRCONTROLLER_SUPPORTED_PLATFORMS 1

#define STEAMVR_SUPPORTED_PLATFORMS 0

#include "../Classes/SteamVRControllerLibrary.h"
#define LOCTEXT_NAMESPACE "SteamVRController"

#if PLATFORM_WINDOWS
#include "../Core/Public/Windows/WindowsHWrapper.h"
#endif

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
#include "../../../openvr.h"
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

DEFINE_LOG_CATEGORY_STATIC(LogSteamVRController, Log, All);

/** Total number of controllers in a set */
#define CONTROLLERS_PER_PLAYER 2

/** Controller axis mappings. @todo steamvr: should enumerate rather than hard code */
#define TOUCHPAD_AXIS 0
#define TRIGGER_AXIS 1
#if UVER >= 418
#define KNUCKLES_TOTAL_HAND_GRIP_AXIS 2
#define KNUCKLES_UPPER_HAND_GRIP_AXIS 3
#define KNUCKLES_LOWER_HAND_GRIP_AXIS 4
#endif
#define DOT_45DEG 0.7071f

//
// Gamepad thresholds
//
#define TOUCHPAD_DEADZONE 0.0f

// Controls whether or not we need to swap the input routing for the hands, for debugging
static TAutoConsoleVariable<int32> CVarSwapHands(TEXT("vr.SwapMotionControllerInput"), 0,
    TEXT("This command allows you to swap the button / axis input handedness for the input controller, for debugging purposes.\n")
        TEXT(" 0: don't swap (default)\n") TEXT(" 1: swap left and right buttons"),
    ECVF_Cheat);

namespace SteamVRControllerKeyNames {
const FGamepadKeyNames::Type Touch0("Steam_Touch_0");
const FGamepadKeyNames::Type Touch1("Steam_Touch_1");
#if UVER >= 418
const FGamepadKeyNames::Type GenericGrip("Steam_Generic_Grip");
const FGamepadKeyNames::Type GenericTrigger("Steam_Generic_Trigger");
const FGamepadKeyNames::Type GenericTouchpad("Steam_Generic_Touchpad");
const FGamepadKeyNames::Type GenericMenu("Steam_Generic_Menu");
const FGamepadKeyNames::Type GenericSystem("Steam_Generic_System");

const FGamepadKeyNames::Type SteamVR_Knuckles_Left_HandGrip("SteamVR_Knuckles_Left_HandGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Left_IndexGrip("SteamVR_Knuckles_Left_IndexGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Left_MiddleGrip("SteamVR_Knuckles_Left_MiddleGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Left_RingGrip("SteamVR_Knuckles_Left_RingGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Left_PinkyGrip("SteamVR_Knuckles_Left_PinkyGrip");

const FGamepadKeyNames::Type SteamVR_Knuckles_Right_HandGrip("SteamVR_Knuckles_Right_HandGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Right_IndexGrip("SteamVR_Knuckles_Right_IndexGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Right_MiddleGrip("SteamVR_Knuckles_Right_MiddleGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Right_RingGrip("SteamVR_Knuckles_Right_RingGrip");
const FGamepadKeyNames::Type SteamVR_Knuckles_Right_PinkyGrip("SteamVR_Knuckles_Right_PinkyGrip");
#endif
} // namespace SteamVRControllerKeyNames

#if UVER >= 418
namespace SteamVRControllerKeys {
const FKey Touch0(SteamVRControllerKeyNames::Touch0);
const FKey Touch1(SteamVRControllerKeyNames::Touch1);
const FKey GenericGrip(SteamVRControllerKeyNames::GenericGrip);
const FKey GenericTrigger(SteamVRControllerKeyNames::GenericTrigger);
const FKey GenericTouchpad(SteamVRControllerKeyNames::GenericTouchpad);
const FKey GenericMenu(SteamVRControllerKeyNames::GenericMenu);
const FKey GenericSystem(SteamVRControllerKeyNames::GenericSystem);

const FKey SteamVR_Knuckles_Left_HandGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Left_HandGrip);
const FKey SteamVR_Knuckles_Left_IndexGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Left_IndexGrip);
const FKey SteamVR_Knuckles_Left_MiddleGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Left_MiddleGrip);
const FKey SteamVR_Knuckles_Left_RingGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Left_RingGrip);
const FKey SteamVR_Knuckles_Left_PinkyGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Left_PinkyGrip);

const FKey SteamVR_Knuckles_Right_HandGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Right_HandGrip);
const FKey SteamVR_Knuckles_Right_IndexGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Right_IndexGrip);
const FKey SteamVR_Knuckles_Right_MiddleGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Right_MiddleGrip);
const FKey SteamVR_Knuckles_Right_RingGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Right_RingGrip);
const FKey SteamVR_Knuckles_Right_PinkyGrip(SteamVRControllerKeyNames::SteamVR_Knuckles_Right_PinkyGrip);
} // namespace SteamVRControllerKeys

namespace KeyMappings {
FGamepadKeyNames::Type Left_TouchPadUp;
FGamepadKeyNames::Type Left_TouchPadDown;
FGamepadKeyNames::Type Left_TouchPadLeft;
FGamepadKeyNames::Type Left_TouchPadRight;

FGamepadKeyNames::Type Right_TouchPadUp;
FGamepadKeyNames::Type Right_TouchPadDown;
FGamepadKeyNames::Type Right_TouchPadLeft;
FGamepadKeyNames::Type Right_TouchPadRight;

FGamepadKeyNames::Type Left_Thumbstick_Up;
FGamepadKeyNames::Type Left_Thumbstick_Down;
FGamepadKeyNames::Type Left_Thumbstick_Left;
FGamepadKeyNames::Type Left_Thumbstick_Right;

FGamepadKeyNames::Type Right_Thumbstick_Up;
FGamepadKeyNames::Type Right_Thumbstick_Down;
FGamepadKeyNames::Type Right_Thumbstick_Left;
FGamepadKeyNames::Type Right_Thumbstick_Right;

FGamepadKeyNames::Type Left_AppMenu;
FGamepadKeyNames::Type Left_TouchPadPress;
FGamepadKeyNames::Type Left_Trigger;
FGamepadKeyNames::Type Left_Grip;

FGamepadKeyNames::Type Right_AppMenu;
FGamepadKeyNames::Type Right_TouchPadPress;
FGamepadKeyNames::Type Right_Trigger;
FGamepadKeyNames::Type Right_Grip;

FGamepadKeyNames::Type Left_Thumbstick_X;
FGamepadKeyNames::Type Left_Thumbstick_Y;
FGamepadKeyNames::Type Left_TriggerAxis;

FGamepadKeyNames::Type Right_Thumbstick_X;
FGamepadKeyNames::Type Right_Thumbstick_Y;
FGamepadKeyNames::Type Right_TriggerAxis;

FGamepadKeyNames::Type Left_System;
FGamepadKeyNames::Type Right_System;
} // namespace KeyMappings

static ESteamVRTouchDPadMapping DefaultDPadMapping2 = ESteamVRTouchDPadMapping::FaceButtons;
#endif

#define MAX_STEAMVR_CONTROLLER_PAIRS 8

class FSteamVRController : public IInputDevice,
#if UVER < 419
                           public IMotionController,
#else
                           public FXRMotionControllerBase,
#endif
                           public IHapticDevice {
#if 1 //STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    FVRgHMD* GetVrgHmd() const {
#if UVER < 418
        if (GEngine->HMDDevice.IsValid() && (GEngine->HMDDevice->GetHMDDeviceType() == EHMDDeviceType::DT_ES2GenericStereoMesh)) {
            return (FVRgHMD*)(GEngine->HMDDevice.Get());
        }
#else
        static FName SystemName(TEXT("VRgHMD"));
        if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName)) {
            return static_cast<FVRgHMD*>(GEngine->XRSystem.Get());
        }
#endif

        return nullptr;
    }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

public:
    /** The maximum number of Unreal controllers.  Each Unreal controller represents a pair of motion controller devices */
    static const int32 MaxUnrealControllers = MAX_STEAMVR_CONTROLLER_PAIRS;

    /** Total number of motion controllers we'll support */
    static const int32 MaxControllers = MaxUnrealControllers * 2;

    /**
	 * Buttons on the SteamVR controller
	 */
    struct ESteamVRControllerButton {
        enum Type {
            System = 0,
            ApplicationMenu,
            TouchPadPress,
            TouchPadTouch,
            TriggerPress,
            Grip,
            TouchPadUp,
            TouchPadDown,
            TouchPadLeft,
            TouchPadRight,

            /** Max number of controller buttons.  Must be < 256 */
            TotalButtonCount
        };
    };

    void InitMappings() {
#if UVER >= 424
        KeyMappings::Left_TouchPadUp = EKeys::Vive_Left_Trackpad_Up.GetFName();
        KeyMappings::Left_TouchPadDown = EKeys::Vive_Left_Trackpad_Down.GetFName();
        KeyMappings::Left_TouchPadLeft = EKeys::Vive_Left_Trackpad_Left.GetFName();
        KeyMappings::Left_TouchPadRight = EKeys::Vive_Left_Trackpad_Right.GetFName();

        KeyMappings::Right_TouchPadUp = EKeys::Vive_Right_Trackpad_Up.GetFName();
        KeyMappings::Right_TouchPadDown = EKeys::Vive_Right_Trackpad_Down.GetFName();
        KeyMappings::Right_TouchPadLeft = EKeys::Vive_Right_Trackpad_Left.GetFName();
        KeyMappings::Right_TouchPadRight = EKeys::Vive_Right_Trackpad_Right.GetFName();

        KeyMappings::Left_Thumbstick_Up = KeyMappings::Left_TouchPadUp;
        KeyMappings::Left_Thumbstick_Down = KeyMappings::Left_TouchPadDown;
        KeyMappings::Left_Thumbstick_Left = KeyMappings::Left_TouchPadLeft;
        KeyMappings::Left_Thumbstick_Right = KeyMappings::Left_TouchPadRight;

        KeyMappings::Right_Thumbstick_Up = KeyMappings::Right_TouchPadUp;
        KeyMappings::Right_Thumbstick_Down = KeyMappings::Right_TouchPadDown;
        KeyMappings::Right_Thumbstick_Left = KeyMappings::Right_TouchPadLeft;
        KeyMappings::Right_Thumbstick_Right = KeyMappings::Right_TouchPadRight;

        KeyMappings::Left_AppMenu = EKeys::Vive_Left_Menu_Click.GetFName();
        KeyMappings::Left_TouchPadPress = EKeys::Vive_Left_Trackpad_Click.GetFName();
        KeyMappings::Left_Trigger = EKeys::Vive_Left_Trigger_Click.GetFName();
        KeyMappings::Left_Grip = EKeys::Vive_Left_Grip_Click.GetFName();

        KeyMappings::Right_AppMenu = EKeys::Vive_Right_Menu_Click.GetFName();
        KeyMappings::Right_TouchPadPress = EKeys::Vive_Right_Trackpad_Click.GetFName();
        KeyMappings::Right_Trigger = EKeys::Vive_Right_Trigger_Click.GetFName();
        KeyMappings::Right_Grip = EKeys::Vive_Right_Grip_Click.GetFName();

        KeyMappings::Left_Thumbstick_X = EKeys::Vive_Left_Trackpad_X.GetFName();
        KeyMappings::Left_Thumbstick_Y = EKeys::Vive_Left_Trackpad_Y.GetFName();
        KeyMappings::Left_TriggerAxis = EKeys::Vive_Left_Trigger_Axis.GetFName();

        KeyMappings::Right_Thumbstick_X = EKeys::Vive_Right_Trackpad_X.GetFName();
        KeyMappings::Right_Thumbstick_Y = EKeys::Vive_Right_Trackpad_Y.GetFName();
        KeyMappings::Right_TriggerAxis = EKeys::Vive_Right_Trigger_Axis.GetFName();

        KeyMappings::Left_System = EKeys::Vive_Left_System_Click.GetFName();
        KeyMappings::Right_System = EKeys::Vive_Right_System_Click.GetFName();

#else // UVER >= 424

        KeyMappings::Left_TouchPadUp = FGamepadKeyNames::MotionController_Left_FaceButton1;
        KeyMappings::Left_TouchPadRight = FGamepadKeyNames::MotionController_Left_FaceButton2;
        KeyMappings::Left_TouchPadDown = FGamepadKeyNames::MotionController_Left_FaceButton3;
        KeyMappings::Left_TouchPadLeft = FGamepadKeyNames::MotionController_Left_FaceButton4;

        KeyMappings::Right_TouchPadUp = FGamepadKeyNames::MotionController_Right_FaceButton1;
        KeyMappings::Right_TouchPadRight = FGamepadKeyNames::MotionController_Right_FaceButton2;
        KeyMappings::Right_TouchPadDown = FGamepadKeyNames::MotionController_Right_FaceButton3;
        KeyMappings::Right_TouchPadLeft = FGamepadKeyNames::MotionController_Right_FaceButton4;

        KeyMappings::Left_Thumbstick_Up = FGamepadKeyNames::MotionController_Left_Thumbstick_Up;
        KeyMappings::Left_Thumbstick_Down = FGamepadKeyNames::MotionController_Left_Thumbstick_Down;
        KeyMappings::Left_Thumbstick_Left = FGamepadKeyNames::MotionController_Left_Thumbstick_Left;
        KeyMappings::Left_Thumbstick_Right = FGamepadKeyNames::MotionController_Left_Thumbstick_Right;

        KeyMappings::Right_Thumbstick_Up = FGamepadKeyNames::MotionController_Right_Thumbstick_Up;
        KeyMappings::Right_Thumbstick_Down = FGamepadKeyNames::MotionController_Right_Thumbstick_Down;
        KeyMappings::Right_Thumbstick_Left = FGamepadKeyNames::MotionController_Right_Thumbstick_Left;
        KeyMappings::Right_Thumbstick_Right = FGamepadKeyNames::MotionController_Right_Thumbstick_Right;

        KeyMappings::Left_AppMenu = FGamepadKeyNames::MotionController_Left_Shoulder;
        KeyMappings::Left_TouchPadPress = FGamepadKeyNames::MotionController_Left_Thumbstick;
        KeyMappings::Left_Trigger = FGamepadKeyNames::MotionController_Left_Trigger;
        KeyMappings::Left_Grip = FGamepadKeyNames::MotionController_Left_Grip1;

        KeyMappings::Right_AppMenu = FGamepadKeyNames::MotionController_Right_Shoulder;
        KeyMappings::Right_TouchPadPress = FGamepadKeyNames::MotionController_Right_Thumbstick;
        KeyMappings::Right_Trigger = FGamepadKeyNames::MotionController_Right_Trigger;
        KeyMappings::Right_Grip = FGamepadKeyNames::MotionController_Right_Grip1;

        KeyMappings::Left_Thumbstick_X = FGamepadKeyNames::MotionController_Left_Thumbstick_X;
        KeyMappings::Left_Thumbstick_Y = FGamepadKeyNames::MotionController_Left_Thumbstick_Y;
        KeyMappings::Left_TriggerAxis = FGamepadKeyNames::MotionController_Left_TriggerAxis;

        KeyMappings::Right_Thumbstick_X = FGamepadKeyNames::MotionController_Right_Thumbstick_X;
        KeyMappings::Right_Thumbstick_Y = FGamepadKeyNames::MotionController_Right_Thumbstick_Y;
        KeyMappings::Right_TriggerAxis = FGamepadKeyNames::MotionController_Right_TriggerAxis;

        KeyMappings::Left_System = FGamepadKeyNames::SpecialLeft;
        KeyMappings::Right_System = FGamepadKeyNames::SpecialRight;

#endif // UVER >= 424
    }

    void SetTouchDPadMapping(ESteamVRTouchDPadMapping NewMapping) {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        DefaultDPadMapping2 = NewMapping;

        switch (NewMapping) {
        case ESteamVRTouchDPadMapping::FaceButtons:
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Left_TouchPadUp;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Left_TouchPadDown;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Left_TouchPadLeft;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Left_TouchPadRight;

            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Right_TouchPadUp;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Right_TouchPadDown;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Right_TouchPadLeft;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Right_TouchPadRight;
            break;

        case ESteamVRTouchDPadMapping::ThumbstickDirections:
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Left_Thumbstick_Up;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Left_Thumbstick_Down;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Left_Thumbstick_Left;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Left_Thumbstick_Right;

            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Right_Thumbstick_Up;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Right_Thumbstick_Down;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Right_Thumbstick_Left;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Right_Thumbstick_Right;
            break;

        default:
            UE_LOG(LogSteamVRController, Warning, TEXT("Unsupported d-pad mapping (%d). Defaulting to disabled."), (int32)NewMapping);
        case ESteamVRTouchDPadMapping::Disabled:
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;

            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
            Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;
            break;
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    void SafeAddKey(const FKeyDetails& key) {
        if (EKeys::GetKeyDetails(key.GetKey()).Get() == nullptr) {
            EKeys::AddKey(key);
        }
    }

    void AddSteamControllerKeys() {
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::Touch0, LOCTEXT("Steam_Touch_0", "SteamVR Touch 0"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::Touch1, LOCTEXT("Steam_Touch_1", "SteamVR Touch 1"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::GenericGrip, LOCTEXT("Steam_Generic_Grip", "SteamVR Generic Grip"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::GenericTrigger, LOCTEXT("Steam_Generic_Trigger", "SteamVR Generic Trigger"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::GenericTouchpad, LOCTEXT("Steam_Generic_Touchpad", "SteamVR Generic Touchpad"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::GenericMenu, LOCTEXT("Steam_Generic_Menu", "SteamVR Generic Menu"), FKeyDetails::GamepadKey));
        SafeAddKey(FKeyDetails(SteamVRControllerKeys::GenericSystem, LOCTEXT("Steam_Generic_System", "SteamVR Generic System"), FKeyDetails::GamepadKey));

        FKeyDetails::EKeyFlags FloatAxis;
#if UVER >= 426
        FloatAxis = FKeyDetails::Axis1D;
#else
        FloatAxis = FKeyDetails::FloatAxis;
#endif

        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Left_HandGrip,
            LOCTEXT("SteamVR_Knuckles_Left_HandGrip", "SteamVR Knuckles (L) Hand Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Left_IndexGrip,
            LOCTEXT("SteamVR_Knuckles_Left_IndexGrip", "SteamVR Knuckles (L) Index Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Left_MiddleGrip,
            LOCTEXT("SteamVR_Knuckles_Left_MiddleGrip", "SteamVR Knuckles (L) Middle Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Left_RingGrip,
            LOCTEXT("SteamVR_Knuckles_Left_RingGrip", "SteamVR Knuckles (L) Ring Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Left_PinkyGrip,
            LOCTEXT("SteamVR_Knuckles_Left_PinkyGrip", "SteamVR Knuckles (L) Pinky Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));

        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Right_HandGrip,
            LOCTEXT("SteamVR_Knuckles_Right_HandGrip", "SteamVR Knuckles (R) Hand Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Right_IndexGrip,
            LOCTEXT("SteamVR_Knuckles_Right_IndexGrip", "SteamVR Knuckles (R) Index Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Right_MiddleGrip,
            LOCTEXT("SteamVR_Knuckles_Right_MiddleGrip", "SteamVR Knuckles (R) Middle Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Right_RingGrip,
            LOCTEXT("SteamVR_Knuckles_Right_RingGrip", "SteamVR Knuckles (R) Ring Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
        EKeys::AddKey(FKeyDetails(SteamVRControllerKeys::SteamVR_Knuckles_Right_PinkyGrip,
            LOCTEXT("SteamVR_Knuckles_Right_PinkyGrip", "SteamVR Knuckles (R) Pinky Grip CapSense"), FKeyDetails::GamepadKey | FloatAxis));
    }

    FSteamVRController(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
        : MessageHandler(InMessageHandler) {

        InitMappings();

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        FMemory::Memzero(ControllerStates, sizeof(ControllerStates));

        for (int32 i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) {
            DeviceToControllerMap[i] = INDEX_NONE;
        }

        for (int32 UnrealControllerIndex = 0; UnrealControllerIndex < MaxUnrealControllers; ++UnrealControllerIndex) {
            for (int32 HandIndex = 0; HandIndex < CONTROLLERS_PER_PLAYER; ++HandIndex) {
                UnrealControllerIdAndHandToDeviceIdMap[UnrealControllerIndex][HandIndex] = INDEX_NONE;
            }
        }

        for (int32& Count : UnrealControllerHandUsageCount) {
            Count = 0;
        }

#if UVER < 418
        for (int i = 0; i < CONTROLLERS_PER_PLAYER; ++i) {
            for (int j = 0; j < ESteamVRControllerButton::TotalButtonCount; ++j) {
                Buttons[i][j] = FGamepadKeyNames::Invalid;
            }
        }
#else
        for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) {
            for (int j = 0; j < ESteamVRControllerButton::TotalButtonCount; ++j) {
                Buttons[i][j] = FGamepadKeyNames::Invalid;
            }
        }
#endif

        NumControllersMapped = 0;
        InitialButtonRepeatDelay = 0.2f;
        ButtonRepeatDelay = 0.1f;

        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::System] = KeyMappings::Left_System;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::ApplicationMenu] = KeyMappings::Left_AppMenu;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadPress] = KeyMappings::Left_TouchPadPress;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadTouch] = SteamVRControllerKeyNames::Touch0;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TriggerPress] = KeyMappings::Left_Trigger;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::Grip] = KeyMappings::Left_Grip;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Left_TouchPadUp;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Left_TouchPadDown;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Left_TouchPadLeft;
        Buttons[(int32)EControllerHand::Left][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Left_TouchPadRight;

        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::System] = KeyMappings::Right_System;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::ApplicationMenu] = KeyMappings::Right_AppMenu;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadPress] = KeyMappings::Right_TouchPadPress;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadTouch] = SteamVRControllerKeyNames::Touch1;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TriggerPress] = KeyMappings::Right_Trigger;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::Grip] = KeyMappings::Right_Grip;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadUp] = KeyMappings::Right_TouchPadUp;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadDown] = KeyMappings::Right_TouchPadDown;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadLeft] = KeyMappings::Right_TouchPadLeft;
        Buttons[(int32)EControllerHand::Right][ESteamVRControllerButton::TouchPadRight] = KeyMappings::Right_TouchPadRight;

        // Init Left & Right, TouchPadUp/Down/Left/Right button mappings
        SetTouchDPadMapping(DefaultDPadMapping2);

        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::System] = SteamVRControllerKeyNames::GenericSystem;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::ApplicationMenu] = SteamVRControllerKeyNames::GenericMenu;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadPress] = SteamVRControllerKeyNames::GenericTouchpad;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadTouch] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TriggerPress] = SteamVRControllerKeyNames::GenericTrigger;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::Grip] = SteamVRControllerKeyNames::GenericGrip;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Pad][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;

        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::System] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::ApplicationMenu] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadPress] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadTouch] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TriggerPress] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::Grip] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::ExternalCamera][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;

        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::System] = SteamVRControllerKeyNames::GenericSystem;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::ApplicationMenu] = SteamVRControllerKeyNames::GenericMenu;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadPress] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadTouch] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TriggerPress] = SteamVRControllerKeyNames::GenericTrigger;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::Grip] = SteamVRControllerKeyNames::GenericGrip;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
        Buttons[(int32)EControllerHand::Gun][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;

        for (int32 SpecialIndex = (int32)EControllerHand::Special_1; SpecialIndex <= (int32)EControllerHand::Special_9; ++SpecialIndex) {
            Buttons[SpecialIndex][ESteamVRControllerButton::System] = SteamVRControllerKeyNames::GenericSystem;
            Buttons[SpecialIndex][ESteamVRControllerButton::ApplicationMenu] = SteamVRControllerKeyNames::GenericMenu;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadPress] = SteamVRControllerKeyNames::GenericTouchpad;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadTouch] = FGamepadKeyNames::Invalid;
            Buttons[SpecialIndex][ESteamVRControllerButton::TriggerPress] = SteamVRControllerKeyNames::GenericTrigger;
            Buttons[SpecialIndex][ESteamVRControllerButton::Grip] = SteamVRControllerKeyNames::GenericGrip;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadUp] = FGamepadKeyNames::Invalid;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadDown] = FGamepadKeyNames::Invalid;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadLeft] = FGamepadKeyNames::Invalid;
            Buttons[SpecialIndex][ESteamVRControllerButton::TouchPadRight] = FGamepadKeyNames::Invalid;
        }

        AddSteamControllerKeys();

        IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

        // Read tracker names.
        if (TrackerNames.Num() == 0) {
            auto VrgHmd = GetVrgHmd();
            if (VrgHmd) {
                auto VrgSdk = VrgHmd->GetSdk();
                if (VrgSdk) {

                    uint32_t count{};
                    VrgSdk->InternalApi->GetTrackerNames(&count, nullptr);

                    if (count > 0) {
                        std::vector<const char*> names(count);
                        VrgSdk->InternalApi->GetTrackerNames(&count, names.data());

                        for (const auto name : names) {
                            TrackerNames.Add(FName(name));
                        }
                    }
                }
            }
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    virtual ~FSteamVRController() {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    virtual void Tick(float DeltaTime) override {}

    virtual void SendControllerEvents() override {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        auto VrgHmd = GetVrgHmd();
        if (!VrgHmd) {
            return;
        }

        auto VrgSdk = VrgHmd->GetSdk();
        if (!VrgSdk) {
            return;
        }

        if (!VrgSdk->m_lighthouseInited) {
            return;
        }

        VrgSdk->InternalApi->UpdateTrackerData();

        const double CurrentTime = FPlatformTime::Seconds();

        uint32_t ControllerIds[MaxControllers];
        auto ControllerCount = VrgSdk->GetControllerIds(ControllerIds, MaxControllers);

        VrgControllerData ControllersData[MaxControllers];
        for (uint32_t CtrlIndex = 0; CtrlIndex < ControllerCount; ++CtrlIndex) {
            if (VrgSdk->vrgGetControllerData(ControllerIds[CtrlIndex], &ControllersData[CtrlIndex]) != VRG_ERROR_OK) {
                ControllersData[CtrlIndex].valid = 0;
                ControllersData[CtrlIndex].connected = 0;
            }
        }

        for (uint32_t CtrlIndex = 0; CtrlIndex < ControllerCount; ++CtrlIndex) {
            auto DeviceIndex = ControllerIds[CtrlIndex];
            auto& ControllerData = ControllersData[CtrlIndex];

            // update the mappings if this is a new device

            if (DeviceToControllerMap[DeviceIndex] == INDEX_NONE) {
                // skip unregistered devices that are not connected.
                // This lets connected devices claim their preferred hands.
                if (!ControllerData.connected) {
                    continue;
                }

                // don't map too many controllers
                if (NumControllersMapped >= MaxControllers) {
                    UE_LOG(LogSteamVRController, Warning, TEXT("Found more controllers than we support (%i vs %i)!  Probably need to fix this."),
                        NumControllersMapped + 1, MaxControllers);
                    continue;
                }

                // Decide which hand to associate this controller with
                EControllerHand ChosenHand = EControllerHand::Special_9;
                {
                    UE_LOG(LogSteamVRController, Log, TEXT("Controller role for device %i is %i (invalid=0, left=1, right=2)."), DeviceIndex,
                        (int32)ControllerData.role);

                    // if we already have one hand we have to put the controller on the other hand
                    if (UnrealControllerHandUsageCount[(int32)EControllerHand::Right] != 0
                        && UnrealControllerHandUsageCount[(int32)EControllerHand::Left] == 0) {
                        UE_LOG(LogSteamVRController, Log, TEXT("Putting controller %i on the left hand because there is already a controller for the right."),
                            DeviceIndex);
                        if (ControllerData.role == VrgControllerData::VRG_CONTROLLER_ROLE_RIGHT_HAND) {
                            UE_LOG(LogSteamVRController, Warning,
                                TEXT("We are ignoring the steam api controller role for device %i, because we have already used that spot."), DeviceIndex);
                        }
                        ChosenHand = EControllerHand::Left;
                    } else if (UnrealControllerHandUsageCount[(int32)EControllerHand::Left] != 0
                        && UnrealControllerHandUsageCount[(int32)EControllerHand::Right] == 0) {
                        UE_LOG(LogSteamVRController, Log, TEXT("Putting controller %i on the right hand because there is already a controller for the left."),
                            DeviceIndex);
                        if (ControllerData.role == VrgControllerData::VRG_CONTROLLER_ROLE_LEFT_HAND) {
                            UE_LOG(LogSteamVRController, Warning,
                                TEXT("We are ignoring the steam api controller role for device %i, because we have already used that spot."), DeviceIndex);
                        }
                        ChosenHand = EControllerHand::Right;
                    } else {
                        // Either both controller hands are unused or both are used.

                        // Try to give the controller to the role it prefers.
                        switch (ControllerData.role) {
                        case VrgControllerData::VRG_CONTROLLER_ROLE_LEFT_HAND:
                            UE_LOG(LogSteamVRController, Warning, TEXT("Controller device %i is being assigned to its prefered role 'left'."), DeviceIndex);
                            ChosenHand = EControllerHand::Left;
                            break;
                        case VrgControllerData::VRG_CONTROLLER_ROLE_RIGHT_HAND:
                            UE_LOG(LogSteamVRController, Warning, TEXT("Controller device %i is being assigned to its prefered role 'right'."), DeviceIndex);
                            ChosenHand = EControllerHand::Right;
                            break;
                        case VrgControllerData::VRG_CONTROLLER_ROLE_INVALID: {
                            bool hasLeft = false;
                            bool hasRight = false;

                            for (uint32_t Ctrl = 0; Ctrl < ControllerCount; ++Ctrl) {
                                if (Ctrl == CtrlIndex) {
                                    continue;
                                }
                                if (ControllersData[Ctrl].role == VrgControllerData::VRG_CONTROLLER_ROLE_LEFT_HAND) {
                                    hasLeft = true;
                                } else if (ControllersData[Ctrl].role == VrgControllerData::VRG_CONTROLLER_ROLE_RIGHT_HAND) {
                                    hasRight = true;
                                }

                                if (hasLeft && hasRight) {
                                    break;
                                }
                            }

                            if (!hasLeft) {
                                UE_LOG(LogSteamVRController, Warning,
                                    TEXT("Controller device %i has no role set.  There is no 'left' controller according to steamvr, so we are pickign "
                                         "that."),
                                    DeviceIndex);
                                ChosenHand = EControllerHand::Left;
                            } else if (!hasRight) {
                                UE_LOG(LogSteamVRController, Warning,
                                    TEXT("Controller device %i has no role set.  There is no 'right' controller according to steamvr, so we are pickign "
                                         "that."),
                                    DeviceIndex);
                                ChosenHand = EControllerHand::Right;
                            } else {
                                UE_LOG(LogSteamVRController, Warning,
                                    TEXT("Controller device %i has no role set.  We could not find an unused role, so ignoring controller."), DeviceIndex);
                            }
                        } break;
                        default:
                            UE_LOG(LogSteamVRController, Error, TEXT("Controller device with unknown role %i encountered.  Ignoring it."),
                                (int32)ControllerData.role);
                            continue;
                        }
                    }
                }

                UE_LOG(LogSteamVRController, Log, TEXT("Controller device %i is being assigned unreal hand %i (left=0, right=1)."), DeviceIndex,
                    (int32)ChosenHand);
                ControllerStates[DeviceIndex].Hand = ChosenHand;
                UnrealControllerHandUsageCount[(int32)ChosenHand] += 1;

                DeviceToControllerMap[DeviceIndex] = FMath::FloorToInt(NumControllersMapped / CONTROLLERS_PER_PLAYER);
                ++NumControllersMapped;

                // update the SteamVR plugin with the new mapping
                UnrealControllerIdAndHandToDeviceIdMap[DeviceToControllerMap[DeviceIndex]][(int32)ControllerStates[DeviceIndex].Hand] = DeviceIndex;
                VrgSdk->SetControllerMap(UnrealControllerIdAndHandToDeviceIdMap);
            }

            // get the controller index for this device
            int32 ControllerIndex = DeviceToControllerMap[DeviceIndex];
            FControllerState& ControllerState = ControllerStates[DeviceIndex];
            EControllerHand HandToUse = ControllerState.Hand;

            // check to see if we need to swap input hands for debugging
            static const auto CVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.SwapMotionControllerInput"));
            bool bSwapHandInput = (CVar->GetValueOnGameThread() != 0) ? true : false;
            if (bSwapHandInput) {
                HandToUse = (HandToUse == EControllerHand::Left) ? EControllerHand::Right : EControllerHand::Left;
            }

            /*add 1*/
            if (ControllerData.packetNumber != ControllerState.PacketNum) {
                bool CurrentStates[ESteamVRControllerButton::TotalButtonCount] = { 0 };

                // Get the current state of all buttons
                CurrentStates[ESteamVRControllerButton::System] = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_System));
                CurrentStates[ESteamVRControllerButton::ApplicationMenu]
                    = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu));
                CurrentStates[ESteamVRControllerButton::TouchPadPress]
                    = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad));
                CurrentStates[ESteamVRControllerButton::TouchPadTouch]
                    = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad));
                CurrentStates[ESteamVRControllerButton::TriggerPress] = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger));
                CurrentStates[ESteamVRControllerButton::Grip] = !!(ControllerData.buttonsPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip));

                // If the touchpad isn't currently pressed or touched, zero put both of the axes
                if (!CurrentStates[ESteamVRControllerButton::TouchPadTouch]) {
                    ControllerData.axis[TOUCHPAD_AXIS].y = 0.0f;
                    ControllerData.axis[TOUCHPAD_AXIS].x = 0.0f;
                }

                // D-pad emulation
                const FVector2D TouchDir = FVector2D(ControllerData.axis[TOUCHPAD_AXIS].x, ControllerData.axis[TOUCHPAD_AXIS].y).GetSafeNormal();
                const FVector2D UpDir(0.f, 1.f);
                const FVector2D RightDir(1.f, 0.f);

                const float VerticalDot = TouchDir | UpDir;
                const float RightDot = TouchDir | RightDir;

                const bool bPressed = !TouchDir.IsNearlyZero() && CurrentStates[ESteamVRControllerButton::TouchPadPress];

                CurrentStates[ESteamVRControllerButton::TouchPadUp] = bPressed && (VerticalDot >= DOT_45DEG);
                CurrentStates[ESteamVRControllerButton::TouchPadDown] = bPressed && (VerticalDot <= -DOT_45DEG);
                CurrentStates[ESteamVRControllerButton::TouchPadLeft] = bPressed && (RightDot <= -DOT_45DEG);
                CurrentStates[ESteamVRControllerButton::TouchPadRight] = bPressed && (RightDot >= DOT_45DEG);

                if (ControllerState.TouchPadXAnalog != ControllerData.axis[TOUCHPAD_AXIS].x) {
                    const FGamepadKeyNames::Type AxisButton
                        = (HandToUse == EControllerHand::Left) ? KeyMappings::Left_Thumbstick_X : KeyMappings::Right_Thumbstick_X;
                    SendAnalogMessage(AxisButton, ControllerIndex, ControllerData.axis[TOUCHPAD_AXIS].x);
                    ControllerState.TouchPadXAnalog = ControllerData.axis[TOUCHPAD_AXIS].x;
                }

                if (ControllerState.TouchPadYAnalog != ControllerData.axis[TOUCHPAD_AXIS].y) {
                    const FGamepadKeyNames::Type AxisButton
                        = (HandToUse == EControllerHand::Left) ? KeyMappings::Left_Thumbstick_Y : KeyMappings::Right_Thumbstick_Y;
                    // Invert the y to match UE4 convention
                    const float Value = -ControllerData.axis[TOUCHPAD_AXIS].y;
                    SendAnalogMessage(AxisButton, ControllerIndex, Value);
                    ControllerState.TouchPadYAnalog = Value;
                }

                if (ControllerState.TriggerAnalog != ControllerData.axis[TRIGGER_AXIS].x) {
                    const FGamepadKeyNames::Type AxisButton
                        = (HandToUse == EControllerHand::Left) ? KeyMappings::Left_TriggerAxis : KeyMappings::Right_TriggerAxis;
                    SendAnalogMessage(AxisButton, ControllerIndex, ControllerData.axis[TRIGGER_AXIS].x);
                    ControllerState.TriggerAnalog = ControllerData.axis[TRIGGER_AXIS].x;
                }

                // For each button check against the previous state and send the correct message if any
                for (int32 ButtonIndex = 0; ButtonIndex < ESteamVRControllerButton::TotalButtonCount; ++ButtonIndex) {
                    if (CurrentStates[ButtonIndex] != ControllerState.ButtonStates[ButtonIndex]) {
                        if (CurrentStates[ButtonIndex]) {
                            SendButtonMessage(true, HandToUse, ButtonIndex, ControllerIndex, false);
                        } else {
                            SendButtonMessage(false, HandToUse, ButtonIndex, ControllerIndex, false);
                        }

                        if (CurrentStates[ButtonIndex] != 0) {
                            // this button was pressed - set the button's NextRepeatTime to the InitialButtonRepeatDelay
                            ControllerState.NextRepeatTime[ButtonIndex] = CurrentTime + InitialButtonRepeatDelay;
                        }
                    }

                    // Update the state for next time
                    ControllerState.ButtonStates[ButtonIndex] = CurrentStates[ButtonIndex];
                }

                ControllerState.PacketNum = ControllerData.packetNumber;
            }

            for (int32 ButtonIndex = 0; ButtonIndex < ESteamVRControllerButton::TotalButtonCount; ++ButtonIndex) {
                if (ControllerState.ButtonStates[ButtonIndex] != 0 && ControllerState.NextRepeatTime[ButtonIndex] <= CurrentTime) {
                    SendButtonMessage(true, HandToUse, ButtonIndex, ControllerIndex, true);

                    // set the button's NextRepeatTime to the ButtonRepeatDelay
                    ControllerState.NextRepeatTime[ButtonIndex] = CurrentTime + ButtonRepeatDelay;
                }
            }
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    void SendAnalogMessage(FGamepadKeyNames::Type KeyName, int32 ControllerId, float AnalogValue) {
        FKey Key(KeyName);
        if (Key.IsValid()) {
            MessageHandler->OnControllerAnalog(KeyName, ControllerId, AnalogValue);
        } else {
            UE_LOG(LogSteamVRController, Log, TEXT("Invalid analog key detected: name=%s, controllerId=%i."), *KeyName.ToString(), ControllerId);
        }
    }

    void SendButtonMessage(bool pressed, EControllerHand hand, int32 buttonIndex, int32 controllerIndex, bool isRepeat) {
        auto button = Buttons[(int32)hand][buttonIndex];

        FKey Key(button);
        if (Key.IsValid()) {
            //UE_LOG(LogSteamVRController, Log, TEXT("Sending button hand=%i, buttonIndex=%i, controllerIndex=%i, name=%s."), (int32)hand, buttonIndex,
            //    controllerIndex, *button.ToString());
            if (pressed) {
                MessageHandler->OnControllerButtonPressed(button, controllerIndex, isRepeat);
            } else {
                MessageHandler->OnControllerButtonReleased(button, controllerIndex, isRepeat);
            }
        } else {
            UE_LOG(LogSteamVRController, Log, TEXT("Invalid key detected: hand=%i, buttonIndex=%i, controllerIndex=%i, name=%s."), (int32)hand, buttonIndex,
                controllerIndex, *button.ToString());
        }
    }

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    int32 UnrealControllerIdToControllerIndex(const int32 UnrealControllerId, const EControllerHand Hand) const {
        return UnrealControllerIdAndHandToDeviceIdMap[UnrealControllerId][(int32)Hand];
    }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

    void SetChannelValue(int32 UnrealControllerId, FForceFeedbackChannelType ChannelType, float Value) override {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        // Skip unless this is the left or right large channel, which we consider to be the only SteamVRController feedback channel
        if (ChannelType != FForceFeedbackChannelType::LEFT_LARGE && ChannelType != FForceFeedbackChannelType::RIGHT_LARGE) {
            return;
        }

        const EControllerHand Hand = (ChannelType == FForceFeedbackChannelType::LEFT_LARGE) ? EControllerHand::Left : EControllerHand::Right;
        const int32 ControllerIndex = UnrealControllerIdToControllerIndex(UnrealControllerId, Hand);

        if ((ControllerIndex >= 0) && (ControllerIndex < MaxControllers)) {
            FControllerState& ControllerState = ControllerStates[ControllerIndex];

            ControllerState.ForceFeedbackValue = Value;

            UpdateVibration(ControllerIndex);
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    void SetChannelValues(int32 UnrealControllerId, const FForceFeedbackValues& Values) override {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        const int32 LeftControllerIndex = UnrealControllerIdToControllerIndex(UnrealControllerId, EControllerHand::Left);
        if ((LeftControllerIndex >= 0) && (LeftControllerIndex < MaxControllers)) {
            FControllerState& ControllerState = ControllerStates[LeftControllerIndex];
            ControllerState.ForceFeedbackValue = Values.LeftLarge;

            UpdateVibration(LeftControllerIndex);
        }

        const int32 RightControllerIndex = UnrealControllerIdToControllerIndex(UnrealControllerId, EControllerHand::Right);
        if ((RightControllerIndex >= 0) && (RightControllerIndex < MaxControllers)) {
            FControllerState& ControllerState = ControllerStates[RightControllerIndex];
            ControllerState.ForceFeedbackValue = Values.RightLarge;

            UpdateVibration(RightControllerIndex);
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    }

    virtual IHapticDevice* GetHapticDevice() override {
        return this;
    }

    virtual void SetHapticFeedbackValues(int32 UnrealControllerId, int32 Hand, const FHapticFeedbackValues& Values) override {
#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        if (Hand != (int32)EControllerHand::Left && Hand != (int32)EControllerHand::Right) {
            return;
        }

        const int32 ControllerIndex = UnrealControllerIdToControllerIndex(UnrealControllerId, (EControllerHand)Hand);
        if (ControllerIndex >= 0 && ControllerIndex < MaxControllers) {
            FControllerState& ControllerState = ControllerStates[ControllerIndex];
            ControllerState.ForceFeedbackValue = (Values.Frequency > 0.0f) ? Values.Amplitude : 0.0f;

            UpdateVibration(ControllerIndex);
        }
#endif
    }

    virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override {
        MinFrequency = 0.0f;
        MaxFrequency = 1.0f;
    }

    virtual float GetHapticAmplitudeScale() const override {
        return 1.0f;
    }

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
    void UpdateVibration(const int32 ControllerIndex) {

        const FControllerState& ControllerState = ControllerStates[ControllerIndex];

        auto VrgSdk = GetVrgHmd()->GetSdk();
        if (!VrgSdk) {
            return;
        }

        // Map the float values from [0,1] to be more reasonable values for the SteamController.  The docs say that [100,2000] are reasonable values
        const float LeftIntensity = FMath::Clamp(ControllerState.ForceFeedbackValue * 2000.f, 0.f, 2000.f);
        if (LeftIntensity > 0.f) {
            VrgSdk->vrgControllerTriggerHapticPulse(ControllerIndex, TOUCHPAD_AXIS, LeftIntensity);
        }
    }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

    virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override {
        MessageHandler = InMessageHandler;
    }

    virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override {
        return false;
    }

#if UVER >= 417
    virtual FName GetMotionControllerDeviceTypeName() const override {
        return DeviceTypeName;
    }
    static FName DeviceTypeName;
#endif

    virtual bool GetControllerOrientationAndPosition(
        const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float worldToMeterScale) const {
        bool RetVal = false;

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        FVRgHMD* VrgHmdPlugin = GetVrgHmd();
        if (VrgHmdPlugin && VrgHmdPlugin->GetSdk()) {
            FQuat DeviceOrientation = FQuat::Identity;
            const int32 DeviceId = UnrealControllerIdAndHandToDeviceIdMap[ControllerIndex][(int32)DeviceHand];

            VrgControllerData ctrlData;
            VrgHmdPlugin->GetSdk()->vrgGetControllerData(DeviceId, &ctrlData);

            OutPosition = VrgSdk::ConvertVector(ctrlData.position) * worldToMeterScale;
            DeviceOrientation = VrgSdk::ConvertQuaternion(ctrlData.orientation);

            OutOrientation = DeviceOrientation.Rotator();
            RetVal = true; // mozna sledovat validitu
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

        return RetVal;
    }

    virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const {
        ETrackingStatus TrackingStatus = ETrackingStatus::NotTracked;

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS
        FVRgHMD* VrgHmdPlugin = GetVrgHmd();
        if (VrgHmdPlugin && VrgHmdPlugin->GetSdk()) {
            // TODO HERE: controller status
            const int32 DeviceId = UnrealControllerIdAndHandToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
            VrgControllerData ctrlData;
            auto res = VrgHmdPlugin->GetSdk()->vrgGetControllerData(DeviceId, &ctrlData);

            TrackingStatus = (res == VRG_ERROR_OK && ctrlData.connected) ? ETrackingStatus::Tracked : ETrackingStatus::NotTracked;
        }
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

        return TrackingStatus;
    }

#if STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

    virtual bool IsGamepadAttached() const override {
        FVRgHMD* VrgHmdPlugin = GetVrgHmd();

        if (VrgHmdPlugin) {
            // Check if at least one motion controller is tracked
            // Only need to check for at least one player (player index 0)
            int32 PlayerIndex = 0;
            ETrackingStatus LeftHandTrackingStatus = GetControllerTrackingStatus(PlayerIndex, EControllerHand::Left);
            ETrackingStatus RightHandTrackingStatus = GetControllerTrackingStatus(PlayerIndex, EControllerHand::Right);

            return LeftHandTrackingStatus == ETrackingStatus::Tracked || RightHandTrackingStatus == ETrackingStatus::Tracked;
        }

        return false;
    }

    virtual bool GetControllerOrientationAndPosition(
        const int32 ControllerIndex, const FName MotionSource, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const override {

        if (FXRMotionControllerBase::GetControllerOrientationAndPosition(ControllerIndex, MotionSource, OutOrientation, OutPosition, WorldToMetersScale)) {
            return true;
        }

        int trackerId{};
        for (; trackerId < TrackerNames.Num(); ++trackerId) {
            if (TrackerNames[trackerId] == MotionSource) {
                break;
            }
        }

        if (trackerId == TrackerNames.Num()) {
            return false;
        }

        auto VrgHmd = GetVrgHmd();
        if (!VrgHmd) {
            return false;
        }

        auto VrgSdk = VrgHmd->GetSdk();
        if (!VrgSdk) {
            return false;
        }

        VrgTrackerDataInternal trackerData{};
        VrgSdk->InternalApi->GetTrackerData(trackerId, 0.0f, &trackerData);

        OutPosition = VrgSdk::ConvertVector(trackerData.position) * WorldToMetersScale;
        auto DeviceOrientation = VrgSdk::ConvertQuaternion(trackerData.orientation);

        OutOrientation = DeviceOrientation.Rotator();

        return true;
    }

    virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const FName MotionSource) const override {
        ETrackingStatus TrackingStatus = ETrackingStatus::Tracked;

        // TODO: Read tracker status.

        return TrackingStatus;
    }

    virtual void EnumerateSources(TArray<FMotionControllerSource>& SourcesOut) const override {
        FXRMotionControllerBase::EnumerateSources(SourcesOut);

        for (const auto& name : TrackerNames) {
            SourcesOut.Add(FMotionControllerSource(name));
        }

        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_2")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_3")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_4")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_5")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_6")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_7")));
        //SourcesOut.Add(FMotionControllerSource(TEXT("Vrg_Tracker_8")));
    }

    /* FName GetMotionSourceName(const EControllerHand MotionSource) const {
        switch ((uint32)MotionSource) {
        case 0:
            return FName(TEXT("Vrg_Tracker_1"));
            break;
        case 1:
            return FName(TEXT("Vrg_Tracker_2"));
            break;
        case 2:
            return FName(TEXT("Vrg_Tracker_3"));
            break;
        case 3:
            return FName(TEXT("Vrg_Tracker_4"));
            break;
        case 4:
            return FName(TEXT("Vrg_Tracker_5"));
            break;
        case 5:
            return FName(TEXT("Vrg_Tracker_6"));
            break;
        case 6:
            return FName(TEXT("Vrg_Tracker_7"));
            break;
        case 7:
            return FName(TEXT("Vrg_Tracker_8"));
            break;
        default:
            return NAME_None;
            break;
        }
    }*/

private:
    struct FControllerState {
        /** Which hand this controller is representing */
        EControllerHand Hand;

        /** If packet num matches that on your prior call, then the controller state hasn't been changed since
		  * your last call and there is no need to process it. */
        uint32 PacketNum;

        /** touchpad analog values */
        float TouchPadXAnalog;
        float TouchPadYAnalog;

        /** trigger analog value */
        float TriggerAnalog;

        /** Last frame's button states, so we only send events on edges */
        bool ButtonStates[ESteamVRControllerButton::TotalButtonCount];

        /** Next time a repeat event should be generated for each button */
        double NextRepeatTime[ESteamVRControllerButton::TotalButtonCount];

        /** Value for force feedback on this controller hand */
        float ForceFeedbackValue;
    };

    /** Mappings between tracked devices and 0 indexed controllers */
    int32 NumControllersMapped;
    int32 DeviceToControllerMap[vr::k_unMaxTrackedDeviceCount];
    int32 UnrealControllerIdAndHandToDeviceIdMap[MaxUnrealControllers][CONTROLLERS_PER_PLAYER];
    int32 UnrealControllerHandUsageCount[CONTROLLERS_PER_PLAYER];

    /** Controller states */
    FControllerState ControllerStates[MaxControllers];

    /** Delay before sending a repeat message after a button was first pressed */
    float InitialButtonRepeatDelay;

    /** Delay before sending a repeat message after a button has been pressed for a while */
    float ButtonRepeatDelay;

/** Mapping of controller buttons */
#if UVER < 418
    FGamepadKeyNames::Type Buttons[CONTROLLERS_PER_PLAYER][ESteamVRControllerButton::TotalButtonCount];
#else
    FGamepadKeyNames::Type Buttons[vr::k_unMaxTrackedDeviceCount][ESteamVRControllerButton::TotalButtonCount];
#endif

    /** weak pointer to the IVRSystem owned by the HMD module */
    TWeakPtr<vr::IVRSystem> HMDVRSystem;
#endif // STEAMVRCONTROLLER_SUPPORTED_PLATFORMS

    /** handler to send all messages to */
    TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

    TArray<FName> TrackerNames; // TODO:
};

#if UVER >= 417
FName FSteamVRController::DeviceTypeName(TEXT("SteamVRController"));
#endif

#if UVER >= 418
// defined here in this .cpp file so we have access to FSteamVRController
void USteamVRControllerLibrary::SetTouchDPadMapping(ESteamVRTouchDPadMapping NewMapping) {
#if STEAMVR_SUPPORTED_PLATFORMS
    // modify the default mapping in case we haven't instantiated a FSteamVRController yet
    FSteamVRController::DefaultDPadMapping = NewMapping;
#endif // STEAMVR_SUPPORTED_PLATFORMS

    TArray<IMotionController*> MotionControllers
        = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
    for (IMotionController* MotionController : MotionControllers) {
        if (MotionController != nullptr && MotionController->GetMotionControllerDeviceTypeName() == FSteamVRController::DeviceTypeName) {
            static_cast<FSteamVRController*>(MotionController)->SetTouchDPadMapping(NewMapping);
        }
    }
}

#endif

class FSteamVRControllerPlugin : public ISteamVRControllerPlugin {
    virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override {
        return TSharedPtr<class IInputDevice>(new FSteamVRController(InMessageHandler));
    }
};

IMPLEMENT_MODULE(FSteamVRControllerPlugin, SteamVRController)
