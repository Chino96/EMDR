// Fill out your copyright notice in the Description page of Project Settings.
#include "EyeOrb.h"
#include <Components/SphereComponent.h>

// Sets default values
AEyeOrb::AEyeOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("StaticMesh'/Game/Mesh/EMDR_Sphere.EMDR_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/Sphere_Inst.Sphere_Inst'"));

	UStaticMeshComponent* meshPtr = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	meshPtr->SetWorldScale3D(FVector(.045, .045, .045));
	meshPtr->SetupAttachment(RootComponent);
	
	if (SphereAsset.Succeeded() && MaterialAsset.Succeeded())
	{
		meshPtr->SetStaticMesh(SphereAsset.Object);
		meshPtr->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		meshPtr->SetMaterial(0, MaterialAsset.Object);
		meshPtr->SetVisibility(true);
		meshPtr->SetHiddenInGame(false);
	}
}

// Called when the game starts or when spawned
void AEyeOrb::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEyeOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

