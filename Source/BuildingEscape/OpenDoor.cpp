// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ClosedYaw = GetOwner()->GetActorRotation().Yaw;
	OpenYaw = ClosedYaw + OpenYawRelative;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component but no PressurePlate set."), *GetOwner()->GetName());
	}
	
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpen >= DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
			
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotator = GetOwner()->GetActorRotation();
	CurrentRotator.Yaw = FMath::FInterpTo(CurrentRotator.Yaw, OpenYaw, DeltaTime, DoorOpenSpeedParameter);
	GetOwner()->SetActorRotation(CurrentRotator);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotator = GetOwner()->GetActorRotation();
	CurrentRotator.Yaw = FMath::FInterpTo(CurrentRotator.Yaw, ClosedYaw, DeltaTime, DoorCloseSpeedParameter);
	GetOwner()->SetActorRotation(CurrentRotator);
}

