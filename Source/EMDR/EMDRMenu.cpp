// Fill out your copyright notice in the Description page of Project Settings.


#include "EMDRMenu.h"
#include <Kismet/GameplayStatics.h>
#include "EyeOrb.h"

// Sets default values
AEMDRMenu::AEMDRMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEMDRMenu::BeginPlay()
{
	Super::BeginPlay();
	StartButtons = this->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Start"));
	//Get the location solver component by the class type
	LocationSolver = Cast<ULocationSolver>(GetComponentByClass(ULocationSolver::StaticClass()));
}

// Called every frame
void AEMDRMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (RadialMovement)
	{
		Frames++;
		if (Frames == 5)
		{
			Frames = 0;
			setOrbVisible();
		}
	}
}

void AEMDRMenu::DestroyOrbs() 
{
	TArray<AActor*> orbs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEyeOrb::StaticClass(), orbs);
	for (AActor* actor : orbs)
	{
		actor->Destroy();
	}
}

void AEMDRMenu::SpawnOrbs(FString type) 
{
	DestroyOrbs();
	UChildActorComponent* DisplayedStartButton;
	if (type.Equals("Radial")) 
	{
		for (UActorComponent* startButton : StartButtons)
		{
			if (startButton->ComponentTags.Contains(FName("Radial")))
			{
				DisplayedStartButton = Cast<UChildActorComponent>(startButton);
				DisplayedStartButton->bHiddenInGame = false;
				DisplayedStartButton->SetVisibility(true);
			}
		}
		SpawnOrbsRadial();
	}
	else if (type.Contains("Linear"))
	{
		for (UActorComponent* startButton : StartButtons)
		{
			if (startButton->ComponentTags.Contains(FName("Linear")))
			{
				DisplayedStartButton = Cast<UChildActorComponent>(startButton);
				DisplayedStartButton->bHiddenInGame = false;
				DisplayedStartButton->SetVisibility(true);
			}
		}
		SpawnOrbsSingular();
	}
	else if (type.Contains("Random")) 
	{
		for (UActorComponent* startButton : StartButtons)
		{
			if (startButton->ComponentTags.Contains(FName("Random")))
			{
				DisplayedStartButton = Cast<UChildActorComponent>(startButton);
				DisplayedStartButton->bHiddenInGame = false;
				DisplayedStartButton->SetVisibility(true);
			}
		}
		SpawnOrbsSingular();
	}
}

void AEMDRMenu::SpawnOrbsSingular()
{
	FVector Center = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	FVector OrbLocation = Center; // Spawn at the same location as the menu
	AActor* Orb = GetWorld()->SpawnActor<AActor>(AEyeOrb::StaticClass(), OrbLocation, FRotator::ZeroRotator);

	Orb->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	//Set the orb location to the EMDR menu location
	Orb->SetActorLocation(OrbLocation);
}

void AEMDRMenu::RadialOrbMovement()
{
	RadialMovement = true;
}

void AEMDRMenu::SpawnOrbsRadial()
{
	DestroyOrbs();
	int NumOrbs = 12; // Change the number of orbs to 12
	float Radius = 200.0f;
	FVector Center = GetActorLocation();

	for (int i = NumOrbs; i > 0; i--)
	{
		float Angle = i * (360.0f / NumOrbs);
		float X = Center.Z + 500;
		float Y = Center.X + Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
		float Z = Center.Y + Radius * FMath::Sin(FMath::DegreesToRadians(Angle));

		FVector OrbLocation = FVector(X, Y, Z);
		AActor* Orb = GetWorld()->SpawnActor<AActor>(AEyeOrb::StaticClass(), OrbLocation, FRotator::ZeroRotator);
		UStaticMeshComponent* mesh = Orb->FindComponentByClass<UStaticMeshComponent>();
		mesh->SetWorldScale3D(FVector(.5, .5, .5));
		RadialOrbs.Add(Orb);
	}

	// Spawn an orb at the center of the circle
	//FVector CenterOrbLocation = FVector(Center.Z + 500, Center.X, Center.Y);
	//AActor* CenterOrb = GetWorld()->SpawnActor<AActor>(AEyeOrb::StaticClass(), CenterOrbLocation, FRotator::ZeroRotator);
	//RadialOrbs.Add(CenterOrb);
}

void AEMDRMenu::setOrbVisible()
{
	if (!Hidden)
	{
		Hidden = true;
		RadialOrbs[OrbPos]->SetActorHiddenInGame(Hidden);
	}
	else 
	{
		Hidden = false;
		RadialOrbs[OrbPos]->SetActorHiddenInGame(Hidden);
		OrbPos++;
	}

	if (OrbPos >= 12) 
		OrbPos = 0;
}


