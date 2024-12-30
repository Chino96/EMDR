// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMDRMenu.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DefaultPawn.h"
#include <Input/UxtHandInteractionActor.h>
#include "EyeTrackerFunctionLibrary.h"
#include "GazeOrb.h"
#include "MRPawn.generated.h"

UCLASS()
class EMDR_API AMRPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	AMRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AEMDRMenu* EMDRMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AGazeOrb* GazeOrb;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AUxtHandInteractionActor* LeftHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AUxtHandInteractionActor* RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FEyeTrackerGazeData GazeData;

	bool BeginEyeTracking = false;
};
