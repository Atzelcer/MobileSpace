#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorRojo.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorRojo : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorRojo();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
