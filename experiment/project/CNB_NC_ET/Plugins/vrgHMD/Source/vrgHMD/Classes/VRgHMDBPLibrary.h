#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Sockets.h"

#include "VRgHMDBPLibrary.generated.h"



/*
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UVRgHMDBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VRg Start Record Eyes", Keywords = "VRg Record Start Eyes HMD"), Category = "VRg")
		static bool StartRecordEyes(FString name = FString("record_file"));

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VRg Stop Record Eyes", Keywords = "VRg Stop Record Eyes HMD"), Category = "VRg")
		static void StopRecordEyes();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VRg Start Calibration Eyes", Keywords = "VRg Calibration Eyes HMD"), Category = "VRg")
		static bool CalibrateEyes();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VRg Eye Calibration Status", Keywords = "VRg Calibration Eyes Status HMD"), Category = "VRg")
		static int IsCalibratingEyes();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VRg AutoIPD Calibration", Keywords = "VRg Calibration Auto IPD HMD"), Category = "VRg")
		static void AutoIPD();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
		bool Recording;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
	//	static FQuat quaternion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
		FVector2D eye_left_uv;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
		FVector2D eye_right_uv;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
		bool eye_detected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRg")
		bool eye_open;

private:
	//FSocket *Socket;

	bool connected;

	FString outString;
};
