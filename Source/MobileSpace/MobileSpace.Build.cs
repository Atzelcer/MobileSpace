// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileSpace : ModuleRules
{
    public MobileSpace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "Niagara",
            "AIModule",
            "NavigationSystem",
            "GameplayTasks",
            "Sockets",
            "Networking",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "EngineSettings"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]
        {
            "OnlineSubsystemNull"
        });
    }
}
