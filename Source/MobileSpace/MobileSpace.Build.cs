// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileSpace : ModuleRules
{
    public MobileSpace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // --- M�dulos p�blicos requeridos por el juego ---
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",                 // N�cleo del motor Unreal
            "CoreUObject",          // Objetos base del motor
            "Engine",               // Subsistema principal de render, f�sicas, etc.
            "InputCore",            // Soporte de entrada (teclado, touch, gamepad)
            "UMG",                  // Interfaz de usuario (Widgets)
            "Slate",                // Sistema de interfaz Slate
            "SlateCore",            // N�cleo de Slate
            // "Niagara",              // REMOVIDO - causaba crashes en Android
            "AIModule",             // Inteligencia artificial
            "NavigationSystem",     // Navegaci�n en IA
            "GameplayTasks",        // Tareas de gameplay as�ncronas
            "Sockets",              // Comunicaci�n en red a bajo nivel
            "Networking",           // Red b�sica (UDP/TCP)
            "OnlineSubsystem",      // Sistema en l�nea (base)
            "OnlineSubsystemUtils", // Herramientas de red avanzadas
            "EngineSettings"        // Configuraci�n del proyecto
        });

        // --- Dependencias privadas (solo visibles dentro del m�dulo) ---
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        // --- M�dulos din�micos cargados en tiempo de ejecuci�n ---
        DynamicallyLoadedModuleNames.AddRange(new string[]
        {
            "OnlineSubsystemNull"   // Activa soporte LAN local
        });

        // --- Configuración específica por plataforma ---
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // Activa optimización móvil
            bEnableExceptions = false;
            bUseUnity = true;
            bEnableUndefinedIdentifierWarnings = false;

            // Librerías adicionales Android (si se usan sockets directos)
            PublicAdditionalLibraries.Add("android");

            // Configuración específica para partículas en Android (Niagara removido)
        }

        // --- Configuraci�n para builds "Shipping" ---
        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            bUseUnity = true;
            bEnableExceptions = false;
            OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        }
    }
}
