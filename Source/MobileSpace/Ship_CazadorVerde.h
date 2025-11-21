#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorVerde.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorVerde : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorVerde();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
