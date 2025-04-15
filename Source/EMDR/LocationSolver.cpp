// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationSolver.h"

// Sets default values for this component's properties
ULocationSolver::ULocationSolver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULocationSolver::BeginPlay()
{
	Super::BeginPlay();
	TFP_Rotation.BindUFunction(this, FName("TP_Rotation"));
	TFP_Location.BindUFunction(this, FName("TP_Location"));
	ParentActor = GetOwner();
	// ...
	
}


// Called every frame
void ULocationSolver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Check if the timeline is valid and if it should be ticked
	if (RotationMovementTimeline && TickRotationMovement)
	{
		RotationMovementTimeline->TickTimeline(DeltaTime);
	}
	if (LocationMovementTimeline && TickLocationMovement)
	{
		LocationMovementTimeline->TickTimeline(DeltaTime);
	}
	// ...
}

void ULocationSolver::SetActorRotation(FRotator Target)
{
	RotationMovementTimeline = new FTimeline();
	//Get the parent Transform
	CurrentTransform = ParentActor->GetTransform();
	UCurveFloat* Curve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), TEXT("RotationCurve"));

	Curve->FloatCurve.AddKey(0.0f, 0.0f);
	Curve->FloatCurve.AddKey(1.0f, 1.5f);

	RotationTarget = Target;
	TickRotationMovement = true;

	RotationMovementTimeline->AddInterpFloat(Curve, TFP_Rotation, FName("Alpha"));
	RotationMovementTimeline->SetLooping(false);
	RotationMovementTimeline->PlayFromStart();
}

void ULocationSolver::SetActorLocation(FVector Location)
{
	LocationMovementTimeline = new FTimeline();
	//Get the parent Transform
	CurrentTransform = ParentActor->GetTransform();
	UCurveFloat* Curve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), TEXT("LocationCurve"));
	Curve->FloatCurve.AddKey(0.0f, 0.0f);
	Curve->FloatCurve.AddKey(1.0f, 1.5f);

	LocationTarget = Location;
	TickLocationMovement = true;
	LocationMovementTimeline->AddInterpFloat(Curve, TFP_Location, FName("Alpha"));
	LocationMovementTimeline->SetLooping(false);
	LocationMovementTimeline->PlayFromStart();
}

void ULocationSolver::TP_Rotation(float val)
{
	//Set the Rotation of the Actor to the Target Rotation Exluding the Roll
	FRotator NewRotation = FMath::Lerp(CurrentTransform.GetRotation().Rotator(), RotationTarget, val);
	NewRotation.Roll = 0.0f; // Set Roll to 0
	ParentActor->SetActorRotation(NewRotation);
}


void ULocationSolver::TP_Location(float val)
{
	//Set the Location of the Actor to the Target Location
	FVector NewLocation = FMath::Lerp(CurrentTransform.GetLocation(), LocationTarget, val);
	ParentActor->SetActorLocation(NewLocation);
}