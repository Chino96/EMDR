// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/SphereComponent.h>
#include "GameFramework/Actor.h"
#include "EMDRMenu.h"
#include "GazeOrb.generated.h"

UCLASS()
class EMDR_API AGazeOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGazeOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndDeactivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UMaterialInstanceDynamic* DynamicMaterial;
	UStaticMeshComponent* mesh;
	AEMDRMenu* EMDRMenu;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
