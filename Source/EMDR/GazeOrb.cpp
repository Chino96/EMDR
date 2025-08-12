// Fill out your copyright notice in the Description page of Project Settings.


#include "GazeOrb.h"
#include "EyeOrb.h"
#include <Kismet/GameplayStatics.h>
#include "EMDR_MovementComponent.h"
#include <Components/SphereComponent.h>

// Sets default values
AGazeOrb::AGazeOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/Gaze_Sphere_Inst.Gaze_Sphere_Inst'"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	RootComponent = SphereCollision; // Set as root component
	SphereCollision->SetSphereRadius(10);
	SphereCollision->SetVisibility(false);
	SphereCollision->bHiddenInGame = false;
	
	// Set up collision for both overlap and raycast detection
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	if (MaterialAsset.Succeeded())
	{
		SphereCollision->SetMaterial(0, MaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void AGazeOrb::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision = this->FindComponentByClass<USphereComponent>();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGazeOrb::OnOverlapActivateSphere);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AGazeOrb::OnOverlapEndDeactivateSphere);
}

void AGazeOrb::OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Log a message indicating the overlap event
	UE_LOG(LogTemp, Warning, TEXT("##########: Overlap GazeOrb"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMDRMenu::StaticClass(), FoundActors);
	EMDRMenu = Cast<AEMDRMenu>(FoundActors[0]);
	TArray<UActorComponent*> ChildActors = EMDRMenu->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Start"));
	for(int i = 0; i < ChildActors.Num(); i++)
	{
		UEMDR_MovementComponent* movementComponent = Cast<UEMDR_MovementComponent>(Cast<UChildActorComponent>(ChildActors[i])->GetChildActor()->GetComponentByClass(UEMDR_MovementComponent::StaticClass()));
		movementComponent->Glow = true;
	}
}

void AGazeOrb::OnOverlapEndDeactivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Log a message indicating the overlap event
	UE_LOG(LogTemp, Warning, TEXT("##########: Overlap GazeOrb"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMDRMenu::StaticClass(), FoundActors);
	EMDRMenu = Cast<AEMDRMenu>(FoundActors[0]);
	TArray<UActorComponent*> ChildActors = EMDRMenu->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Start"));
	for(int i = 0; i < ChildActors.Num(); i++)
	{
		UEMDR_MovementComponent* movementComponent = Cast<UEMDR_MovementComponent>(Cast<UChildActorComponent>(ChildActors[i])->GetChildActor()->GetComponentByClass(UEMDR_MovementComponent::StaticClass()));
		movementComponent->Glow = false;
	}
}

// Called every frame
void AGazeOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
