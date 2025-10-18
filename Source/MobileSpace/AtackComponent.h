// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtackComponent.generated.h"
UENUM(BlueprintType)
enum class EAtackPattern : uint8
{
	Single      UMETA(DisplayName = "Single Shot"),
	Spread      UMETA(DisplayName = "Spread"),
	Burst       UMETA(DisplayName = "Burst"),
	// Puedes agregar más patrones si quieres
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBILESPACE_API UAtackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAtackComponent();

	UFUNCTION(BlueprintCallable)
	void Fire(EAtackPattern Pattern);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	USoundBase* FireSound;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	bool bCanFire;
	FTimerHandle FireTimerHandle;

	void OnFireTimerExpired();

	void FireSingle();
	void FireSpread();
	void FireBurst();

	// Para alternar proyectiles en single
	int32 ProjectileCycle = 0;
		
};
