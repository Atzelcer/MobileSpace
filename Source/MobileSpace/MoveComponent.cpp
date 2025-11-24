#include "MoveComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be ticked every frame.  You can turn this off to improve performance if not needed.
	PrimaryComponentTick.bCanEverTick = true;
	Elapsed = 0.f;
}

// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		Origin = GetOwner()->GetActorLocation();
	}
}

// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	Elapsed += DeltaTime;

	FVector NewLocation = Origin;
	float t = Elapsed;

	switch (Pattern)
	{
	case EArcadeMovement::None:
		return;
		
	case EArcadeMovement::Wave:
		NewLocation.X -= Speed * t;
		NewLocation.Y += Amplitude * FMath::Sin(Frequency * t);
		break;

	case EArcadeMovement::SEntry:
		{
			float EntryTime = 2.0f;
			if (t < EntryTime)
			{
				float Progress = t / EntryTime;
				float Curve = FMath::Sin(Progress * PI * 0.5f);
				NewLocation.X = Origin.X - Speed * t * Curve;
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * t * 2.0f) * (1.0f - Progress);
			}
			else
			{
				NewLocation.X -= Speed * t;
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * (t - EntryTime));
			}
		}
		break;

	case EArcadeMovement::RadialBurst:
		{
			float BurstRadius = Amplitude * t;
			float Angle = Frequency * t;
			NewLocation.X = Origin.X + BurstRadius * FMath::Cos(Angle);
			NewLocation.Y = Origin.Y + BurstRadius * FMath::Sin(Angle);
			if (BurstRadius > Amplitude * 3.0f)
			{
				Elapsed = 0.f;
			}
		}
		break;

	case EArcadeMovement::WavyWave:
		{
			// Patrón complejo en forma de rosa
			float T1 = 2.1f;
			float T2 = 0.4f; 
			float T3 = 0.7f;
			FVector2D Center = (MovementMin + MovementMax) * 0.5f;

			float Phase = FMath::Fmod(t, T1 + T2 + T3);

			if (Phase < T1)
			{
				float u = 2.f * PI * Phase / T1;
				NewLocation.X = Center.X + 110.f * FMath::Sin(u) * (FMath::Exp(FMath::Cos(u)) - 2 * FMath::Cos(4 * u));
				NewLocation.Y = Center.Y + 90.f * FMath::Cos(u);
			}
			else if (Phase < T1 + T2)
			{
				float k = (Phase - T1) / T2;
				float DX = MovementMin.X + FMath::Fmod(GetOwner()->GetUniqueID() * 147, int(MovementMax.X - MovementMin.X));
				float DY = MovementMin.Y + FMath::Fmod(GetOwner()->GetUniqueID() * 71, int(MovementMax.Y - MovementMin.Y));
				NewLocation.X = FMath::Lerp(NewLocation.X, DX, k);
				NewLocation.Y = FMath::Lerp(NewLocation.Y, DY, k);
			}
			else
			{
				float k = (Phase - T1 - T2) / T3;
				NewLocation.X = FMath::Lerp(NewLocation.X, Center.X, k) + 40.f * FMath::Cos(k * 4.5f);
				NewLocation.Y = FMath::Lerp(NewLocation.Y, Center.Y, k) + 35.f * FMath::Sin(k * 3.2f);
			}
		}
		break;

	case EArcadeMovement::ExpandingCircle:
		{
			float CircleRadius = Amplitude + Speed * t * 0.1f;
			float CircleSpeed = Frequency * t;
			NewLocation.X = Origin.X + CircleRadius * FMath::Cos(CircleSpeed);
			NewLocation.Y = Origin.Y + CircleRadius * FMath::Sin(CircleSpeed);
		}
		break;

	case EArcadeMovement::BossMajesticArc:
		{
			// Arco majestuoso para jefes
			float ArcProgress = FMath::Sin(Frequency * t) * 0.5f + 0.5f;
			NewLocation.X = Origin.X - Speed * t * 0.3f; // Movimiento lento hacia adelante
			NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * t * 0.5f); // Arco lento
			NewLocation.Z = Origin.Z + 50.0f * ArcProgress; // Flotación vertical
		}
		break;

	case EArcadeMovement::SuperSequence:
		{
			// Secuencia súper compleja
			float Phase = FMath::Fmod(t, 8.0f) / 8.0f;
			if (Phase < 0.25f)
			{
				float LocalT = Phase / 0.25f;
				NewLocation.X -= Speed * LocalT;
				NewLocation.Y += Amplitude * FMath::Sin(Frequency * LocalT * 4.0f);
			}
			else if (Phase < 0.5f)
			{
				float LocalT = (Phase - 0.25f) / 0.25f;
				NewLocation.X += Speed * LocalT * 0.5f;
				NewLocation.Y += Amplitude * FMath::Cos(Frequency * LocalT * 6.0f);
			}
			else if (Phase < 0.75f)
			{
				float LocalT = (Phase - 0.5f) / 0.25f;
				float Spiral = LocalT * 2.0f * PI;
				NewLocation.X = Origin.X + (Amplitude * LocalT) * FMath::Cos(Spiral);
				NewLocation.Y = Origin.Y + (Amplitude * LocalT) * FMath::Sin(Spiral);
			}
			else
			{
				float LocalT = (Phase - 0.75f) / 0.25f;
				NewLocation.X = Origin.X - Speed * LocalT;
				NewLocation.Y = Origin.Y;
			}
		}
		break;

	// ===== PATRONES ESPECÍFICOS PARA JEFES =====
	case EArcadeMovement::BossSlowSweep:
		{
			// Movimiento lento y elegante de lado a lado
			float SweepRange = Amplitude * 2.0f;
			NewLocation.X = Origin.X - Speed * t * 0.2f; // Avance muy lento
			NewLocation.Y = Origin.Y + SweepRange * FMath::Sin(Frequency * t * 0.3f); // Barrido lento
		}
		break;

	case EArcadeMovement::BossThreatening:
		{
			// Movimiento amenazante con paradas súbitas
			float ThreatCycle = 4.0f; // 4 segundos por ciclo
			float CycleTime = FMath::Fmod(t, ThreatCycle);
			
			if (CycleTime < 2.0f)
			{
				// Movimiento normal por 2 segundos
				NewLocation.X = Origin.X - Speed * CycleTime * 0.1f;
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * CycleTime);
			}
			else if (CycleTime < 2.5f)
			{
				// Parada súbita y temblor por 0.5 segundos
				float ShakeIntensity = 20.0f;
				NewLocation.X = Origin.X - Speed * 2.0f * 0.1f + FMath::RandRange(-ShakeIntensity, ShakeIntensity);
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * 2.0f) + FMath::RandRange(-ShakeIntensity, ShakeIntensity);
			}
			else
			{
				// Avance agresivo por 1.5 segundos
				float AggressiveTime = CycleTime - 2.5f;
				NewLocation.X = Origin.X - Speed * 2.0f * 0.1f - Speed * AggressiveTime * 0.3f;
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * (2.0f + AggressiveTime * 2.0f));
			}
		}
		break;

	case EArcadeMovement::BossCircularDominance:
		{
			// Círculo grande y dominante
			float BigRadius = Amplitude * 3.0f;
			float SlowRotation = Frequency * t * 0.2f;
			NewLocation.X = Origin.X + BigRadius * FMath::Cos(SlowRotation) - Speed * t * 0.1f;
			NewLocation.Y = Origin.Y + BigRadius * FMath::Sin(SlowRotation);
			
			// Pulsación del radio para efecto dominante
			float Pulse = 1.0f + 0.3f * FMath::Sin(Frequency * t * 2.0f);
			NewLocation.X *= Pulse;
			NewLocation.Y *= Pulse;
		}
		break;

	case EArcadeMovement::BossErraticPower:
		{
			// Movimiento errático pero poderoso
			float ErraticCycle = 3.0f;
			float CycleProgress = FMath::Fmod(t, ErraticCycle) / ErraticCycle;
			
			// Uso de múltiples frecuencias para crear caos controlado
			float Chaos1 = FMath::Sin(Frequency * t * 3.7f) * Amplitude;
			float Chaos2 = FMath::Cos(Frequency * t * 2.3f) * Amplitude * 0.7f;
			float Chaos3 = FMath::Sin(Frequency * t * 5.1f) * Amplitude * 0.4f;
			
			NewLocation.X = Origin.X - Speed * t * 0.15f + Chaos1;
			NewLocation.Y = Origin.Y + Chaos2 + Chaos3;
			
			// Burst súbitos de velocidad
			if (FMath::Fmod(t, 1.0f) < 0.1f)
			{
				NewLocation.X -= Speed * 0.5f;
			}
		}
		break;

	case EArcadeMovement::BossEpicFinal:
		{
			// Movimiento épico para jefe final
			float EpicPhase = FMath::Fmod(t, 12.0f); // 12 segundos de ciclo épico
			
			if (EpicPhase < 3.0f)
			{
				// Fase 1: Círculos concéntricos
				float InnerRadius = Amplitude * FMath::Sin(Frequency * EpicPhase);
				float OuterRadius = Amplitude * 2.0f * FMath::Cos(Frequency * EpicPhase * 0.7f);
				NewLocation.X = Origin.X + InnerRadius * FMath::Cos(Frequency * EpicPhase * 2.0f);
				NewLocation.Y = Origin.Y + OuterRadius * FMath::Sin(Frequency * EpicPhase * 1.3f);
			}
			else if (EpicPhase < 6.0f)
			{
				// Fase 2: Figura de ocho épica
				float EightTime = (EpicPhase - 3.0f) * 2.0f;
				NewLocation.X = Origin.X + Amplitude * 1.5f * FMath::Sin(Frequency * EightTime);
				NewLocation.Y = Origin.Y + Amplitude * FMath::Sin(Frequency * EightTime * 2.0f);
			}
			else if (EpicPhase < 9.0f)
			{
				// Fase 3: Espiral dorada
				float SpiralTime = EpicPhase - 6.0f;
				float SpiralRadius = Amplitude * (1.0f + SpiralTime * 0.3f);
				float SpiralAngle = Frequency * SpiralTime * 3.0f;
				NewLocation.X = Origin.X + SpiralRadius * FMath::Cos(SpiralAngle);
				NewLocation.Y = Origin.Y + SpiralRadius * FMath::Sin(SpiralAngle);
			}
			else
			{
				// Fase 4: Caos controlado final
				float ChaosTime = EpicPhase - 9.0f;
				float Multi1 = FMath::Sin(Frequency * ChaosTime * 4.0f) * Amplitude;
				float Multi2 = FMath::Cos(Frequency * ChaosTime * 3.0f) * Amplitude * 0.8f;
				float Multi3 = FMath::Sin(Frequency * ChaosTime * 5.5f) * Amplitude * 0.6f;
				
				NewLocation.X = Origin.X + Multi1 + Multi3;
				NewLocation.Y = Origin.Y + Multi2 - Multi3;
				
				// Pulsación épica final
				float EpicPulse = 1.0f + 0.5f * FMath::Sin(Frequency * ChaosTime * 8.0f);
				NewLocation.X *= EpicPulse;
				NewLocation.Y *= EpicPulse;
			}
		}
		break;

	case EArcadeMovement::SwarmEntry:
		{
			if (SwarmPhase == 0)
			{
				NewLocation.X -= SwarmEntrySpeed * t;
				NewLocation.Y += FMath::Sin(t * 3.0f) * 50.0f; 
				
				if (SwarmTargetPosition != FVector::ZeroVector)
				{
					float DistanceToTarget = FVector::Dist(NewLocation, SwarmTargetPosition);
					if (DistanceToTarget < 200.0f)
					{
						SwarmPhase = 1;
						Origin = NewLocation; 
						Elapsed = 0.0f;
					}
				}
			}
			// Fase 1: Movimiento hacia posición de formación
			else if (SwarmPhase == 1)
			{
				if (SwarmTargetPosition != FVector::ZeroVector)
				{
					NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), SwarmTargetPosition, DeltaTime, SwarmFormationSpeed / 100.0f);
					
					// Cambiar a fase de avance cuando esté en posición
					float DistanceToTarget = FVector::Dist(NewLocation, SwarmTargetPosition);
					if (DistanceToTarget < 50.0f)
					{
						SwarmPhase = 2;
						Elapsed = 0.0f;
					}
				}
			}
			// Fase 2: Avance coordinado hacia el jugador
			else if (SwarmPhase == 2)
			{
				NewLocation.X -= SwarmAdvanceSpeed * t;
				NewLocation.Y += FMath::Sin(t * 2.0f) * 30.0f; // Movimiento suave en Y
			}
		}
		break;

	case EArcadeMovement::SwarmFormation:
		{
			// Mantener posición de formación con ligero movimiento
			if (SwarmTargetPosition != FVector::ZeroVector)
			{
				FVector TargetWithMovement = SwarmTargetPosition;
				TargetWithMovement.X -= t * 30.0f; // Avance muy lento
				TargetWithMovement.Y += FMath::Sin(t * 1.5f) * 20.0f; // Ondulación suave
				
				NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), TargetWithMovement, DeltaTime, 2.0f);
			}
		}
		break;

	case EArcadeMovement::SwarmAdvance:
		{
			// Avance agresivo manteniendo formación
			NewLocation.X -= SwarmAdvanceSpeed * t;
			NewLocation.Y += FMath::Sin(t * 2.5f + Origin.Y * 0.01f) * 40.0f; // Cada nave con ligera variación
		}
		break;

	default:
		NewLocation.X -= Speed * t;
		break;
	}

	// Aplicar límites de movimiento
	NewLocation.X = FMath::Clamp(NewLocation.X, MovementMin.X, MovementMax.X);
	NewLocation.Y = FMath::Clamp(NewLocation.Y, MovementMin.Y, MovementMax.Y);

	GetOwner()->SetActorLocation(NewLocation);
}

void UMoveComponent::SetSwarmTargetPosition(FVector NewTarget)
{
	SwarmTargetPosition = NewTarget;
}

void UMoveComponent::SetSwarmPhase(int32 NewPhase)
{
	SwarmPhase = NewPhase;
	Elapsed = 0.0f; // Reiniciar tiempo para nueva fase
}
