// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MobileSpaceProjectile.h"  
#include "MobileSpacePawn.generated.h"

UCLASS(Blueprintable)
class AMobileSpacePawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	
public:
	AMobileSpacePawn();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;



	

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void FireShot(FVector FireDirection);

	void ShotTimerExpired();


	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:

	uint32 bCanFire : 1;

	FTimerHandle TimerHandle_ShotTimerExpired;

public:

	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	

	UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<AMobileSpaceProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Movement|Limits")
	FVector2D MovementMin = FVector2D(-1400.f, -3000.f);

	UPROPERTY(EditAnywhere, Category = "Movement|Limits")
	FVector2D MovementMax = FVector2D(1400.f, 3000.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 NumLifes = 3; // Vidas
};

