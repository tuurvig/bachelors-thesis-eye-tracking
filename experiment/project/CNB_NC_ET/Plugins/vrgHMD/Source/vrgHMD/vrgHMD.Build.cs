// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{

    public class vrgHMD : ModuleRules
    {
        public vrgHMD(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PrivateIncludePaths.AddRange(
                new string[] {
                    "vrgHMD/Private",
                }
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "RHI",
                    "RenderCore",
                    "Renderer",
                    "HeadMountedDisplay"
                }
                );

            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "RHI",
                    "RenderCore",
                    "Renderer",
                    "HeadMountedDisplay" });

            var BuildVersion = Target.Version;

            if (BuildVersion.MajorVersion == 4 && BuildVersion.MinorVersion < 22)
            {
                PrivateDependencyModuleNames.Add("ShaderCore");
                PublicDependencyModuleNames.Add("ShaderCore");
            }

            if (Target.bBuildEditor == true)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }

            PrivateDependencyModuleNames.Add("Slate");
            PrivateDependencyModuleNames.Add("SlateCore");

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
                RuntimeDependencies.Add(Path.Combine(BaseDirectory, "ThirdParty", "hmd64.dll"));
            }
        }
    }
}
