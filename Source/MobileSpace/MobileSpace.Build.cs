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
            "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        bEnableExceptions = true;
        bUseRTTI = true;
        //bEnableUndefinedIdentifierWarnings = false;
    }
}
