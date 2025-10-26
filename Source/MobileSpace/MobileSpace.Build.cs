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
            "Networking"    
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        // Opcionales si planeas hacer builds Android con red
        //bEnableExceptions = true;   // permite uso seguro de excepciones C++
        //bUseRTTI = true;            // habilita runtime type info
        //bEnableUndefinedIdentifierWarnings = false; // opcional para compatibilidad
    }
}
