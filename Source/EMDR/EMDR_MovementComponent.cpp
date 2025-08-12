// Fill out your copyright notice in the Description page of Project Settings.


#include "EMDR_MovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "EyeOrb.h"

// Sets default values for this component's properties
UEMDR_MovementComponent::UEMDR_MovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Parent = GetOwner();
	// ...
}

// Called when the game starts
void UEMDR_MovementComponent::BeginPlay()
{
	Super::BeginPlay();
	TFP_Horizontal_Movement.BindUFunction(this, FName("TP_Horizontal_Movement"));
	TFP_Vertical_Movement.BindUFunction(this, FName("TP_Vertical_Movement"));
	TFP_Random_Movement.BindUFunction(this, FName("TP_Random_Movement"));
	TickHorizontalMovement = false;
	TickVerticalMovement = false;
	FlipHorizontal = false;
	FlipVertical = false;
	LocationsGenerated = false;
	LastRandomLocationRequest = -1;
	RandomLocationRequest = 0;
	color_select = 0;
	alpha = -50;
	colors = TArray<FVector>();
	colors.Add(FVector(1, 0, 0));
	colors.Add(FVector(0, 0, 1));
	colors.Add(FVector(0, 1, 0));
	colors.Add(FVector(1, 0, 1));
	colors.Add(FVector(1, 1, 0));
	colors.Add(FVector(2, 1, 0));
	Glow = false;
	// ...
	
}

// Called every frame
void UEMDR_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickVerticalMovement)
	{
		VerticalMovementTimeline->TickTimeline(DeltaTime);
		if (!VerticalMovementTimeline->IsPlaying())
		{
			if (FlipVertical)
			{
				FlipVertical = false;
				color_select = FMath::FRandRange(0, 6);
			}
			else 
			{
				FlipVertical = true;
				color_select = FMath::FRandRange(0, 6);
			}
			StartVerticalOrbMovement(VerticalCurve);
		}
	}
	if (TickHorizontalMovement)
	{
		HorizontalMovementTimeline->TickTimeline(DeltaTime);
		if (!HorizontalMovementTimeline->IsPlaying())
		{
			if (FlipHorizontal)
			{
				FlipHorizontal = false;
				color_select = FMath::FRandRange(0, 6);
			}
			else
			{
				FlipHorizontal = true;
				color_select = FMath::FRandRange(0, 6);
			}
			StartHorizontalOrbMovement(HorizontalCurve);
		}
	}
	if (TickRandomMovement)
	{
		RandomMovementTimeline->TickTimeline(DeltaTime);
		if (!RandomMovementTimeline->IsPlaying())
		{

			FlipHorizontal = false;
			color_select = FMath::FRandRange(0, 6);
			RandomLocationRequest++;
			if (RandomLocationRequest >= Locations.Num())
			{
				RandomLocationRequest = 0;
			}
			UE_LOG(LogTemp, Warning, TEXT("Going to Location: %s"), *Locations[RandomLocationRequest].ToString());
			StartRandomMovement(RandomCurve);
		}
	}
}

void UEMDR_MovementComponent::StartVerticalOrbMovement(UCurveFloat* CurveVertical)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEyeOrb::StaticClass(), FoundActors);
	EyeOrb = Cast<AEyeOrb>(FoundActors[0]);
	UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
	UMaterialInterface* matInt = sphere->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(matInt->GetMaterial(), EyeOrb);

	FoundActors.Empty();
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EMDRMenu"), FoundActors);
    AActor* EMDRMenu = FoundActors[0];

	EMDRMenu->SetActorHiddenInGame(true);

	VerticalCurve = CurveVertical;

	float z = 200;

	if (FlipVertical)
	{
		z *= -1;
	}
	VerticalLocationRequest = FVector(EyeOrb->GetActorLocation().X, EyeOrb->GetActorLocation().Y, z);

	VerticalMovementTimeline = new FTimeline();
	StartLocation = EyeOrb->GetActorLocation();

	VerticalMovementTimeline->AddInterpFloat(CurveVertical, TFP_Vertical_Movement, FName("Alpha"));
	VerticalMovementTimeline->SetLooping(false);
	VerticalMovementTimeline->PlayFromStart();

	TickVerticalMovement = true;
}

int UEMDR_MovementComponent::FindVectorQuadrant(FVector vector)
{
	//using an if else statement find the quadrant of the vector
	if(vector.Y > 0 && vector.Z > 0)
	{
		return 1;
	}
	else if(vector.Y < 0 && vector.Z > 0)
	{
		return 2;
	}
	else if(vector.Y < 0 && vector.Z < 0)
	{
		return 3;
	}
	else if(vector.Y > 0 && vector.Z < 0)
	{
		return 4;
	}
	else
	{
		return -1;
	}
}

void UEMDR_MovementComponent::StartRandomMovement(UCurveFloat* Curve)
{
	//Find the eye orb in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEyeOrb::StaticClass(), FoundActors);
	EyeOrb = Cast<AEyeOrb>(FoundActors[0]);
	
	//Get the static mesh component of the eye orb
	UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
	
	//Set up proper collision for raycast detection
	sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);


	//Get the material of the static mesh component
	UMaterialInterface* matInt = sphere->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(matInt->GetMaterial(), EyeOrb);

	//Clear the found actors array
	FoundActors.Empty();

	//Find the EMDR menu in the world
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EMDRMenu"), FoundActors);
    AActor* EMDRMenu = FoundActors[0];

	//Hide the EMDR menu
	EMDRMenu->SetActorHiddenInGame(true);
	AEMDRMenu* EMDRMenuActor = Cast<AEMDRMenu>(EMDRMenu);

	//Create a new TArray of 7 Vectors
	if(!LocationsGenerated)
	{
		//Set the boolean to show that the session has started
		EMDRMenuActor->SessionStarted = true;
		//Create an empty TArray of Vectors
		Locations = TArray<FVector>();

		//Add 8 random locations to the TArray
		for (int i = 0; i < 8; i++)
		{
			//Init the empty Y, and Z coordinates
			float y;
			float z;
			
			//Generate a Random Boolean
			bool negative = FMath::RandBool();
			
			//create a TArray of 4 random numbers
			TArray<int> RandomCoordinates;
			
			//Generate 2 random numbers between -150 and -75
			y = FMath::FRandRange(-15, -35);
			z = FMath::FRandRange(-15, -35);
			RandomCoordinates.Add(y);
			RandomCoordinates.Add(z);

			//Generate 2 random numbers between 75 and 150
			y = FMath::FRandRange(15, 35);
			z = FMath::FRandRange(15, 35);
			RandomCoordinates.Add(y);
			RandomCoordinates.Add(z);
			int k = 0;
			int j = 0;
			while(j == k)
			{
				j = FMath::RandRange(0, 3);
				k = FMath::RandRange(0, 3);
				if (j != k)
				{
					y = RandomCoordinates[j];
					z = RandomCoordinates[k];
					break;
				}
			}
			Locations.Add(FVector(EyeOrb->GetActorLocation().X, y, z));
		}

		//Create an array to hold the reordered locations
		TArray<FVector> ReorderedLocations;
		ReorderedLocations.Add(Locations[0]);

		//Iterate through the original locations and reorder them
		for (int i = 1; i < Locations.Num(); i++)
		{
			int CurrentQuadrant = FindVectorQuadrant(ReorderedLocations.Last());
			for (int j = 0; j < Locations.Num(); j++)
			{
				int NextQuadrant = FindVectorQuadrant(Locations[j]);
				if (NextQuadrant != CurrentQuadrant && !ReorderedLocations.Contains(Locations[j]))
				{
					ReorderedLocations.Add(Locations[j]);
					break;
				}
			}
		}

		//Replace the original locations with the reordered locations
		Locations = ReorderedLocations;
		// Using a for loop print all the locations
		for(int i = 0; i < Locations.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("##########: Location Quadrant %d: Vector: %s"), FindVectorQuadrant(Locations[i]), *Locations[i].ToString());
		}

		LocationsGenerated = true;
		RandomLocationRequest = 0;
	}

	RandomCurve = Curve;
	//Create a new timeline
	RandomMovementTimeline = new FTimeline();
	//Set the start location to the current location of the EyeOrb
	StartLocation = EyeOrb->GetActorLocation();
	//Add the curve to the timeline
	RandomMovementTimeline->AddInterpFloat(Curve, TFP_Random_Movement, FName("Alpha"));
	//Set the timeline to not loop
	RandomMovementTimeline->SetLooping(false);
	//Play the timeline from the start
	RandomMovementTimeline->PlayFromStart();
	//Set the TickRandomMovement to true
	TickRandomMovement = true;
}

FVector UEMDR_MovementComponent::FindFurthestVector(TArray<FVector> vectors)
{
	//using a for loop find the FVector with the greatest Distance from the Center
	FVector Max = FVector(150,0,0);
	int MaxLocation = 0;
	for(int i = 0; i < vectors.Num(); i++)
	{
		if(FMath::Abs(FVector::Dist(Max, vectors[i])) > FMath::Abs(FVector::Dist(Max, FVector(150, 0, 0))))
		{
			Max = vectors[i];
			MaxLocation = i;
		}
	}
	return Max;
}

void UEMDR_MovementComponent::StartHorizontalOrbMovement(UCurveFloat* curve)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEyeOrb::StaticClass(), FoundActors);
	EyeOrb = Cast<AEyeOrb>(FoundActors[0]);
	UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
	UMaterialInterface* matInt = sphere->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(matInt->GetMaterial(), EyeOrb);

	FoundActors.Empty();
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EMDRMenu"), FoundActors);
    AActor* EMDRMenu = FoundActors[0];

	EMDRMenu->SetActorHiddenInGame(true);

	HorizontalCurve = curve;

	float y = 100;
	
	if (FlipHorizontal) 
	{
		y *= -1;
	}
	
	HorizontalLocationRequest = FVector(EyeOrb->GetActorLocation().X, y, EyeOrb->GetActorLocation().Z);

	HorizontalMovementTimeline = new FTimeline();
	StartLocation = EyeOrb->GetActorLocation();

	HorizontalMovementTimeline->AddInterpFloat(curve, TFP_Horizontal_Movement, FName("Alpha"));
	HorizontalMovementTimeline->SetLooping(false);
	HorizontalMovementTimeline->PlayFromStart();

	TickHorizontalMovement = true;
}

void UEMDR_MovementComponent::TP_Horizontal_Movement(float val)
{
		FVector NewHorizontalLocation = FMath::Lerp(StartLocation, HorizontalLocationRequest, val);
		
		UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
		FVector color = colors[color_select];

		DynamicMaterial->SetVectorParameterValue(TEXT("Color_0"), color);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color_1"), FVector(0, 0, 0));
		DynamicMaterial->SetScalarParameterValue(TEXT("Alpha"), -50);
		if(Glow)
		{
			DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 1.f);
		}
		else
		{
			DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 0.05f);
		}
		sphere->SetMaterial(0, DynamicMaterial);
		EyeOrb->SetActorLocation(FVector(EyeOrb->GetActorLocation().X, NewHorizontalLocation.Y, EyeOrb->GetActorLocation().Z));
}

void UEMDR_MovementComponent::TP_Vertical_Movement(float val)
{
		FVector NewVerticalLocation = FMath::Lerp(StartLocation, VerticalLocationRequest, val);

		UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
		FVector color = colors[color_select];

		DynamicMaterial->SetVectorParameterValue(TEXT("Color_0"), color);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color_1"), FVector(0, 0, 0));
		DynamicMaterial->SetScalarParameterValue(TEXT("Alpha"), -50);
		if(Glow)
		{
			DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 1.f);
		}
		else
		{
			DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 0.05f);
		}
		sphere->SetMaterial(0, DynamicMaterial);
		EyeOrb->SetActorLocation(FVector(EyeOrb->GetActorLocation().X, EyeOrb->GetActorLocation().Y, NewVerticalLocation.Z));
}

void UEMDR_MovementComponent::TP_Random_Movement(float val)
{
	FVector NewLocation = FMath::Lerp(StartLocation, Locations[RandomLocationRequest], val);
	UStaticMeshComponent* sphere = Cast<AActor>(EyeOrb)->FindComponentByClass<UStaticMeshComponent>();
	sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	sphere->SetCollisionResponseToAllChannels(ECR_Block);
	sphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	FVector color = colors[color_select];

	DynamicMaterial->SetVectorParameterValue(TEXT("Color_0"), color);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color_1"), FVector(0, 0, 0));
	DynamicMaterial->SetScalarParameterValue(TEXT("Alpha"), -50);
	if(Glow)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 1.f);
	}
	else
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), 0.05f);
	}
	sphere->SetMaterial(0, DynamicMaterial);
	EyeOrb->SetActorLocation(NewLocation);
}
