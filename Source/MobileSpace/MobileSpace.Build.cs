// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileSpace : ModuleRules
{
    public MobileSpace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // === DEPENDENCIAS PÚBLICAS ===
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
            "OnlineSubsystemUtils"
        });

        // === DEPENDENCIAS PRIVADAS ===
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        // === MÓDULOS CARGADOS DINÁMICAMENTE ===
        DynamicallyLoadedModuleNames.AddRange(new string[]
        {
            "OnlineSubsystemNull"  // ? Solo aquí
        });

        // === FORZAR COMPILACIÓN EN EDITOR (SIN REPETIR OnlineSubsystemNull) ===
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "OnlineSubsystemUtils",
                "OnlineSubsystem"
                // ? ¡NO agregues OnlineSubsystemNull aquí!
            });
        }

        // === PARA MÓVIL (opcional) ===
        if (Target.Platform == UnrealTargetPlatform.Android || Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.Add("Launch");
        }
    }
}