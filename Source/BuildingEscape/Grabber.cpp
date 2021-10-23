// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();	
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector ViewPointLocation;		// initialize OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// initialize OUT parameter for player viewpoint
	// finding the line trace end in order to see which actor is in reach. to be refactored to a function as it repeats several times in the code.
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint
	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Pressed!"));		// debug line to test if grabber is activated with key press

	FVector ViewPointLocation;		// initialize OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// initialize OUT parameter for player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint
	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	// finding the line trace end in order to see which actor is in reach. to be refactored to a function as it repeats several times in the code.

	FHitResult GrabbedBody = GetFirstPhysicsBodyInReach();
	if (GrabbedBody.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(GrabbedBody.GetComponent(), NAME_None, LineTraceEnd);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Released!"));		// debug line to test if grabber is released with key release
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::FindPhysicsHandle()				// If there is no physics handle for the actor, show an error code that explains how to fix it.
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, 
		Error, 
		TEXT("%s has no PhysicsHandle component! To fix this, double click on %s in the content browser, then Add Component -> PhysicsHandle."), 
		*GetOwner()->GetName(),
		*GetOwner()->GetName()
		)
	}
}

void UGrabber::SetupInputComponent()			// Couple key press with action
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector ViewPointLocation;		// initialize OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// initialize OUT parameter for player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint
	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	// finding the line trace end in order to see which actor is in reach. to be refactored to a function as it repeats several times in the code.

	FHitResult Hit;		// initialize OUT parameter for the physics body hit result
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, ViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)				// Debug line to see if the line trace hit an actor with a physics handle
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace hit actor: %s"), *HitActor->GetName());
	}
	else UE_LOG(LogTemp, Warning, TEXT("No actor with physics handle in reach."));
	
	return Hit;
	
}
