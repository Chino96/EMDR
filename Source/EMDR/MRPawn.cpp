// Fill out your copyright notice in the Description page of Project Settings.

#include "MRPawn.h"
#include "GazeOrb.h"
#include "EMDRMenu.h"
#include "EyeTrackerFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <Input/UxtHandInteractionActor.h>

AMRPawn::AMRPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    GazeData = FEyeTrackerGazeData();
}

void AMRPawn::BeginPlay()
{
    Super::BeginPlay();
    LeftHand = GetWorld()->SpawnActor<AUxtHandInteractionActor>(AUxtHandInteractionActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
    RightHand = GetWorld()->SpawnActor<AUxtHandInteractionActor>(AUxtHandInteractionActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
    LeftHand->SetHand(EControllerHand::Left);
    RightHand->SetHand(EControllerHand::Right);
    LeftHand->SetOwner(this);
    RightHand->SetOwner(this);
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EMDRMenu"), FoundActors);
    EMDRMenu = Cast<AEMDRMenu>(FoundActors[0]);
    
}


void AMRPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (UEyeTrackerFunctionLibrary::GetGazeData(GazeData))
    {
        if(BeginEyeTracking){
            FVector GazeLocation = CameraComponent->GetComponentLocation() + GazeData.GazeDirection * 250;
            GazeOrb->SetActorLocation(FVector(GazeOrb->GetActorLocation().X, GazeLocation.Y, GazeLocation.Z));
            GazeOrb->SetActorRotation(CameraComponent->GetComponentRotation());
        }
        EMDRMenu->SetActorLocation(FVector(CameraComponent->GetComponentLocation().X + 75, CameraComponent->GetComponentLocation().Y, CameraComponent->GetComponentLocation().Z));
    }
    if(EMDRMenu->SessionStarted)
    {
        GazeOrb = GetWorld()->SpawnActor<AGazeOrb>(AGazeOrb::StaticClass(), FVector(EMDRMenu->GetActorLocation().X + 150, EMDRMenu->GetActorLocation().Y, EMDRMenu->GetActorLocation().Z), FRotator(0, 0, 0));
        GazeOrb->SetOwner(this);
        BeginEyeTracking = true;
        EMDRMenu->SessionStarted = false;
    }

}