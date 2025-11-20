#include "MegaPlataforma.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AMegaPlataforma::AMegaPlataforma()
{
	PrimaryActorTick.bCanEverTick = true;

	PlataformaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlataformaMesh"));
	SetRootComponent(PlataformaMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Importaciones/Campo_Juego/Scenery.Scenery'"));
	if (MeshObj.Succeeded())
	{
		PlataformaMesh->SetStaticMesh(MeshObj.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat0(TEXT("MaterialInstanceConstant'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/MI_VFX_Lush_Galaxy_Shader_03.MI_VFX_Lush_Galaxy_Shader_03'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat1(TEXT("MaterialInstanceConstant'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/MI_VFX_Lush_Galaxy_Shader_01.MI_VFX_Lush_Galaxy_Shader_01'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat2(TEXT("Material'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/M_VFX_Lush_Galaxy_Shader.M_VFX_Lush_Galaxy_Shader'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat3(TEXT("MaterialInstanceConstant'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/MI_VFX_Lush_Galaxy_Shader_01_Inverted.MI_VFX_Lush_Galaxy_Shader_01_Inverted'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat4(TEXT("MaterialInstanceConstant'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/MI_VFX_Lush_Galaxy_Shader_02.MI_VFX_Lush_Galaxy_Shader_02'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat5(TEXT("MaterialInstanceConstant'/Game/Vefects/Stylized_Galaxy_Shader/Galaxy/Materials/MI_VFX_Lush_Galaxy_Shader_02_Inverted.MI_VFX_Lush_Galaxy_Shader_02_Inverted'"));

	if (Mat0.Succeeded()) Materiales.Add(Mat0.Object);
	if (Mat1.Succeeded()) Materiales.Add(Mat1.Object);
	if (Mat2.Succeeded()) Materiales.Add(Mat2.Object);
	if (Mat3.Succeeded()) Materiales.Add(Mat3.Object);
	if (Mat4.Succeeded()) Materiales.Add(Mat4.Object);
	if (Mat5.Succeeded()) Materiales.Add(Mat5.Object);

	SetActorLocation(FVector(-116.168701f, 6.159666f, -940.0f));
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
	SetActorScale3D(FVector(8.0f, 9.75f, 3.0f));
}

void AMegaPlataforma::BeginPlay()
{
	Super::BeginPlay();
}

void AMegaPlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMegaPlataforma::CambiarMaterialAleatorio()
{
	if (Materiales.Num() == 0) return;

	int32 Index = FMath::RandRange(0, Materiales.Num() - 1);
	PlataformaMesh->SetMaterial(0, Materiales[Index]);
}

void AMegaPlataforma::CambiarMaterialPorIndice(int32 Index)
{
	if (!Materiales.IsValidIndex(Index)) return;

	PlataformaMesh->SetMaterial(0, Materiales[Index]);
}
