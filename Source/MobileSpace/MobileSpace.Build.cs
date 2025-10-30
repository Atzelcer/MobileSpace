// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileSpace : ModuleRules
{
    public MobileSpace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // --- Módulos públicos requeridos por el juego ---
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",                 // Núcleo del motor Unreal
            "CoreUObject",          // Objetos base del motor
            "Engine",               // Subsistema principal de render, físicas, etc.
            "InputCore",            // Soporte de entrada (teclado, touch, gamepad)
            "UMG",                  // Interfaz de usuario (Widgets)
            "Slate",                // Sistema de interfaz Slate
            "SlateCore",            // Núcleo de Slate
            "Niagara",              // Efectos visuales y partículas
            "AIModule",             // Inteligencia artificial
            "NavigationSystem",     // Navegación en IA
            "GameplayTasks",        // Tareas de gameplay asíncronas
            "Sockets",              // Comunicación en red a bajo nivel
            "Networking",           // Red básica (UDP/TCP)
            "OnlineSubsystem",      // Sistema en línea (base)
            "OnlineSubsystemUtils", // Herramientas de red avanzadas
            "EngineSettings"        // Configuración del proyecto
        });

        // --- Dependencias privadas (solo visibles dentro del módulo) ---
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "UMG",
            "Slate",
            "SlateCore"
        });

        // --- Módulos dinámicos cargados en tiempo de ejecución ---
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
        }

        // --- Configuración para builds "Shipping" ---
        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            bUseUnity = true;
            bEnableExceptions = false;
            OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        }
    }
}
