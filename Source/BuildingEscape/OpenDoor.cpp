// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	TargetYaw = GetOwner()->GetActorRotation().Yaw + TargetYawRelative;

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("Current Yaw value: %f"), GetOwner()->GetActorRotation().Yaw);	// debug line
	// UE_LOG(LogTemp, Warning, TEXT("Current FTransform: %s"), *GetOwner()->GetActorRotation().ToString());	// debug line

	
	FRotator CurrentRotator = GetOwner()->GetActorRotation();
	CurrentRotator.Yaw = FMath::FInterpTo(CurrentRotator.Yaw, TargetYaw, DeltaTime, 2);
	GetOwner()->SetActorRotation(CurrentRotator);
}

