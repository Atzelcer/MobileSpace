#pragma once

#include "CoreMinimal.h"
#include "Ship_X.h"
#include "Ship_CazadorAlfa.generated.h"

UCLASS()
class MOBILESPACE_API AShip_CazadorAlfa : public AShip_X
{
	GENERATED_BODY()

public:
	AShip_CazadorAlfa();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
