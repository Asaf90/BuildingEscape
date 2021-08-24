// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber component initialized on %s"), *GetOwner()->GetName());
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ViewPointLocation;
	FRotator ViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	// UE_LOG(LogTemp, Warning, TEXT("ViewPoint location: %s. ViewPoint rotation: %s"), *ViewPointLocation.ToString(), *ViewPointRotation.ToString());

	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), ViewPointLocation, LineTraceEnd, FColor(128,255,64), false, 0.0f, 0, 5.0f);

}

