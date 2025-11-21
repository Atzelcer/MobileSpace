#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorAzul.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorAzul : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorAzul();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
