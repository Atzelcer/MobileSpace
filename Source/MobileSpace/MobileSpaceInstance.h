#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MobileSpaceInstance.generated.h"

UCLASS()
class MOBILESPACE_API UMobileSpaceInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void HostSala();

	UFUNCTION(BlueprintCallable)
	void UnirseSala(const FString& CodigoIngresado);

	FString ObtenerIPLocal() const;
	bool VerificarConexionLAN(const FString& HostIP) const;

	UPROPERTY()
	class AMultiplayerManager* MultiplayerManager;
};
