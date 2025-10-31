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

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	// Camera components removed - using fixed camera from GameMode
	// /** The camera */
	// UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// class UCameraComponent* CameraComponent;

	// /** Camera boom positioning the camera above the character */
	// UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// class USpringArmComponent* CameraBoom;

public:
	AMobileSpacePawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleParticleAsset;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	// Camera getters removed - using fixed camera from GameMode
	// /** Returns CameraComponent subobject **/
	// FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	// /** Returns CameraBoom subobject **/
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<AMobileSpaceProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Movement|Limits")
	FVector2D MovementMin = FVector2D(-1400.f, -3000.f);

	UPROPERTY(EditAnywhere, Category = "Movement|Limits")
	FVector2D MovementMax = FVector2D(1400.f, 3000.f);
};

