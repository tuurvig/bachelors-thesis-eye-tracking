// Copyright (c) 2021 Vrgineers Inc. All rights reserved.

#pragma once

#include <stdint.h>

#ifndef VRG_API
#define VRG_API(major, minor) extern "C" __declspec(dllimport)


#define VRG_VERSION_MAJOR	4
#define VRG_VERSION_MINOR	4
#endif // VRG_API


//  Event mask, one bit per event (potentially can be extended)
typedef uint64_t EventType;

//  Set n-th bit in the event mask
constexpr EventType MakeEvent(const uint8_t n) {
    return static_cast<EventType>(1) << n;
}

constexpr EventType MakeMask(const uint8_t n) {
    EventType result{ 0 };
    for (uint8_t i{ 0 }; i < n; ++i) {
        result = (result << 1) | 0x1;
    }

    return result;
}

//  Predefined events
//  Image separation (position offset on displays) changed
constexpr EventType VRG_EVENT_RENDER_IMAGE_SEPARATION = MakeEvent(0);

//  Virtual camera offset to nominal eye position in "X" axis (OpenGL coords system)
constexpr EventType VRG_EVENT_RENDER_CAMERA_X_OFFSET = MakeEvent(1);

//  Virtual camera offset to nominal eye position in "Z" axis (OpenGL coords system)
constexpr EventType VRG_EVENT_RENDER_CAMERA_Z_OFFSET = MakeEvent(2);

//  User changed eye relief settings
constexpr EventType VRG_EVENT_WARP_EYE_RELIEF = MakeEvent(3);

//  HMD focus has been updated
constexpr EventType VRG_EVENT_HMD_FOCUS = MakeEvent(4);

//  HMD IPD has been updated
constexpr EventType VRG_EVENT_HMD_IPD = MakeEvent(5);

//  Motion cancellation confit has been updated
constexpr EventType VRG_EVENT_MOCA_UPDATED = MakeEvent(6);

//  HMD color correction has been updated.
constexpr EventType VRG_EVENT_HMD_RENDER_COLOR_CORRECTION = MakeEvent(7);

//  HMD IPD calibration started.
constexpr EventType VRG_EVENT_HMD_IPD_CALIBRATION_STARTED = MakeEvent(8);

//  HMD IPD calibration ended.
constexpr EventType VRG_EVENT_HMD_IPD_CALIBRATION_ENDED = MakeEvent(9);

//  HMD center of projection has been updated.
constexpr EventType VRG_EVENT_HMD_CENTER_OF_PROJECTION = MakeEvent(10);

//  HMD image adjustment (brightness, contrast, gamma).
constexpr EventType VRG_EVENT_HMD_IMAGE_ADJUSTMENT = MakeEvent(11);

//  Virtual camera canted mode changed
constexpr EventType VRG_EVENT_RENDER_CAMERA_CANTED = MakeEvent(12);

//  HMD lens distance has been updated
constexpr EventType VRG_EVENT_HMD_LENS_DIST = MakeEvent(13);

//  Global configuration has changed.
constexpr EventType VRG_EVENT_CONFIG_CHANGED = MakeEvent(14);

//  HMD Gui overlay has changed.
constexpr EventType VRG_EVENT_GUI_OVERLAY_UPDATED = MakeEvent(15);

//  HMD Gui overlay show.
constexpr EventType VRG_EVENT_GUI_OVERLAY_SHOW = MakeEvent(16);

//  HMD Gui overlay hide.
constexpr EventType VRG_EVENT_GUI_OVERLAY_HIDE = MakeEvent(17);

// HMD Gui overlay position reset.
constexpr EventType VRG_EVENT_GUI_OVERLAY_RESET_POSITION = MakeEvent(18);

//  HMD IPD calibration failed.
constexpr EventType VRG_EVENT_HMD_IPD_CALIBRATION_FAILED = MakeEvent(19);

//  HMD debug grab frame.
constexpr EventType VRG_EVENT_HMD_DEBUG_GRAB_FRAME = MakeEvent(20);

//  HMD show hands overlay.
constexpr EventType VRG_EVENT_HMD_DEBUG_SHOW_HANDS = MakeEvent(21);

//  HMD hide hands overlay.
constexpr EventType VRG_EVENT_HMD_DEBUG_HIDE_HANDS = MakeEvent(22);

//  Main FOV has changed.
constexpr EventType VRG_EVENT_RENDER_MAIN_FOV = MakeEvent(23);

//  Recommended rendering resolution changed.
constexpr EventType VRG_EVENT_RENDER_RECOMMENDED_RESOLUTION = MakeEvent(24);

//  Mixed Reality overlay show.
constexpr EventType VRG_EVENT_MR_OVERLAY_SHOW = MakeEvent(25);

//  Mixed Reality overlay hide.
constexpr EventType VRG_EVENT_MR_OVERLAY_HIDE = MakeEvent(26);

//  Mixed Reality settings changed.
constexpr EventType VRG_EVENT_MR_SETTINGS_CHANGED = MakeEvent(27);

//  Mixed Reality overlay capture position.
constexpr EventType VRG_EVENT_MR_OVERLAY_RESET_POSITION = MakeEvent(28);

//  Mixed Reality start.
constexpr EventType VRG_EVENT_MR_CAMERA_START = MakeEvent(29);

//  Mixed Reality stop.
constexpr EventType VRG_EVENT_MR_CAMERA_STOP = MakeEvent(30);

//  Show VR view.
constexpr EventType VRG_EVENT_VR_VIEW_SHOW = MakeEvent(31);

//  Hide VR view.
constexpr EventType VRG_EVENT_VR_VIEW_HIDE = MakeEvent(32);

//  VR view settings.
constexpr EventType VRG_EVENT_VR_VIEW_SETTINGS = MakeEvent(33);

//  VR view screenshot.
constexpr EventType VRG_EVENT_VR_VIEW_SCREENSHOT = MakeEvent(34);

//  VR view video capture.
constexpr EventType VRG_EVENT_VR_VIEW_VIDEO_START = MakeEvent(35);

//  VR view video capture.
constexpr EventType VRG_EVENT_VR_VIEW_VIDEO_STOP = MakeEvent(36);

//  Heatmap overlay show.
constexpr EventType VRG_EVENT_HEATMAP_OVERLAY_SHOW = MakeEvent(37);

//  Heatmap overlay hide.
constexpr EventType VRG_EVENT_HEATMAP_OVERLAY_HIDE = MakeEvent(38);

//  Heatmap activate.
constexpr EventType VRG_EVENT_HEATMAP_ACTIVATE = MakeEvent(39);

//  Heatmap deactivate.
constexpr EventType VRG_EVENT_HEATMAP_DEACTIVATE = MakeEvent(40);

//  Heatmap clear.
constexpr EventType VRG_EVENT_HEATMAP_CLEAR = MakeEvent(41);

//  Heatmap settings.
constexpr EventType VRG_EVENT_HEATMAP_SETTINGS = MakeEvent(42);

constexpr EventType VRG_EVENT_PRIMARY_MASK = MakeMask(42);

//  VR view closed by user.
constexpr EventType VRG_EVENT_VR_VIEW_CLOSED = MakeEvent(43);

//  Left remote MR camera status changed.
constexpr EventType VRG_EVENT_REMOTE_MR_LEFT_STATUS = MakeEvent(44);

//  Right remote MR camera status changed.
constexpr EventType VRG_EVENT_REMOTE_MR_RIGHT_STATUS = MakeEvent(45);

//  Context supplied be the receiver in the registration.
typedef void* EventContext;

//  Event callback supplied by the receiver in the registration
typedef void (*EventCallback)(EventContext, EventType);

//  Receiver configuration options
typedef uint32_t ReceiverOptions;

//  Set n-th bit in the config flag
constexpr ReceiverOptions MakeReceiverOption(uint8_t n) {
    return ReceiverOptions{ 1 } << n;
}

//  Receiver configuration options
constexpr ReceiverOptions VRG_RECEIVER_OPT_SYNCHRONIZED = MakeReceiverOption(0);

/// <summary>Error codes returned by VRG API. Get detailed error description by calling <see cref="vrgGetErrorMessage" />.</summary>
/// <seealso cref="vrgGetErrorMessage" />
typedef enum {
    VRG_ERROR_OK = 0,
    VRG_ERROR_DP_NOT_CONNECTED = 1,
    VRG_ERROR_UNKNOWN = 4,
    VRG_ERROR_INCOMPLETE = 5,
    VRG_ERROR_UNCHANGED = 6,
    VRG_ERROR_INVALID_ARGUMENT = 7,
    VRG_ERROR_NOT_IMPLEMENTED = 8,
    VRG_ERROR_HMD_NOT_AVAILABLE = 9,
    VRG_ERROR_NOT_INITIALIZED = 10,
    VRG_ERROR_INVALID_VERSION = 11,
    VRG_ERROR_API_LOAD = 12,
    VRG_ERROR_HMD_NOT_PRESENT = -1,
    VRG_ERROR_TRACKER_NOT_FOUND = -2,
    VRG_ERROR_CANNOT_SET_IPD = -5,
    VRG_ERROR_NOT_SUPPORTED = -6,
    VRG_ERROR_IN_PROGRESS = -7,
} VrgErrorCode;

/// <summary>VRG API version.</summary>
typedef struct {
    uint32_t majorVersion;
    uint32_t minorVersion;
} VrgInitInfo;

VRG_API(4, 0)
const VrgInitInfo VRG_API_INIT_INFO;

/// <summary>Graphics API type. Determines headset's rendering pipeline input resources format.</summary>
typedef enum {
    VRG_API_DIRECTX_11 = 0,
    VRG_API_DIRECTX_12 = 1,
    VRG_API_OPENGL = 2,
    VRG_API_VULKAN = 3,
} VrgGraphicsApi;

/// <summary>Hand tracking hand identification.</summary>
typedef enum {
    VRG_HAND_LEFT = 0,
    VRG_HAND_RIGHT = 1,
} VrgHand;

/// <summary>Hand tracking finger identification.</summary>
typedef enum {
    VRG_FINGER_THUMB = 0,
    VRG_FINGER_INDEX = 1,
    VRG_FINGER_MIDDLE = 2,
    VRG_FINGER_RING = 3,
    VRG_FINGER_LITTLE = 4,
} VrgFinger;

/// <summary>Hand tracking finger bone identification.</summary>
typedef enum {
    VRG_FINGER_BONE_METACARPAL = 0,
    VRG_FINGER_BONE_PROXIMAL = 1,
    VRG_FINGER_BONE_INTERMEDIATE = 2,
    VRG_FINGER_BONE_DISTAL = 3,
} VrgFingerBone;

/// <summary>Frame layer identification for the headset rendering pipeline compositor.</summary>
typedef enum {
    VRG_LAYER_LEFT = 0,
    VRG_LAYER_RIGHT = 1,
    VRG_LAYER_LEFT_DETAIL = 3,
    VRG_LAYER_RIGHT_DETAIL = 4,
    VRG_LAYER_LEFT_OVERLAY = 5,
    VRG_LAYER_RIGHT_OVERLAY = 6,
    VRG_LAYER_LEFT_DEPTH = 7,
    VRG_LAYER_RIGHT_DEPTH = 8,
    VRG_LAYER_LEFT_MR_MASK = 9,
    VRG_LAYER_RIGHT_MR_MASK = 10,
} VrgFrameLayer;

/// <summary>Headset boolean properties.</summary>
/// <seealso cref="vrgGetBool" />
/// <seealso cref="vrgSetBool" />
typedef enum {
    /// <summary>Enables or disables IMU. Enabled by default.</summary>
    VRG_BOOL_IMU = 0,
    /// <summary>Enables or disables tray menu. Enabled by default.</summary>
    VRG_BOOL_TRAY_MENU = 1,
    VRG_BOOL_ASYNC_TIMEWARP = 2,
    VRG_BOOL_HAND_TRACKING = 3,
    VRG_BOOL_EYE_TRACKING = 4,
    VRG_BOOL_STATUS_MESSAGES = 5,
    VRG_BOOL_DYNAMIC_RENDER_TARGET_RESOLUTION = 6,
    /// <summary>Enables SRGB mode. Disabled by default. Can't be changed after calling <see cref="vrgAttachGraphicsDevice" />.</summary>
    VRG_BOOL_SRGB = 7,
    VRG_BOOL_FOVEATED_RENDER = 8,
    VRG_BOOL_EXTERNAL_TRACKING = 9,
    VRG_BOOL_EXTERNAL_TRACKING_MERGE = 10,
    VRG_BOOL_POSITION_TRACKING = 11,
    /// <summary>Read-only property to obtain Mixed reality support of current headset.</summary>
    VRG_BOOL_MIXED_REALITY_AVAILABLE = 12,
    /// <summary>Enables alpha channel mode. By default alpha channel of submitted textures is ignored.</summary>
    VRG_BOOL_ALPHA_CHANNEL = 13,
    VRG_BOOL_REMOTE_MIXED_REALITY_AVAILABLE = 14,
    VRG_BOOL_MR_MARKER_DETECTOR = 15,
} VrgBoolProperty;

/// <summary>Headset field of view properties.</summary>
/// <seealso cref="vrgFov" />
typedef enum {
    VRG_FOV_LEFT = 0,
    VRG_FOV_RIGHT = 1,
    VRG_FOV_LEFT_DETAIL = 2,
    VRG_FOV_RIGHT_DETAIL = 3,
    VRG_FOV_LEFT_MR_CAMERA = 4,
    VRG_FOV_RIGHT_MR_CAMERA = 5,
} VrgFovProperty;

/// <summary>Headset float properties.</summary>
/// <seealso cref="vrgGetFloat" />
/// <seealso cref="vrgSetFloat" />
typedef enum {
    VRG_FLOAT_HMD_POSE_PREDICTION_MS = 1,
    VRG_FLOAT_PUPILLARY_DISTANCE_LEFT_M = 2,
    VRG_FLOAT_PUPILLARY_DISTANCE_RIGHT_M = 3,
    VRG_FLOAT_EYE_RELIEF_M = 4,
    VRG_FLOAT_EYE_TO_LENS_DISTANCE = 5,
    VRG_FLOAT_MINIMUM_LENS_DISTANCE = 6,
} VrgFloatProperty;

/// <summary>Headset integer properties.</summary>
/// <seealso cref="vrgGetInt" />
/// <seealso cref="vrgSetInt" />
typedef enum {
    VRG_INT_DISPLAY_RESOLUTION_WIDTH = 0,
    VRG_INT_DISPLAY_RESOLUTION_HEIGHT = 1,
    VRG_INT_DISPLAY_FRAMERATE = 2,
    VRG_INT_RENDER_TARGET_WIDTH = 3,
    VRG_INT_RENDER_TARGET_HEIGHT = 4,
    VRG_INT_RENDER_TARGET_DETAIL_WIDTH = 5,
    VRG_INT_RENDER_TARGET_DETAIL_HEIGHT = 6,
} VrgIntProperty;

/// <summary>Available camera type.</summary>
/// <seealso cref="vrgInitCamera" />
typedef enum {
    VRG_CAMERA_TYPE_AR = 0,
    VRG_CAMERA_TYPE_EYE = 1,
    VRG_CAMERA_TYPE_LEAP_MOTION = 2,
} VrgCameraType;

/// <summary>Headset string properties.</summary>
/// <seealso cref="vrgGetString" />
typedef enum {
    VRG_STRING_SERIAL_NUMBER = 4400,
} VrgStringProperty;

/// <summary>Headset pose properties.</summary>
/// <seealso cref="vrgGetPoseEx" />
typedef enum {
    VRG_POSE_HMD = 0,
    VRG_POSE_CAMERA_LEFT = 1,
    VRG_POSE_CAMERA_RIGHT = 2,
    VRG_POSE_CAMERA_LEFT_DETAIL = 3,
    VRG_POSE_CAMERA_RIGHT_DETAIL = 4,
    VRG_POSE_EYE_LEFT = 5,
    VRG_POSE_EYE_RIGHT = 6,
    VRG_POSE_MR_CAMERA_LEFT = 7,
    VRG_POSE_MR_CAMERA_RIGHT = 8,
} VrgPoseProperty;

/// <summary>Image format.</summary>
/// <seealso cref=" VrgCameraImage" />
typedef enum {
    VRG_IMAGE_FORMAT_UNKNOWN = 0,
    VRG_IMAGE_FORMAT_R8G8B8A8 = 1,
    VRG_IMAGE_FORMAT_GRAY8 = 2,
} VrgImageFormat;

/// <summary>Image camera image type.</summary>
/// <seealso cref=" VrgCameraImageType" />
typedef enum {
    VRG_CAMERA_IMAGE_TYPE_CPU_RAM = 1,
    VRG_CAMERA_IMAGE_TYPE_GPU_CUDA = 2,
    VRG_CAMERA_IMAGE_TYPE_GPU_D3D11 = 4,
    VRG_CAMERA_IMAGE_TYPE_GPU_OPENGL = 8
} VrgCameraImageType;

/// <summary>Deph buffer values mapping.</summary>
typedef enum {
    VRG_DEPTH_ZERO_TO_ONE = 0, // DirectX
    VRG_DEPTH_ONE_TO_ZERO = 1, // Reversed
    VRG_DEPTH_MINUS_ONE_TO_ONE = 2, // Standard OpenGL
    VRG_DEPTH_ZERO_TO_INFINITY = 3, // Infinite projection
    VRG_DEPTH_ONE_TO_INFINITY = 4, // Infinite projection reversed
} VrgDepthMapping;

/// <summary>Structure representing a vector in 3D right-handed coordinate space.</summary>
typedef struct { float x, y, z; } VrgVector3;

/// <summary>Structure representing a quaternion in 3D right-handed coordinate space.</summary>
typedef struct { double x, y, z, w; } VrgQuaternion;

/// <summary>Structure representing transformation matrix in format [row][column].</summary>
typedef struct { float m[4][4]; } VrgMatrix4;

/// <summary>Structure representing single image returned by <see cref="vrgAcquireCameraImage" />.</summary>
/// <seealso cref="VrgImageFormat" />
typedef struct {
    uint32_t width;
    uint32_t height;
    double timestamp;
    void* data;
    VrgImageFormat format;
    uint64_t id;
    size_t pitch;
} VrgCameraImage;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t fps;
} VrgCameraProfile;

typedef struct { uint32_t viewCount; } VrgCameraInfo;

/// <summary>Descriptor of a headset device. Id is the main headset identifier used in majority of the VRG API functions.</summary>
typedef struct {
    uint32_t id;
    char name[255];
} VrgDevice;

/// <summary>Structure representing field of view (angle tangents).
/// <para>Left and bottom tangents are expected to be negative, right and top tangents are expected to be positive.</para>
/// </summary>
typedef struct { float leftTan, rightTan, topTan, bottomTan; } VrgFov;

/// <summary>Structure representing a position and rotation of an object in 3D right-handed coordinate space at particular time.</summary>
typedef struct {
    VrgVector3 position;
    VrgQuaternion orientation;
    int64_t time;
} VrgPose;

/// <summary>Structure representing a single supported tracking system.</summary>
typedef struct {
    uint32_t id;
    char name[255];
} VrgTrackingSystem;

/// <summary>Structure representing a position and rotation of a bone in 3D right-handed coordinate space.</summary>
typedef struct {
    VrgVector3 position;
    VrgQuaternion orientation;
    float length;
} VrgFingerBoneData;

/// <summary>Structure representing finger bones data for returned by the hand tracking functions.</summary>
typedef struct { VrgFingerBoneData fingerBones[4]; } VrgFingerData;

/// <summary>Structure representing a position and rotation of a hand at wrist and all its five fingers.</summary>
typedef struct {
    VrgFingerData fingers[5];
    VrgVector3 wristPosition;
    VrgQuaternion wristOrientation;
    float length;
} VrgHandData;

/// <summary>Structure representing a single controller configuration.</summary>
typedef struct {
    VrgVector3 position;
    VrgQuaternion orientation;
    VrgVector3 velocity;
    VrgVector3 angularVelocity;

    enum {
        VRG_CONTROLLER_UNINITIALIZED = 1,
        VRG_CONTROLLER_CALIBRATING_INPROGRESS = 100,
        VRG_CONTROLLER_CALIBRATING_OUTOFRANGE = 101,
        VRG_CONTROLLER_RUNNING_OK = 200,
        VRG_CONTROLLER_RUNNING_OUTOFRANGE = 201,
    } status;

    enum {
        VRG_CONTROLLER_ROLE_INVALID = 0,
        VRG_CONTROLLER_ROLE_LEFT_HAND = 1,
        VRG_CONTROLLER_ROLE_RIGHT_HAND = 2,
    } role;

    uint32_t packetNumber;
    uint64_t buttonsPressed;
    uint64_t buttonsTouched;

    struct Axis {
        float x;
        float y;
    } axis[5];

    uint8_t valid;
    uint8_t connected;

} VrgControllerData;

/// <summary>
/// Texture formats available for swap-chain creation.
/// </summary>
/// <seealso cref="VrgSwapChainDesc" />
typedef enum {
    VRG_FORMAT_R8G8B8A8_SRGB = 0,
    VRG_FORMAT_R8G8B8A8_UNORM = 1,
    VRG_FORMAT_R8G8B8A8_UINT = 2,
    VRG_FORMAT_D24_UNORM_S8_UINT = 3,
    VRG_FORMAT_B8G8R8A8_SRGB = 4,
    VRG_FORMAT_B8G8R8A8_UNORM = 5,
    VRG_FORMAT_D32_FLOAT = 6,
    VRG_FORMAT_D32_FLOAT_S8X24_UINT = 7,
} VrgTextureFormat;

/// <summary>
/// Texture usage available for swap-chain creation.
/// </summary>
/// <seealso cref="VrgSwapChainDesc" />
typedef enum {
    VRG_BIND_SHADER_RESOURCE = 1 << 0,
    VRG_BIND_RENDER_TARGET = 1 << 1,
    VRG_BIND_DEPTH_STENCIL = 1 << 2,
    VRG_BIND_UNORDERED_ACCESS = 1 << 3
} VrgTextureBindFlags;

/// <summary>
/// VRS update texture API call flags.
/// </summary>
/// <seealso cref="vrgVrsTextureUpdate" />
typedef enum {
    VRG_VRS_UPDATE_NONE = 0,
    VRG_VRS_UPDATE_SIDE_BY_SIDE = 1 << 0,
    VRG_VRS_UPDATE_SIDE_BY_SIDE_FLIPPED = 1 << 1,
} VrgVrsTextureUpdateFlags;

/// <summary>Opaque swap-chain handle that will be filled when creating a swap-chain.</summary>
/// <seealso cref="vrgCreateSwapChain" />
typedef struct { void* handle; } VrgSwapChain;

/// <summary>
/// Descriptor used for swap-chain creation.
/// </summary>
/// <seealso cref="vrgCreateSwapChain" />
typedef struct {
    /// <summary>Texture format.</summary>
    VrgTextureFormat format;
    /// <summary>Texture width.</summary>
    int width;
    /// <summary>Texture height.</summary>
    int height;
    /// <summary>Size of an array in case swap-chain of texture arrays is needed. Use 1 for common texture.</summary>
    int arraySize;
    /// <summary>Combination of VrgTextureBindFlags</summary>
    uint32_t bindFlags;
    /// <summary>Size of swap-chain. Mininum of 2 textures per swap-chain are required. To avoid pipeline stalling and improve performance count should be 3 or more.</summary>
    uint32_t textureCount;
} VrgSwapChainDesc;

/// <summary>
/// Mixed reality overlay mode.
/// </summary>
/// <seealso cref="vrgSetMrOverlayMode" />
typedef enum {
    /// <summary>Virtual scene (with alpha channel) is rendered over the video stream.</summary>
    VRG_MR_OVERLAY_BACKGROUND = 0,
    /// <summary>Video stream is rendered over virtual scene. Chroma key masking can be used to mask video.</summary>
    VRG_MR_OVERLAY_MASKED,
    /// <summary>Video stream overlay will be rendered in virtual scene at absolute position or relatively to headset.</summary>
    VRG_MR_OVERLAY_CINEMA,
} VrgMrOverlayMode;

typedef enum {
    /// <summary>Video stream is always visible (no masking at all).</summary>
    VRG_MR_MASKING_ALWAYS = 0,
    /// <summary>Video stream is masked by chroma key settings.</summary>
    VRG_MR_MASKING_CHROMA_KEY,
    /// <summary>Video stream is masked by texture provided by user.</summary>
    /// <seealso cref="VrgFrameLayer" />
    /// <seealso cref="vrgSubmitFrameLayer" />
    VRG_MR_MASKING_TEXTURE,
    /// <summary>Video stream is masked by combination (logical AND) of user provided texture and chroma key settings.</summary>
    VRG_MR_MASKING_TEXTURE_CHROMA_KEY_AND,
    /// <summary>Video stream is masked by combination (logical XOR) of user provided texture and chroma key settings.</summary>
    VRG_MR_MASKING_TEXTURE_CHROMA_KEY_XOR,
} VrgMrMaskingType;

/// <summary>
/// Mixed reality chroma key masking.
/// </summary>
typedef struct {
    VrgMrMaskingType type;
    float hsv[3];
    float tolerance[3];
} VrgMrOverlayMasking;

/// <summary>
/// Mixed reality marker detection.
/// </summary>
typedef struct {
    int id;
    VrgPose pose;
    int numCameras;
} VrgMrMarker;

/// <summary>
/// Mixed reality marker detector pattern dictionary.
/// </summary>
typedef enum {
    VRG_MARKER_ARUCO_4X4_50 = 0,
    VRG_MARKER_ARUCO_4X4_100,
    VRG_MARKER_ARUCO_4X4_250,
    VRG_MARKER_ARUCO_4X4_1000,
    VRG_MARKER_ARUCO_5X5_50,
    VRG_MARKER_ARUCO_5X5_100,
    VRG_MARKER_ARUCO_5X5_250,
    VRG_MARKER_ARUCO_5X5_1000,
    VRG_MARKER_ARUCO_6X6_50,
    VRG_MARKER_ARUCO_6X6_100,
    VRG_MARKER_ARUCO_6X6_250,
    VRG_MARKER_ARUCO_6X6_1000,
    VRG_MARKER_ARUCO_7X7_50,
    VRG_MARKER_ARUCO_7X7_100,
    VRG_MARKER_ARUCO_7X7_250,
    VRG_MARKER_ARUCO_7X7_1000,
} VrgMrMarkerType;

/// <summary>Callback that provides eye positions called at specified frequency.</summary>
typedef void (*VrgEyePoseCallback)(const uint32_t deviceId, const VrgPose* left, const VrgPose* right, const void* userData);

/// <summary>Initializes the VRG API. This function has to be called before any other API calls.</summary>
/// <param name="info">Initialization information e.g. API version. Use <see cref="VRG_API_INIT_INFO" /> as a default.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_INVALID_ARGUMENT.</returns>
VRG_API(4, 0)
VrgErrorCode vrgInitApi(const VrgInitInfo* info);

/// <summary>Releases and unloads the VRG API. This function has to be called once all the work with VRG SDK is done.</summary>
/// <returns>VRG_ERROR_OK, VRG_ERROR_UNKNOWN.</returns>
VRG_API(4, 0)
VrgErrorCode vrgFreeApi();

/// <summary>Returns string representation of a given error code.</summary>
/// <param name="errorCode">Error code returned from one of the VRG API calls.</param>
/// <param name="message">Pointer to string that will contain string represenation of the error code. Allocated and owned by the VRG API.</param>
/// <returns>VRG_ERROR_OK if textual representation exists, VRG_ERROR_INVALID_ARGUMENT otherwise.</returns>
VRG_API(4, 0)
VrgErrorCode vrgGetErrorMessage(const VrgErrorCode errorCode, const char** message);

/// <summary>Enumerate all available VRG headsets currently attached to the system.</summary>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="devices" /> argument.
/// If the <paramref name="devices" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of all available headsets.
/// If the <paramref name="devices" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="devices" /> array.
/// At most <paramref name="count" /> devices will be stored in the <paramref name="devices" /> array.</param>
/// <param name="devices">Array of <see cref="VrgDevice" /> that will contain descriptors of available headsets. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_INVALID_ARGUMENT if invalid pointer for count is given, VRG_ERROR_INCOMPLETE if more then count items are available.</returns>
/// <seealso cref="VrgDevice" />
VRG_API(4, 0)
VrgErrorCode vrgEnumerateDevices(uint32_t* count, VrgDevice* devices);

/// <summary>Enumerate available tracking systems supported by current VRG API version. Usually the VRG API takes care of tracking systems (tray menu).
/// You don't have to call this function unless you want to manually manage and expose tracking systems in your application.</summary>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="trackingSystems" /> argument.
/// If the <paramref name="trackingSystems" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of all available tracking systems.
/// If the <paramref name="trackingSystems" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="trackingSystems" /> array.
/// At most <paramref name="count" /> tracking systems will be stored in the <paramref name="trackingSystems" /> array.</param>
/// <param name="trackingSystems">Array of <see cref="VrgTrackingSystem" /> that will contain descriptors of available tracking systems. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_INVALID_ARGUMENT if invalid pointer for count is given, VRG_ERROR_INCOMPLETE if more then count items are available.</returns>
VRG_API(4, 0)
VrgErrorCode vrgEnumerateTrackingSystems(uint32_t* count, VrgTrackingSystem* trackingSystems);

/// <summary>Enumerate available controllers attached to the system.</summary>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="ids" /> argument.
/// If the <paramref name="ids" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of all available controllers.
/// If the <paramref name="ids" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="ids" /> array.
/// At most <paramref name="count" /> controller IDs will be stored in the <paramref name="ids" /> array.</param>
/// <param name="ids">Array of uint32_t that will contain descriptors of available controllers. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_INVALID_ARGUMENT if invalid pointer for count is given, VRG_ERROR_INCOMPLETE if more then count items are available.</returns>
VRG_API(4, 0)
VrgErrorCode vrgEnumerateControllers(uint32_t* count, uint32_t* ids);

/// <summary>Gets pose of an object identified by <paramref name="property" />.
/// <para>All poses are returned in right-handed cartesian coordinate system. Neutral pose has positive axis Y oriented up,
/// positive axis X oriented to the right and negative Z axis oriented forward.</para>
/// </summary>
/// <param name="deviceId">Headset to be queried.</param>
/// <param name="property">Queried pose property.</param>
/// <param name="pose" >Output pose. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_INVALID_ARGUMENT if property is not valid or pose is NULL.</returns>
VRG_API(4, 0)
VrgErrorCode vrgGetPoseEx(const uint32_t deviceId, const VrgPoseProperty property, VrgPose* pose);

/// <summary>Sets current position returned from tracking system as a new origin for tracking. Headset position returned by <see cref="vrgGetPoseEx" /> function call will be relative
/// to the position in which headset was located when <see cref="vrgSetTrackingSystemOrigin" /> was called. The origin is part of the headset settings and is persistent across
/// multiple run instances. The origin can be reset by calling <see cref="vrgResetTrackingSystemOrigin" />. Usually the VRG API takes care of tracking system calibration (tray menu).
/// You don't have to call this function unless you want to manually manage and expose tracking system calibration in your application.</summary>
/// <param name="deviceId">Headset for which new origin will be set.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT is invalid device Id is provided.</returns>
/// <seealso cref="vrgResetTrackingSystemOrigin" />
/// <seealso cref="vrgGetPoseEx" />
VRG_API(4, 0)
VrgErrorCode vrgSetTrackingSystemOrigin(const uint32_t deviceId);

/// <summary>Resets tracking position origin previously set by <see cref="vrgSetTrackingSystemOrigin" />. Headset position returned by <see cref="vrgGetPoseEx" /> function call will depend
/// on the actual tracking system being used. Usually the VRG API takes care of tracking system calibration (tray menu).
/// You don't have to call this function unless you want to manually manage and expose tracking system calibration in your application.</summary>
/// <param name="deviceId">Headset for which origin will be reset.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT is invalid device Id is provided.</returns>
/// <seealso cref="vrgSetTrackingSystemOrigin" />
/// <seealso cref="vrgGetPoseEx" />
VRG_API(4, 0)
VrgErrorCode vrgResetTrackingSystemOrigin(const uint32_t deviceId);

/// <summary>Sets active tracking system of given headset. Usually the VRG API takes care of tracking systems (tray menu).
/// You don't have to call this function unless you want to manually manage and expose tracking systems in your application.</summary>
/// <param name="deviceId">Headset for which new tracking is set.</param>
/// <param name="trackerSystemId">Tracking system ID (returned by <see cref="vrgEnumerateTrackingSystems" /> call).</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT is invalid device Id is provided, VRG_ERROR_TRACKER_NOT_FOUND if invalid <paramref name="trackerSystemId" /> is provided.</returns>
/// <seealso cref="vrgEnumerateTrackingSystems" />
VRG_API(4, 0)
VrgErrorCode vrgInitTrackingSystem(const uint32_t deviceId, const uint32_t trackerSystemId);

/// <summary>Gets positional data of a tracking system controller.</summary>
/// <param name="controllerId">Controller ID (returned by <see cref="vrgEnumerateControllers()" /> call).</param>
/// <param name="controllerData">Output controller data. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_TRACKER_NOT_FOUND.</returns>
/// <seealso cref="VrgControllerData" />
VRG_API(4, 0)
VrgErrorCode vrgGetControllerData(const uint32_t controllerId, VrgControllerData* controllerData);

/// <summary>Triggers haptic pulse on controller.</summary>
/// <param name="controllerId">Controller ID (returned by <see cref="vrgEnumerateControllers()" /> call).</param>
/// <param name="axis">Axis on which haptic pulse will be active.</param>
/// <param name="duration">Duration of pulse in microseconds (max. duration is 65 milliseconds).</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_INVALID_ARGUMENT.</returns>
VRG_API(4, 0)
VrgErrorCode vrgControllerTriggerHapticPulse(const uint32_t controllerId, const uint32_t axis, const uint32_t duration);

/// <summary>Initializes headset's rendering pipeline.</summary>
/// <param name="deviceId">Headset to be initialized.</param>
/// <param name="type">Graphics API type.</param>
/// <param name="graphicsDevice">Handle to a graphics device. For VRG_API_DIRECTX_11 pass ID3D11Device. For VRG_API_OPENGL pass HGLRC.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN if initialization failed.</returns>
/// <seealso cref="VrgGraphicsApi" />
VRG_API(4, 0)
VrgErrorCode vrgAttachGraphicsDevice(const uint32_t deviceId, const VrgGraphicsApi type, void* graphicsDevice);

/// <summary>Delimits the beginning of frame layers and metadata submission.
/// Submitted resources are controlled by the VRG API exclusively until the <see cref="vrgEndFrame" /> is called. Any access during the submission may result in undefined behaviour.
/// Headset's rendering pipeline has to be initialized prior submission. See <see cref="vrgAttachGraphicsDevice" />.</summary>
/// <param name="deviceId">Headset that will receive the frame data.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 0)
VrgErrorCode vrgBeginFrame(const uint32_t deviceId);

/// <summary>Submits a frame layer to the rendering pipeline designated by <paramref name="layer">.</summary>
/// <param name="deviceId">Headset that will receive the frame layer.</param>
/// <param name="layer">Frame layer designation.</param>
/// <param name="texture">Texture resource. For VRG_API_DIRECTX_11 pass ID3D11Texture2D. For VRG_API_OPENGL pass address of GLuint.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN.</returns>
/// <seealso cref="VrgFrameLayer" />
VRG_API(4, 0)
VrgErrorCode vrgSubmitFrameLayer(const uint32_t deviceId, const VrgFrameLayer layer, void* texture);

/// <summary>Sets texture coordinates for given frame layer. <remarks>Default values are used if
/// <see cref="vrgSubmitFrameCoordinates" /> was never called for given layer. Default values depend on target graphics API. Coordinates from previus call of <see cref="vrgSubmitFrameCoordinates" />
/// for the layer will be reused.</remarks></summary>
/// <param name="deviceId">Headset for which layer coordinates are set.</param>
/// <param name="layer">Frame layer designation.</param>
/// <param name="uMin">Minimal horizontal texture coordinate.</param>
/// <param name="vMin">Minimal vertical texture coordinate.</param>
/// <param name="uMax">Maximal horizontal texture coordinate.</param>
/// <param name="vMax">Maximal vertical texture coordinate.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="VrgFrameLayer" />
VRG_API(4, 0)
VrgErrorCode vrgSubmitFrameCoordinates(const uint32_t deviceId, const VrgFrameLayer layer, float uMin, float vMin, float uMax, float vMax);

/// <summary>Sets headset pose that given frame layer was rendered with.</summary>
/// <param name="deviceId">Headset that will receive the metadata.</param>
/// <param name="layer">Frame layer designation.</param>
/// <param name="pose">Submitted layer pose. For base layers (VRG_LAYER_LEFT and VRG_LAYER_RIGHT) the pose is expected to be in world space, i.e. it should be combination
/// of pose returned by call to vrgGetPoseEx(..., VRG_POSE_HMD, ...) and pose returned by call to vrgGetPoseEx(..., VRG_POSE_LEFT, ...) (and VRG_POSE_RIGHT respectively).
/// On the other hand poses for detail layers (VRG_LAYER_LEFT_DETAIL and VRG_LAYER_RIGHT_DETAIL) are expected to be in HMD space, i.e. they translate directly to
/// poses returned by call to vrgGetPoseEx(..., VRG_POSE_CAMERA_LEFT_DETAIL, ...) (and VRG_POSE_CAMERA_RIGHT_DETAIL respectively).</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_INVALID_ARGUMENT.</returns>
/// <seealso cref="VrgFrameLayer" />
/// <seealso cref="VrgPose" />
/// <seealso cref="vrgGetPoseEx" />
VRG_API(4, 0)
VrgErrorCode vrgSubmitFramePose(const uint32_t deviceId, const VrgFrameLayer layer, const VrgPose* pose);

/// <summary>Sets the field of view (angle tangents) that given frame layer was rendered with. If no field of view is set for given layer then default values are
/// used if no prior <see cref="vrgSubmitFrameFov" /> was called for given layer. Default values are those that are returned by <see cref="vrgGetFov" /> call. Otherwise values from
/// last call of <see cref="vrgSubmitFrameFov" /> for same layer are used.</summary>
/// <param name="deviceId">Headset that will receive the metadata.</param>
/// <param name="layer">Frame layer designation.</param>
/// <param name="fov">Frame layer field of view.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_INVALID_ARGUMENT.</returns>
/// <seealso cref="VrgFrameLayer" />
/// <seealso cref="VrgFov" />
/// <seealso cref="vrgGetFov" />
VRG_API(4, 0)
VrgErrorCode vrgSubmitFrameFov(const uint32_t deviceId, const VrgFrameLayer layer, const VrgFov* fov);

/// <summary>Sets near and far clipping planes that given frame layer was rendered with. without these values spacewarp reprojection can't be used.</summary>
/// <param name="deviceId">Headset that will receive the metadata.</param>
/// <param name="layer">Frame layer designation.</param>
/// <param name="nearPlane">Frame layer field of view.</param>
/// <param name="farPlane">Frame layer field of view.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_INVALID_ARGUMENT.</returns>
/// <seealso cref="VrgFrameLayer" />
/// <seealso cref="VrgFov" />
VRG_API(4, 1)
VrgErrorCode vrgSubmitFrameClipPlanes(const uint32_t deviceId, const VrgFrameLayer layer, const float nearPlane, const float farPlane);

/// <summary>Delimits the end of frame layers and metadata submission.
/// The control of submitted resources is returned back to the user.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN.</returns>
VRG_API(4, 0)
VrgErrorCode vrgEndFrame(const uint32_t deviceId);

/// <summary>Initializes the headset for use and opens a connection to it. This has to be called before any other API calls that require <paramref name="deviceId" />.
/// Get the ID by enumerating connected devices <seealso cref="vrgEnumerateDevices" />.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_AVAILABLE.</returns>
/// <seealso cref="vrgEnumerateDevices" />
VRG_API(4, 0)
VrgErrorCode vrgInitDevice(const uint32_t deviceId);

/// <summary>Releases the headset and closes the connection to it making it available for other applications.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK.</returns>
VRG_API(4, 0)
VrgErrorCode vrgFreeDevice(const uint32_t deviceId);

/// <summary>Sets interpupillary distance automatically.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 0)
VrgErrorCode vrgRunAutoInterpupillaryDistance(const uint32_t deviceId);

/// <summary>Sets bool property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="flag">Bool property to be changed.</param>
/// <param name="enabled">New value for given property. Value of 0 will disable given property, non-zero values will enable it.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgGetBool" />
/// <seealso cref="VrgBoolProperty" />
VRG_API(4, 0)
VrgErrorCode vrgSetBool(const uint32_t deviceId, const VrgBoolProperty flag, const uint32_t enabled);

/// <summary>Returns current value of a bool property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="flag">Bool property to be retrieved.</param>
/// <param name="enabled">Pointer to a variable which will be filled with the current property value.
/// Value of 0 means the property is disabled. Non-zero value means the property is enabled.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgSetBool" />
/// <seealso cref="VrgBoolProperty" />
VRG_API(4, 0)
VrgErrorCode vrgGetBool(const uint32_t deviceId, const VrgBoolProperty flag, uint32_t* enabled);

/// <summary>Sets float property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be changed.</param>
/// <param name="value">New value for given property.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgGetFloat" />
/// <seealso cref="VrgFloatProperty" />
VRG_API(4, 0)
VrgErrorCode vrgSetFloat(const uint32_t deviceId, const VrgFloatProperty property, const float value);

/// <summary>Returns float property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be retrieved.</param>
/// <param name="value">Pointer to a variable which will be filled with the current property value.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgSetFloat" />
/// <seealso cref="VrgFloatProperty" />
VRG_API(4, 0)
VrgErrorCode vrgGetFloat(const uint32_t deviceId, const VrgFloatProperty property, float* value);

/// <summary>Sets int property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be changed.</param>
/// <param name="value">New value for given property.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgGetInt" />
/// <seealso cref="VrgIntProperty" />
VRG_API(4, 0)
VrgErrorCode vrgSetInt(const uint32_t deviceId, const VrgIntProperty property, const int value);

/// <summary>Returns int property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be retrieved.</param>
/// <param name="value">Pointer to a variable which will be filled with the current property value.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="vrgGetFloat" />
/// <seealso cref="VrgIntProperty" />
VRG_API(4, 0)
VrgErrorCode vrgGetInt(const uint32_t deviceId, const VrgIntProperty property, int* value);

/// <summary>Returns string property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be retrieved.</param>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="value" /> argument.
/// If the <paramref name="value" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of characters.
/// If the <paramref name="value" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="devices" /> array.
/// At most <paramref name="count" /> characters will be stored in the <paramref name="value" /> array.</param>
/// <param name="value">Array of char that will contain the string value of the property. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided,
/// VRG_ERROR_UNKNOWN if other error occured.</returns>
/// <seealso cref="VrgStringProperty" />
VRG_API(4, 0)
VrgErrorCode vrgGetString(const uint32_t deviceId, VrgStringProperty property, uint32_t* count, char* value);

/// <summary>Returns field of view (angle tangents) property of a given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="property">Property to be retrieved.</param>
/// <param name="fov">Pointer to variable that will be filled with field of view property values. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided, VRG_ERROR_INVALID_ARGUMENT if invalid property is provided.</returns>
/// <seealso cref="VrgFovProperty" />
VRG_API(4, 0)
VrgErrorCode vrgGetFov(const uint32_t deviceId, const VrgFovProperty property, VrgFov* fov);

/// <summary>Calibrate inertial measurement unit (IMU) of given headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK if no error occured, VRG_ERROR_HMD_NOT_PRESENT if invalid deviceId is provided.</returns>
VRG_API(4, 0)
VrgErrorCode vrgCalibrateImu(const uint32_t deviceId);

/// <summary>Registers a callback that will periodically report eye pose at specified frequency.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="frequencyHz">Frequency in hertz that eye pose will be reported. Valid range is (0, 120>.</param>
/// <param name="callback">A function pointer to <see cref="VrgEyePoseCallback" />. Pass NULL to unregister.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 0)
VrgErrorCode vrgRegisterEyePoseCallback(const uint32_t deviceId, uint32_t frequencyHz, VrgEyePoseCallback callback);

/// <summary>Triggers a calibration process that will show calibration patterns on the display that the user needs to follow.
/// This is necessary to determine correct eye positions and directions and needs to be called before reading eye pose or registering a callback.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 0)
VrgErrorCode vrgCalibrateEyeTracking(const uint32_t deviceId);

/// <summary>Sets mixed reality video stream overlay visible.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="visible">Set 1 to show overlay, 0 to hide.</param>
/// <param name="profile">Obsolete parameter, can be null. To set camera overlay profile first initialize camera with call to <see cref="vrgInitCamera" />.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 1)
VrgErrorCode vrgSetMrOverlayVisible(const uint32_t deviceId, const uint32_t visible, const VrgCameraProfile* profile);

/// <summary>Sets mixed reality video stream overlay mode.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="mode">Display mode of the overlay.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 1)
VrgErrorCode vrgSetMrOverlayMode(const uint32_t deviceId, const VrgMrOverlayMode mode);

/// <summary>Sets mixed reality video stream masking configuration.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="masking">New configuration for video stream masking.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 1)
VrgErrorCode vrgSetMrOverlayMasking(const uint32_t deviceId, const VrgMrOverlayMasking* masking);

/// <summary>Returns set of detected MR markers.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="markers" /> argument.
/// If the <paramref name="markers" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of detected markers.
/// If the <paramref name="markers" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="markers" /> array.
/// At most <paramref name="count" /> markers will be stored in the <paramref name="markers" /> array.</param>
/// <param name="markers">Array of markers. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 4)
VrgErrorCode vrgGetMrVisibleMarkers(const uint32_t deviceId, uint32_t* count, VrgMrMarker* markers);

/// <summary>Sets MR marker type. Change of MR marker type will be effective next time MR marker detector is enabled by settings bool property
/// VRG_BOOL_MR_MARKER_DETECTOR to 1. If detector is already running it needs to be stopped first by setting bool property VRG_BOOL_MR_MARKER_DETECTOR to 0.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="type">Type of MR marker dictionary.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 4)
VrgErrorCode vrgSetMrMarkerType(const uint32_t deviceId, VrgMrMarkerType type);

/// <summary>Enumerates available camera profiles (resolution and framerate).</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type for which profiles will be enumerated.</param>
/// <param name="count">Count parameter interpretation depends on value of the <paramref name="profiles" /> argument.
/// If the <paramref name="profiles" /> argument is NULL then <paramref name="count" /> is output argument and represent the number of all available profiles.
/// If the <paramref name="profiles" /> argument is not NULL then <paramref name="count" /> is input argument and represents size of <paramref name="profiles" /> array.
/// At most <paramref name="count" /> devices will be stored in the <paramref name="profiles" /> array.</param>
/// <param name="profiles">Array of <see cref="VrgCameraProfile" /> that will contain descriptors of available profiles. NULL may be passed. Allocated by the user.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
VRG_API(4, 1)
VrgErrorCode vrgEnumerateCameraProfiles(const uint32_t deviceId, const VrgCameraType cameraType, uint32_t* count, VrgCameraProfile* profiles);

/// <summary>Initializes a hmd camera with given type and profile.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <param name="profile">Profile a camera will be initialized with. If NULL is passed then default resolution and framerate are used.</param>
/// <param name="outInitInfo">Output argument containing details about initialized camera.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgInitCamera(const uint32_t deviceId, const VrgCameraType cameraType, const VrgCameraProfile* profile, VrgCameraInfo* outInitInfo);

/// <summary>Deinitializes camera previously initizalized by <see cref="vrgInitCamera" />.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgFreeCamera(const uint32_t deviceId, const VrgCameraType cameraType);

/// <summary>Initializes camera frames swpachain. API allows you get frames stored in VRAM directly.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <param name="imageType">Requested image type of camera swapchain. API can provide images in native representations i.e. RAM, D3D11 texture, CUDA memory, OpenGL texture.
/// This function can be called more then once with different image types. In this case multiple camera image representations will be available. See <see cref="vrgAcquireCameraImage" />.</param>
/// <param name="context">When using image type VRG_CAMERA_IMAGE_TYPE_GPU_D3D11 then ID3D11Device should be passed as context. Otherwise NULL value is used.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgInitCameraImageType(const uint32_t deviceId, const VrgCameraType cameraType, const VrgCameraImageType imageType, void* context);

/// <summary>Acquires (locks) last camera frame. Frame is locked until <see cref="vrgReleaseCameraImage" /> is called.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <param name="imageType">Image type.</param>
/// <param name="viewIndex">For camera types that supports multiple frames (left eye, right eye, etc.) this parameter can be used to specify requested frame.</param>
/// <param name="lastFrameId">Specifies last frame that was returned by the API. In case there are no newer frames the API will return other value than VRG_ERROR_OK.</param>
/// <param name="image">Image data of the frame in case VRG_ERROR_OK is returned..</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgAcquireCameraImage(const uint32_t deviceId, const VrgCameraType cameraType, const VrgCameraImageType imageType, const uint32_t viewIndex,
    const uint64_t lastFrameId, VrgCameraImage* image);

/// <summary>Releases (unlocks) frame previously acquired by <see cref="vrgAcquireCameraImage" />.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <param name="imageType">Image type.</param>
/// <param name="viewIndex">See <see cref="vrgAcquireCameraImage" />.</param>
/// <param name="lastFrameId">Specifies frame id that should be released.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgReleaseCameraImage(
    const uint32_t deviceId, const VrgCameraType cameraType, const VrgCameraImageType imageType, const uint32_t viewIndex, const uint64_t lastFrameId);

/// <summary>Checks if new frame for given camera type and image type is available and can be acquired with <see cref="vrgAcquireCameraImage" />.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <param name="imageType">Image type.</param>
/// <param name="viewIndex">See <see cref="vrgAcquireCameraImage" />.</param>
/// <param name="lastFrameId">ID of a last frame acquired by <see cref="vrgAcquireCameraImage" />.</param>
/// <param name="newImageAvailable">Pointer to uint32_t where number of available frames for camera will be stored.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgIsNewCameraImageAvailable(const uint32_t deviceId, const VrgCameraType cameraType, const VrgCameraImageType imageType, const uint32_t viewIndex,
    const uint64_t lastFrameId, uint32_t* newImageAvailable);

/// <summary>Starts a camera of a given time. Once camera starts it will provide new frames.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgStartCamera(const uint32_t deviceId, const VrgCameraType cameraType);

/// <summary>Stops a camera previously started by <see cref="vrgStartCamera" />.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="cameraType">Camera type.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_UNKNOWN</returns>
VRG_API(4, 1)
VrgErrorCode vrgStopCamera(const uint32_t deviceId, const VrgCameraType cameraType);

/// <summary>Gets hand data in absolute coordinates.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="hand">Hand designator. Left or right.</param>
/// <param name="handData">Output hand data. Allocated by the used.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
/// <seealso cref="VrgHand" />
/// <seealso cref="VrVrgHandData" />
VRG_API(4, 0)
VrgErrorCode vrgGetHandData(const uint32_t deviceId, const VrgHand hand, VrgHandData* handData);

/// <summary>Gets hand data in coordinates relative to the headset.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="hand">Hand designator. Left or right.</param>
/// <param name="handData">Output hand data. Allocated by the used.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
/// <seealso cref="VrgHand" />
/// <seealso cref="VrVrgHandData" />
VRG_API(4, 0)
VrgErrorCode vrgGetHandRelativeData(const uint32_t deviceId, const VrgHand hand, VrgHandData* handData);

/// <summary>Sets tracking position for when tracking system is set to "External".</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="pose">New pose for external tracking.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_INVALID_ARGUMENT</returns>
VRG_API(4, 0)
VrgErrorCode vrgSetExternalTrackingPose(const uint32_t deviceId, const VrgPose* pose);

/// <summary>Sets how user-provided depth buffer values are mapped to Z coordinates i.e. determines
/// interpretation of depth VRG_LAYER_LEFT_DEPTH and VRG_LAYER_RIGHT_DEPTH.
/// It is necessary to set the right mapping for spacewarp feature to work correctly.</summary>
/// <param name="deviceId">Headset ID.</param>
/// <param name="depthMapping">Depth mapping type.</param>
/// <returns>VRG_ERROR_OK, VRG_ERROR_HMD_NOT_PRESENT</returns>
/// <seealso cref="vrgSubmitFrameLayer" />
VRG_API(4, 1)
VrgErrorCode vrgSetDepthMapping(uint32_t deviceId, VrgDepthMapping depthMapping);

/// <summary>
/// Creates new swap-chain for given device using a given description.
/// </summary>
/// <param name="deviceId">Headset for which swap-chain will be created.</param>
/// <param name="swapChainDesc">Description of the swap-chain.</param>
/// <param name="swapChain">Opaque swap-chain handle that will be filled with newly created swap-chain.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK, VRG_ERROR_UNKNOWN</returns>
/// <seealso cref="VrgSwapChainDesc" />
/// <seealso cref="VrgSwapChain" />
/// <seealso cref="vrgCreateSwapChainDX12" />
VRG_API(4, 1)
VrgErrorCode vrgCreateSwapChain(const uint32_t deviceId, const VrgSwapChainDesc* swapChainDesc, VrgSwapChain* swapChain);

/// <summary>
/// Destroys swap-chain previously created by vrgCreateSwapChain.
/// </summary>
/// <param name="deviceId">Headset swap-chain belongs to.</param>
/// <param name="swapChain">Swap-chain handle to be deleted.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK</returns>
/// <seealso cref="VrgSwapChain" />
/// <seealso cref="vrgCreateSwapChain" />
VRG_API(4, 1)
VrgErrorCode vrgDestroySwapChain(const uint32_t deviceId, VrgSwapChain* swapChain);

/// <summary>
/// Obtains next index of a texture in given swap-chain that can be used in current frame. This function is blocking. To avoid stalling the swap-chain shoud be created
/// with at least 3 or more textures.
/// </summary>
/// <param name="deviceId">Headset swap-chain belongs to.</param>
/// <param name="swapChain">Swap-chain handle.</param>
/// <param name="index">Output parameter that will contain index of available texture for current frame.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK</returns>
/// <seealso cref="VrgSwapChain" />
/// <seealso cref="vrgSwapChainGetTexture" />
/// <seealso cref="vrgSwapChainCurrentIndexVulkan" />
VRG_API(4, 1)
VrgErrorCode vrgSwapChainCurrentIndex(const uint32_t deviceId, VrgSwapChain* swapChain, uint32_t* index);

/// <summary>
/// Obtains next index of a texture in given swap-chain that can be used in current frame. This function is blocking. To avoid stalling the swap-chain shoud be created
/// with at least 3 or more textures. This is an extended version with API-specific synchronization scheme.
/// </summary>
/// <param name="deviceId">Headset swap-chain belongs to.</param>
/// <param name="swapChain">Swap-chain handle.</param>
/// <param name="index">Output parameter that will contain index of available texture for current frame.</param>
/// <param name="sync">Pointer to API-specific synchronization object. For Vulkan this function expects pointer to VrgVulkanSwapchainSync.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK</returns>
/// <seealso cref="VrgSwapChain" />
/// <seealso cref="vrgSwapChainGetTexture" />
VRG_API(4, 1)
VrgErrorCode vrgSwapChainCurrentIndexEx(const uint32_t deviceId, VrgSwapChain* swapChain, uint32_t* index, void* sync);

/// <summary>
/// Obtains raw pointer to native texture in given swap-chain at given index.
/// </summary>
/// <param name="deviceId">Headset swap-chain belongs to.</param>
/// <param name="swapChain">Swap-chain handle.</param>
/// <param name="index">Index in the swap-chain for which texture will be returned.</param>
/// <param name="texture">Output parameter that will contain raw pointer to native texture.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK</returns>
/// <seealso cref="VrgSwapChain" />
/// <seealso cref="vrgSwapChainGetTextureDX12" />
/// <seealso cref="vrgSwapChainGetTextureVulkan" />
VRG_API(4, 1)
VrgErrorCode vrgSwapChainGetTexture(const uint32_t deviceId, const VrgSwapChain* swapChain, const uint32_t index, void** texture);

/// <summary>
/// Updates content of VRS texture for given layer.
/// </summary>
/// <param name="deviceId">Headset for which VRS texture will be updated.</param>
/// <param name="layer">Target layer.</param>
/// <param name="vrsUpdate">Pointer to graphics API specific data.</param>
/// <returns>VRG_ERROR_INVALID_ARGUMENT, VRG_ERROR_HMD_NOT_PRESENT, VRG_ERROR_OK</returns>
/// <seealso cref="VrgVrsTextureUpdateFlags" />
/// <seealso cref="VrgDx12VrsTextureUpdate" />
/// <seealso cref="vrgVrsTextureUpdateDX12" />
VRG_API(4, 1)
VrgErrorCode vrgVrsTextureUpdate(const uint32_t deviceId, const VrgFrameLayer layer, const void* vrsUpdate);

//  IMU component identification
enum class Sensor {
    Gyroscope,
    GyroscopeAndTempereture,
    Accelerometer,
};

//  IMU component position
enum class SensorPosition {
    CentralBoard,
    LeftEye,
    RightEye,
};

//  Raw data from IMU
struct ImuRawData {
    float x;
    float y;
    float z;
    float temperature;
};

//  Display orientation
enum class Orientation {
    Unknown,
    Landscape,
    LandscapeFlipped,
    Portrait,
    PortraitFlipped,
};

//  Display config geometry
struct TiledConfig {
    bool enabled;
    int horizontal_tiles;
    int vertical_tiles;
    int horizontal_location;
    int vertical_location;
};

//  Diplay (panel) identification
struct Panel {
    char id[32];
    unsigned int sn;
    char displaySn[32];
    char manufacturerId[32];
    char productId[32];
    Orientation orientation;
    TiledConfig tiledConfig;
};

//  Diplay (panel) identification
struct HmdDeviceIdent {
    char sn[32];
    Panel left;
    Panel right;
};

// TODO: Remove.
//  HMD model ID
enum class HmdModelInternal { H, J, G, M, P, AA, Unknown };

// TODO: enum class is C++ feature.
enum class VrgItemInternal {
    //  Camera(s) are related to rendering cameras
    CameraLeft,
    CameraRight,
    //  Eye related configurations
    EyeLeft,
    EyeRight,
    //  Lens are related to lens configuration
    LensLeft,
    LensRight,
    //  Displays are related to the warped images
    DisplayLeft,
    DisplayRight,
    //  Warping related items
    WarpingRaytracer,
    WarpingResampler,
};

enum class VrgFloatPropertyInternal {
    XAxisOffset,
    YAxisOffset,
    ZAxisOffset,
    XAxisRotDeg,
    YAxisRotDeg,
    ZAxisRotDeg,
    MinPd, // Minimum "eye PD" (which is technically a "lens PD") suppported by the headset
    MaxPd, // Maximum "eye PD" (which is technically a "lens PD") suppported by the headset
    EyeReliefOffset, // Eye relief offset applied on top of the eye relief value
    ImageXAxisOffsetRelative, // Relative image shift in viewport (display) space (positive value = further from the nose)
    PupillaryDistance, // Used for eyes and lenses
    DefEyeToLensZDistM, // Default "eye relief" (exactly the camera nodal point to lens center distance in raytracer)
    HeadToLensZDistM, // Distance of the lens center to the tracked head position
};

enum class VrgIntPropertyInternal {
    resolutionWidth,
    resolutionHeight,
};

enum class VrgBoolPropertyInternal {
    OpenVrWarpingSdk,
};

typedef void (*SteamVRTrackingCB)(VrgPose pose);

typedef void (*SteamVRWarpRefreshCB)();

typedef struct { void* handle; } VrgWarping;

// TODO: Move to common.
typedef enum {
    VRG_CHANNEL_RED,
    VRG_CHANNEL_GREEN,
    VRG_CHANNEL_BLUE,
} VrgRGBChannel;

// TODO: Move to common.
typedef enum {
    VRG_EYE_LEFT = 0,
    VRG_EYE_RIGHT = 1,
} VrgEye;

// TODO: Move to common.
typedef struct { double x, y; } VrgPoint2;

typedef struct {
    VrgPoint2 projectionCenter; // projection center of the image from the top left corner
    VrgPoint2 focalLength; // pinhole camera focal length
} VrgProjectionInfo;

typedef struct {
    //should the values for the left eye or right eye be returned?
    VrgEye eye;

    // focus normalized to the range 0 - 1 to which the HMD is physically set
    double focus;

    //left eye pupilary distance to which the HMD is physically set
    double pupilaryDistanceMm;

    // the maximum field of view of the rendered image with which the warping can be used
    VrgFov maximumRenderingFov;

    // distance of the eye center to the lens
    double eyeCenterLensDistanceMm;

    // the tilt angle (in degrees) of the rendering camera in the y axis in OpenGL coordinate system, assuming
    // the observer looks in the negative z axis direction
    double renderingCameraYawDeg;

    //the x dimension of the grid on which the values are computed via raytracing
    //e.g. 128
    // the y dimension is determined according to the aspect ratio of the maximum_rendering_fov
    // determines the precision of the result and the computation time
    int raytracingResolutionWidth;

    // the offset of eye (and of the rendering camera) in the horizontal direction
    //  with respect to the correct eye position for the given hardware configuration
    // = x offset in the OpenGL coordinate system
    double eyeHorizontalOffsetMm;

    // the offset of eye (and of the rendering camera) in the vertical direction
    //  with respect to the correct eye position for the given hardware configuration
    // = y offset in the OpenGL coordinate system
    double eyeVerticalOffsetMm;

    // Enable physically unrealistic extrapolation,
    // achieved by continuing the simulation behind the physical bounds of the surfaces
    bool allowExtrapolation;

    // Dimensions of the grid used to resample the raytraced data
    // in order to obtain regular samples on the display
    VrgPoint2 resamplingResolution;

} VrgWarpingConfiguration;

typedef enum {
    VRG_LAYER_LEFT_OVERLAY_NO_WARPING = 3100,
    VRG_LAYER_RIGHT_OVERLAY_NO_WARPING,
    VRG_LAYER_LEFT_MR_VIDEO,
    VRG_LAYER_RIGHT_MR_VIDEO,
} VrgFrameLayerInternal;

typedef struct {
    VrgVector3 position;
    VrgQuaternion orientation;

    struct Flags {
        uint8_t active : 1;
        uint8_t valid : 1;
        uint8_t reserved : 6;
    } flags;
} VrgTrackerDataInternal;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t id, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t id, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t id, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t id);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t id, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t id, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t id, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t id, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t id, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t id, VrgFovProperty prop, VrgFov* fov);
    void* unused_0; // WaitPresentGetStats
    VrgErrorCode (*SetOpenVRWarpCB)(const uint32_t id, SteamVRWarpRefreshCB cb); // Stub. Not implemented.
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t id, SteamVRTrackingCB cb);
    VrgErrorCode (*InitHmdNoRender)(const uint32_t id);
    void* unused_1; // InitGraphicsOnly
    void* unused_2; // FreeGraphics
    void* unused_3; // SetDisplayPower
    VrgErrorCode (*GetHeadToEyesMatrix)(const uint32_t id, float leftEye[4], float rightEye[4]); // Stub. Not implemented.
} VrgInternalApi;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitHmdNoRender)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    void* unused_0; //VrgErrorCode (*InitGraphicsOnly)(const uint32_t deviceId);
    void* unused_1; //VrgErrorCode (*WaitPresentGetStats)(const uint32_t id, unsigned long long vtime, unsigned int frame);
    void* unused_2; //VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
} VrgInternalApi_1;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetHmdModel)(const uint32_t deviceId, HmdModelInternal* hmdModel);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);

} VrgInternalApi_2;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetHmdModel)(const uint32_t deviceId, HmdModelInternal* hmdModel);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);
    VrgErrorCode (*GetLedCameraPosition)(const uint32_t deviceId, int ledId, int right_eye, VrgVector3* led_output);

    VrgErrorCode (*ShowSettings)();
    VrgErrorCode (*ShowStandaloneSettings)();
    VrgErrorCode (*GetBoolInternal)(const uint32_t deviceId, VrgBoolPropertyInternal prop, bool* value);
    VrgErrorCode (*SetVrgInternalApi)(const uint32_t deviceId, void* api);
} VrgInternalApi_3;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);
    VrgErrorCode (*GetLedCameraPosition)(const uint32_t deviceId, int ledId, int right_eye, VrgVector3* led_output);

    VrgErrorCode (*ShowSettings)();
    VrgErrorCode (*ShowStandaloneSettings)();
    VrgErrorCode (*GetBoolInternal)(const uint32_t deviceId, VrgBoolPropertyInternal prop, bool* value);
    VrgErrorCode (*SetVrgInternalApi)(const uint32_t deviceId, void* api);
} VrgInternalApi_4;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);
    VrgErrorCode (*GetLedCameraPosition)(const uint32_t deviceId, int ledId, int right_eye, VrgVector3* led_output);

    VrgErrorCode (*ShowSettings)();
    VrgErrorCode (*ShowStandaloneSettings)();
    VrgErrorCode (*GetBoolInternal)(const uint32_t deviceId, VrgBoolPropertyInternal prop, bool* value);
    VrgErrorCode (*SetVrgInternalApi)(const uint32_t deviceId, void* api);
} VrgInternalApi_5;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);
    VrgErrorCode (*GetLedCameraPosition)(const uint32_t deviceId, int ledId, int right_eye, VrgVector3* led_output);

    VrgErrorCode (*ShowSettings)();
    VrgErrorCode (*ShowStandaloneSettings)();
    VrgErrorCode (*GetBoolInternal)(const uint32_t deviceId, VrgBoolPropertyInternal prop, bool* value);
    VrgErrorCode (*SetVrgInternalApi)(const uint32_t deviceId, void* api);
    VrgErrorCode (*InitSharedHmdPrimary2)(const uint32_t deviceId, bool usingNativeLightouseTracking, void* clientInternalApi);
    VrgErrorCode (*InitSharedHmdSecondary2)(const uint32_t deviceId, void* clientInternalApi);
    VrgErrorCode (*ShowEyeCalPoint3d)(const uint32_t deviceId, VrgVector3 corePosition, bool right, bool measuring);

    VrgErrorCode (*SubmitFrameFov)(const uint32_t deviceId, const VrgFrameLayerInternal layer, const VrgFov* fov);
    VrgErrorCode (*SubmitFramePose)(const uint32_t deviceId, const VrgFrameLayerInternal layer, const VrgPose* pose);
} VrgInternalApi_6;

typedef struct {
    VrgErrorCode (*GetFocus)(const uint32_t deviceId, int* focus);
    VrgErrorCode (*SubmitFrameLayer)(const uint32_t deviceId, const VrgFrameLayerInternal layer, void* texture);
    VrgErrorCode (*SetTimeWarp)(const uint32_t deviceId, const uint32_t enabled);
    VrgErrorCode (*GrabFrame)(const uint32_t deviceId, const wchar_t* fileName);
    VrgErrorCode (*SetIRLEDs)(const uint32_t deviceId, unsigned char intensity);
    VrgErrorCode (*GetEyeCamIDs)(const uint32_t deviceId, char* l, char* r, bool* old);
    VrgErrorCode (*ShowEyeCalPoint)(const uint32_t deviceId, float x, float y, bool right, bool measuring);
    VrgErrorCode (*HideEyeCalPoint)(const uint32_t deviceId);
    VrgErrorCode (*EnableEyeCalOverlay)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebug)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingSlowdown)(const uint32_t deviceId, bool enable);
    VrgErrorCode (*SetEyeTrackingDebugLevel)(const uint32_t deviceId, int level);
    // SteamVr extensions
    VrgErrorCode (*GetStaticWarpMap)(const uint32_t deviceId, float u, float v, bool rightEye, float (&uw)[3], float (&vw)[3]);
    VrgErrorCode (*InitLighthouseInternalDriver)(void* host);
    VrgErrorCode (*GetNoWarpFov)(const uint32_t deviceId, VrgFovProperty prop, VrgFov* fov);
    VrgErrorCode (*SetOpenVRTrackingCB)(const uint32_t deviceId, SteamVRTrackingCB cb);
    VrgErrorCode (*InitSharedHmdPrimary)(const uint32_t deviceId, bool usingNativeLightouseTracking);
    VrgErrorCode (*InitSharedHmdSecondary)(const uint32_t deviceId);
    VrgErrorCode (*GetFrameStats)(const uint32_t deviceId, uint64_t* timeOfLastVSync, uint32_t* frameIndex);
    VrgErrorCode (*WaitForPresent)(const uint32_t deviceId);
    VrgErrorCode (*SetDisplayPower)(bool enable);
    VrgErrorCode (*RegisterEventReceiver)(
        const uint32_t deviceId, EventType eventMask, EventCallback eventCallback, EventContext eventContext, ReceiverOptions receiverOptions);
    VrgErrorCode (*UnregisterEventReceiver)(const uint32_t deviceId, EventCallback eventCallback, EventContext eventContext);
    VrgErrorCode (*SignalEvent)(const uint32_t deviceId, EventType eventMask);
    VrgErrorCode (*GetHmdDeviceIdent)(const uint32_t deviceId, HmdDeviceIdent* hmdDeviceIdent);
    VrgErrorCode (*GetHmdMinMaxPdMeters)(const uint32_t deviceId, float* minPd, float* maxPd);
    VrgErrorCode (*GetMotionCancellationConfig)(bool* isMoCaOn, char* snBuff, size_t snBuffsize);
    VrgErrorCode (*InitOpenVr)();
    VrgErrorCode (*EnumerateSharedDevices)(uint32_t* count, VrgDevice* devices);
    VrgErrorCode (*SubmitFrameCoordinates)(const uint32_t deviceId, const VrgFrameLayerInternal layer, float uMin, float vMin, float uMax, float vMax);
    VrgErrorCode (*GetFloatInternal)(const uint32_t deviceId, VrgItemInternal item, VrgFloatPropertyInternal prop, float* value);
    VrgErrorCode (*GetIntInternal)(const uint32_t deviceId, VrgItemInternal item, VrgIntPropertyInternal prop, int* value);

    // TODO: Warping interface?
    VrgErrorCode (*WarpComputeProjectionInfo)(const VrgFov* fov, VrgProjectionInfo* projectionInfo);
    VrgErrorCode (*WarpComputeWarping)(const uint32_t deviceId, const VrgWarpingConfiguration* warpingConfig, VrgWarping* warping);
    VrgErrorCode (*WarpComputeRenderedImageUv)(const VrgWarping* warping, VrgRGBChannel channel, const VrgProjectionInfo* renderedImageProjection,
        const VrgPoint2* displayPosition, VrgPoint2* imagePosition);
    VrgErrorCode (*WarpReleaseWarping)(VrgWarping* warpingId);
    VrgErrorCode (*GetLedCameraPosition)(const uint32_t deviceId, int ledId, int right_eye, VrgVector3* led_output);

    VrgErrorCode (*ShowSettings)();
    VrgErrorCode (*ShowStandaloneSettings)();
    VrgErrorCode (*GetBoolInternal)(const uint32_t deviceId, VrgBoolPropertyInternal prop, bool* value);
    VrgErrorCode (*SetVrgInternalApi)(const uint32_t deviceId, void* api);
    VrgErrorCode (*InitSharedHmdPrimary2)(const uint32_t deviceId, bool usingNativeLightouseTracking, void* clientInternalApi);
    VrgErrorCode (*InitSharedHmdSecondary2)(const uint32_t deviceId, void* clientInternalApi);
    VrgErrorCode (*ShowEyeCalPoint3d)(const uint32_t deviceId, VrgVector3 corePosition, bool right, bool measuring);

    // Graphics debugging.
    VrgErrorCode (*InjectCompositorDevice)(void* device, void* hwnd);
    VrgErrorCode (*SetVirtualHmd)(const uint32_t virtualHmd, const uint32_t tiled);

    VrgErrorCode (*UpdateTrackerData)();
    VrgErrorCode (*GetTrackerData)(const uint32_t tracker, const float predictedSeconds, VrgTrackerDataInternal* data);
    VrgErrorCode (*GetTrackerNames)(uint32_t* count, const char** names);
} VrgInternalApi_7;

VRG_API(4, 0) VrgErrorCode vrgGetInternalApi(VrgInternalApi* internalApi, const int version);

VRG_API(4, 0) VrgErrorCode vrgGetInternalApi_1(const VrgInternalApi_1** internalApi);

VRG_API(4, 1) VrgErrorCode vrgGetInternalApi_2(const VrgInternalApi_2** internalApi);

VRG_API(4, 1) VrgErrorCode vrgGetInternalApi_3(const VrgInternalApi_3** internalApi);

VRG_API(4, 1) VrgErrorCode vrgGetInternalApi_4(const VrgInternalApi_4** internalApi);

VRG_API(4, 2) VrgErrorCode vrgGetInternalApi_5(const VrgInternalApi_5** internalApi);

VRG_API(4, 3) VrgErrorCode vrgGetInternalApi_6(const VrgInternalApi_6** internalApi);

VRG_API(4, 4) VrgErrorCode vrgGetInternalApi_7(const VrgInternalApi_7** internalApi);

VRG_API(4, 1) VrgErrorCode vrgShowStandaloneSettings();

/// Deprecated. Replaced by vrgGetInt.
VRG_API(4, 0) VrgErrorCode vrgGetResolutionAndFramerate(const uint32_t deviceId, uint32_t* width, uint32_t* height, uint32_t* framerate);

/// Deprecated. Replaced by vrgGetFrustum property API.
VRG_API(4, 0) VrgErrorCode vrgGetRawProjectionFrustum(const uint32_t deviceId, VrgFov* left, VrgFov* right);

/// Deprecated. Replaced by vrgGetInt.
VRG_API(4, 0) VrgErrorCode vrgGetTargetResolution(const uint32_t deviceId, uint32_t* width, uint32_t* height);

/// Deprecated. Replaced by vrgGetInt.
VRG_API(4, 0) VrgErrorCode vrgGetTargetDetailResolution(const uint32_t deviceId, uint32_t* width, uint32_t* height);

/// Deprecated. Replaced by vrgGetEyePose.
VRG_API(4, 0) VrgErrorCode vrgGetEyeRotation(const uint32_t deviceId, bool leftEye, VrgQuaternion* rotation);

/// Deprecated. Replaced by vrgGetFloat.
VRG_API(4, 0) VrgErrorCode vrgGetPupillaryDistance(uint32_t deviceId, float* left, float* right);

/// Deprecated. Replaced by vrgSetFloat.
VRG_API(4, 0) VrgErrorCode vrgSetPupillaryDistance(uint32_t deviceId, float left, float right);

/// Deprecated. Replaced by vrgGetFrustum property API.
VRG_API(4, 0) VrgErrorCode vrgGetRawProjectionFrustumDetail(const uint32_t deviceId, VrgFov* left, VrgFov* right);

/// Deprecated. Replaced by vrgGetPoseEx.
VRG_API(4, 0) VrgErrorCode vrgGetPose(const uint32_t deviceId, VrgPose* pose);

/// Deprecated. Replaced by vrgSetTrackingSystemOrigin.
VRG_API(4, 0) VrgErrorCode vrgCalibrateTrackingSystem(const uint32_t deviceId);

/// Deprecated. Replaced by vrgCalibrateImu.
VRG_API(4, 0) VrgErrorCode vrgCalibrateIMU(const uint32_t deviceId);

