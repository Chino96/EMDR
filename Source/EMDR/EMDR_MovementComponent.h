// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "EMDRMenu.h"
#include "EMDR_MovementComponent.generated.h"

class UCurveFloat;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMDR_API UEMDR_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEMDR_MovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FOnTimelineFloat TFP_Horizontal_Movement{};
	FOnTimelineFloat TFP_Vertical_Movement{};
	FOnTimelineFloat TFP_Random_Movement{};
	UFUNCTION()
	void TP_Vertical_Movement(float val);
	UFUNCTION()
	void TP_Horizontal_Movement(float val);
	UFUNCTION()
	void TP_Random_Movement(float val);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartHorizontalOrbMovement(UCurveFloat* curve);

	UFUNCTION(BlueprintCallable)
	virtual void StartVerticalOrbMovement( UCurveFloat* CurveVertical);

	UFUNCTION(BlueprintCallable)
	virtual void StartRandomMovement(UCurveFloat* Curve);

	FVector FindFurthestVector(TArray<FVector> vectors);
	int FindVectorQuadrant(FVector vector);


	UMaterialInstanceDynamic* DynamicMaterial;
	AActor* Parent;
	bool TickHorizontalMovement;
	bool TickVerticalMovement;
	bool TickRandomMovement;
	bool Glow;
	FTimeline* MovementTimeline;
	FTimeline* HorizontalMovementTimeline;
	FTimeline* VerticalMovementTimeline;
	FTimeline* RandomMovementTimeline;
	FVector StartLocation;
	FVector VerticalLocationRequest;
	FVector HorizontalLocationRequest;
	int RandomLocationRequest;
	int LastRandomLocationRequest;
	AActor* EyeOrb;
	UCurveFloat* HorizontalCurve;
	UCurveFloat* VerticalCurve;
	UCurveFloat* RandomCurve;
	bool FlipHorizontal;
	bool FlipVertical;
	bool LocationsGenerated;

	int alpha;
	TArray<FVector> colors;
	TArray<FVector> Locations;

	int color_select;
};
