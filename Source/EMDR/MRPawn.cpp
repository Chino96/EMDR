// Fill out your copyright notice in the Description page of Project Settings.

#include "MRPawn.h"
#include "GazeOrb.h"
#include "EyeOrb.h"
#include "EMDR_MovementComponent.h"
#include "EMDRMenu.h"
#include "EyeTrackerFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <Input/UxtHandInteractionActor.h>
#include "DrawDebugHelpers.h" // Add this include for debug drawing

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
    RayColor = FColor::Green; // Default ray color
    
}

TArray<AActor*> AMRPawn::RaycastGridScan(FVector StartLocation, FVector ForwardVector, int32 GridHalfSize, float RayLength, float GridSpacing, FColor tRayColor)
{
    TArray<AActor*> HitActors;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    for (int32 i = -GridHalfSize; i <= GridHalfSize; ++i)
    {
        for (int32 j = -GridHalfSize; j <= GridHalfSize; ++j)
        {
            FVector Offset = FVector(0, i * GridSpacing, j * GridSpacing);
            FRotator ForwardRot = ForwardVector.Rotation();
            FVector RotatedOffset = ForwardRot.RotateVector(Offset);
            FVector RayStart = StartLocation + RotatedOffset;

            // Calculate an angle offset based on i and j
            float AngleOffsetYaw = i * .20f; // Adjust multiplier for spread
            float AngleOffsetPitch = j * .20f;
            FRotator RayDirectionRot = ForwardVector.Rotation();
            RayDirectionRot.Yaw += AngleOffsetYaw;
            RayDirectionRot.Pitch += AngleOffsetPitch;
            FVector RayDirection = RayDirectionRot.Vector();

            FVector RayEnd = RayStart + RayDirection * RayLength;

            // Draw debug line for each ray
            DrawDebugLine(GetWorld(), RayStart, RayEnd, tRayColor, false, 0.1f, 0, 1.0f);

            FHitResult HitResult;
            if (GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECC_WorldDynamic, CollisionParams))
            {
                if (HitResult.GetActor())
                {
                    HitActors.Add(HitResult.GetActor());
                }
            }
        }
    }
    return HitActors;
}

void AMRPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    BeginEyeTracking = true;
    //Get the Location Solver component from the EMDR menu
    ULocationSolver* LocationSolver = Cast<ULocationSolver>(EMDRMenu->GetComponentByClass(ULocationSolver::StaticClass()));
    FVector Heading = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 60;
    FVector RayHeading = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10;
    LocationSolver->SetActorLocation(Heading);
    LocationSolver->SetActorRotation(CameraComponent->GetComponentRotation());

    if (UEyeTrackerFunctionLibrary::GetGazeData(GazeData))
    {
        if(BeginEyeTracking){
            FVector GazeDirection = GazeData.GazeDirection;
            //GazeOrb->SetActorLocation(GazeLocation);
            //GazeOrb->SetActorRotation(CameraComponent->GetComponentRotation());

            TArray<AActor*> HitActors = RaycastGridScan(RayHeading, GazeDirection, 6, 100000, .20, RayColor);
            //Search for the EyeOrb in the HitActors
            bool HitEyeOrb = false;
            for (AActor* Actor : HitActors)
            {
                if (Actor->IsA(AEyeOrb::StaticClass()))
                {
                    HitEyeOrb = true;
                    break;
                }
            }
            
            if(HitEyeOrb)
            {
                RayColor = FColor::Red;
                
                // Get the UEMDR_MovementComponent from the Start Button on the EMDRMenu and set Glow to true
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMDRMenu::StaticClass(), FoundActors);
                AEMDRMenu* FoundEMDRMenu = Cast<AEMDRMenu>(FoundActors[0]);
                if (FoundEMDRMenu)
                {
                    TArray<UActorComponent*> ChildActors = FoundEMDRMenu->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Start"));
                    for(int i = 0; i < ChildActors.Num(); i++)
                    {
                        UEMDR_MovementComponent* movementComponent = Cast<UEMDR_MovementComponent>(Cast<UChildActorComponent>(ChildActors[i])->GetChildActor()->GetComponentByClass(UEMDR_MovementComponent::StaticClass()));
                        if (movementComponent)
                        {
                            movementComponent->Glow = true;
                        }
                    }
                }
            }
            else
            {
                RayColor = FColor::Green;
                
                // Get the UEMDR_MovementComponent from the Start Button on the EMDRMenu and set Glow to false
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEMDRMenu::StaticClass(), FoundActors);
                AEMDRMenu* FoundEMDRMenu = Cast<AEMDRMenu>(FoundActors[0]);
                if (FoundEMDRMenu)
                {
                    TArray<UActorComponent*> ChildActors = FoundEMDRMenu->GetComponentsByTag(UChildActorComponent::StaticClass(), FName("Start"));
                    for(int i = 0; i < ChildActors.Num(); i++)
                    {
                        UEMDR_MovementComponent* movementComponent = Cast<UEMDR_MovementComponent>(Cast<UChildActorComponent>(ChildActors[i])->GetChildActor()->GetComponentByClass(UEMDR_MovementComponent::StaticClass()));
                        if (movementComponent)
                        {
                            movementComponent->Glow = false;
                        }
                    }
                }
            }
        }
    }
    if(EMDRMenu->SessionStarted)
    {
        //GazeOrb = GetWorld()->SpawnActor<AGazeOrb>(AGazeOrb::StaticClass(), FVector(EMDRMenu->GetActorLocation().X + 150, EMDRMenu->GetActorLocation().Y, EMDRMenu->GetActorLocation().Z), FRotator(0, 0, 0));
        //GazeOrb->SetOwner(this);
        EMDRMenu->SessionStarted = false;
    }

}