// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "EyeTrackerFunctionLibrary.h"
#include "LocationSolver.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMDR_API ULocationSolver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULocationSolver();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* ParentActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "!LeftHand && !RightHand", EditConditionHides))
	bool Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "!Head && !RightHand", EditConditionHides))
	bool LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "!Head && !LeftHand", EditConditionHides))
	bool RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "Head || LeftHand || RightHand", EditConditionHides))
	bool AlwaysFacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "Head || LeftHand || RightHand", EditConditionHides))
	float XOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta=(EditCondition = "Head || LeftHand || RightHand", EditConditionHides))
	bool Orbit;

	UFUNCTION()
	void TP_Rotation(float val);

	UFUNCTION()
	void TP_Location(float val);

	FOnTimelineFloat TFP_Location{};
	FOnTimelineFloat TFP_Rotation{};

	UFUNCTION()
	void SetActorRotation(FRotator TargetRotation);

	UFUNCTION()
	void SetActorLocation(FVector TargetLocation);

	FTimeline* RotationMovementTimeline;
	bool TickRotationMovement;

	FTimeline* LocationMovementTimeline;
	bool TickLocationMovement;

	FTransform CurrentTransform;

	FVector StartLocation;
	FVector RotationDimension;
	FRotator RotationTarget;
	FVector LocationTarget;
	bool CounterClockwise;
	float StartDegree;
	float EndDegree;
};
