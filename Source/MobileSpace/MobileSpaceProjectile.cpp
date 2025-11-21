// Copyright Epic Games, Inc. All Rights Reserve

#include "MobileSpaceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ship_X.h"
#include "MegaObstaculo.h"

AMobileSpaceProjectile::AMobileSpaceProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	RootComponent = ProjectileMesh;

	// Evita colisión entre proyectiles
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel2);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMobileSpaceProjectile::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bSweepCollision = true;



	ParticleProjectile = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleProjectile->SetupAttachment(RootComponent);

	WeaponType = EWeaponType::Arma1;

	EvolutionLevel = 1;
	FireSound = nullptr;
	ExplosionParticle = nullptr;
	InitialLifeSpan = 3.0f;
}

void AMobileSpaceProjectile::ApplyWeaponConfig(EWeaponType NewType)
{
	WeaponType = NewType;

	switch (NewType)
	{
	case EWeaponType::Arma1:
		ParticleProjectile->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Slash01_Blue.P_Projectile_Slash01_Blue")));
		ParticleProjectile->SetRelativeScale3D(FVector(0.70f));
		ParticleProjectile->SetRelativeRotation(FRotator(0.f, 2.f, 0.f));
		FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MagicCombatAudio/ETC/RoundHitIce.RoundHitIce"));
		ExplosionParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Hits/P_Hit_Slash01_Blue.P_Hit_Slash01_Blue"));
		ExplosionScale = 0.5f;
		break;

	case EWeaponType::Arma2:
		ParticleProjectile->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/VFXSeries1/Particles/Energy/P_ChargedPlasma_2.P_ChargedPlasma_2")));
		ParticleProjectile->SetRelativeScale3D(FVector(3.0f));
		ParticleProjectile->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MagicCombatAudio/ETC/RoundHitPoison.RoundHitPoison"));
		ExplosionParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/TurretVFX/Sources/Particle/Hit/P_Poison_Hit.P_Poison_Hit"));
		ExplosionScale = 0.6f;
		break;

	case EWeaponType::Arma3:
		ParticleProjectile->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Orb01_Purple.P_Projectile_Orb01_Purple")));
		ParticleProjectile->SetRelativeScale3D(FVector(1.8f));
		ParticleProjectile->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MissleSetAudio/Soul/SoulMissile_CrimsonExplosion.SoulMissile_CrimsonExplosion"));
		ExplosionParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Hits/P_Hit_Orb01_Purple.P_Hit_Orb01_Purple"));
		ExplosionScale = 0.8f;
		break;

	case EWeaponType::Arma4:
		ParticleProjectile->SetTemplate(LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_Orb05_Orange.P_Projectile_Orb05_Orange")));
		ParticleProjectile->SetRelativeScale3D(FVector(1.9f));
		ParticleProjectile->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		FireSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Musica_D/EpicToonSFX/MissleSetAudio/Mystic/MysticMissle_PoisonShoot.MysticMissle_PoisonShoot"));
		ExplosionParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/MagicProjectilesVol2/Particles/Hits/P_Hit_Fireball04_Orange.P_Hit_Fireball04_Orange"));
		ExplosionScale = 0.9f;
		break;
	}
}


void AMobileSpaceProjectile::SetWeaponAndEvolution(EWeaponType NewType, int32 NewLevel)
{
	EvolutionLevel = FMath::Clamp(NewLevel, 1, 5);
	ApplyWeaponConfig(NewType);

	FVector BaseScale = ParticleProjectile->GetRelativeScale3D();


	float ScaleMultiplier = 1.0f + (EvolutionLevel - 1) * 0.2f;
	float BaseSpeed = 5000.f;
	float SpeedIncrement = 200.f;

	float SpeedFactor = BaseSpeed + (EvolutionLevel - 1) * SpeedIncrement;

	ParticleProjectile->SetRelativeScale3D(BaseScale * ScaleMultiplier);

	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = SpeedFactor;
		ProjectileMovement->MaxSpeed = SpeedFactor;
	}
}





void AMobileSpaceProjectile::PlayFireSound()
{
	if (FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

void AMobileSpaceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner() || OtherActor == this) return;

	if (OtherComp && OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());

	if (OtherActor->IsA(AShip_X::StaticClass()))
		Cast<AShip_X>(OtherActor)->HandleDestruction();

	if (OtherActor->IsA(AMegaObstaculo::StaticClass()))
		Cast<AMegaObstaculo>(OtherActor)->DestruirObstaculo();

	if (ExplosionParticle)
	{
		FVector ExplosionScaleVec(ExplosionScale);
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			FRotator::ZeroRotator,
			ExplosionScaleVec
		);
	}

	Destroy();
}

