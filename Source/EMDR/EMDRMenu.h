// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationSolver.h"
#include "EMDRMenu.generated.h"

UCLASS()
class EMDR_API AEMDRMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMDRMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	AActor* Parent;
	virtual void Tick(float DeltaTime) override;
	AActor* RadioButtonRadial;
	AActor* RadioButtonAscending;
	AActor* RadioButtonLinear;
	FTimerHandle MyTimerHandle;
	int32 OrbPos = 0;
	int Frames = 0;
	bool Hidden;
	bool RadialMovement = false;
	bool SessionStarted = false;
	TArray<UActorComponent*> StartButtons;


	UFUNCTION(BlueprintCallable)
	void setOrbVisible();


	UFUNCTION(BlueprintCallable)
	void DestroyOrbs();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnOrbsRadial();

	UFUNCTION(BlueprintCallable)
	void SpawnOrbs(FString type);

	UFUNCTION(BlueprintCallable)
	void SpawnOrbsSingular();

	TArray<AActor*> RadialOrbs;

	UFUNCTION(BlueprintCallable)
	void RadialOrbMovement();

	ULocationSolver* LocationSolver;

public:
	UFUNCTION(BlueprintCallable)
	ULocationSolver* GetLocationSolver() const { return LocationSolver; }
};
