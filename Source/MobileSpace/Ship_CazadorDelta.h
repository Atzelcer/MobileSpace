#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorDelta.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorDelta : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorDelta();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
