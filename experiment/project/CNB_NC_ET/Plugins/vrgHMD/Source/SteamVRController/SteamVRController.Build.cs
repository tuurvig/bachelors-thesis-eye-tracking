// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SteamVRController : ModuleRules
{
    public SteamVRController(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateIncludePathModuleNames.AddRange(new string[]
        {
            "TargetPlatform",
            "vrgHMD"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputDevice",
            "HeadMountedDisplay",
            "vrgHMD",
            "InputCore",
            "ApplicationCore"
        });

        var BuildVersion = Target.Version;

        if (BuildVersion.MajorVersion == 4 && BuildVersion.MinorVersion >= 25)
        {
            PrivateDependencyModuleNames.Add("RenderCore");
            PrivateDependencyModuleNames.Add("RHI");
        }

        AddEngineThirdPartyPrivateStaticDependencies(Target,
            "OpenVR"
        );
    }
}
