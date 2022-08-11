#include "VRgHMDBPLibrary.h"
#include "VRgHMD.h"

FVRgHMD* GetVRgHMD()
{
#if UVER < 418
	if (GEngine->HMDDevice.IsValid() && (GEngine->HMDDevice->GetHMDDeviceType() == EHMDDeviceType::DT_ES2GenericStereoMesh))/*chng*/
	{
		return static_cast<FVRgHMD*>(GEngine->HMDDevice.Get());
	}
#else
	static FName SystemName(TEXT("VRgHMD"));
	if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName))
	{
		return static_cast<FVRgHMD*>(GEngine->XRSystem.Get());
	}

#endif

	return nullptr;
}


UVRgHMDBPLibrary::UVRgHMDBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	connected = false;
//	quaternion = FQuat(0, 0, 0, 1.0f);
	Recording = false;
	outString = FString("");
}

bool recording = false;

bool UVRgHMDBPLibrary::StartRecordEyes(FString name)
{
	if (recording)return true;
	FVRgHMD* hmd = GetVRgHMD();
	if (hmd)
	{
		recording = hmd->StartRecordEyes(name);
		return recording;
	}
	return false;
}

void UVRgHMDBPLibrary::StopRecordEyes()
{
	FVRgHMD* hmd = GetVRgHMD();
	if (hmd && recording)
	{
		if(recording)hmd->StopRecordEyes();
	}
	recording = false;
}

bool UVRgHMDBPLibrary::CalibrateEyes()
{
	FVRgHMD* hmd = GetVRgHMD();
	if (hmd && recording)
	{
		return hmd->CalibrateEyes();
	}
	return false;
}

int UVRgHMDBPLibrary::IsCalibratingEyes()
{
	FVRgHMD* hmd = GetVRgHMD();
	if (hmd)
	{
		return hmd->IsCalibrating();
	}
	return -1;
}

void UVRgHMDBPLibrary::AutoIPD()
{
	FVRgHMD* hmd = GetVRgHMD();
	if (hmd && !recording)
	{
		hmd->AutoIPD();
	}
}

/*bool UVRgHMDBPLibrary::recording = false;
bool UVRgHMDBPLibrary::eye_detected = false;
bool UVRgHMDBPLibrary::eye_open = false;
FVector2D UVRgHMDBPLibrary::eye_left_uv = FVector2D(0,0);
FVector2D UVRgHMDBPLibrary::eye_right_uv = FVector2D(0, 0);
*/