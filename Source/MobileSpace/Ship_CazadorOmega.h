#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorOmega.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorOmega : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorOmega();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
