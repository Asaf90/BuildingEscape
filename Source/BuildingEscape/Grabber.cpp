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

	// UE_LOG(LogTemp, Warning, TEXT("Grabber component initialized on %s"), *GetOwner()->GetName());
	FindPhysicsHandle();	
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector ViewPointLocation;		// OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// OUT parameter for player viewpoint

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint

	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Pressed!"));

	FVector ViewPointLocation;		// OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// OUT parameter for player viewpoint

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint

	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;

	FHitResult GrabbedBody = GetFirstPhysicsBodyInReach();
	if (GrabbedBody.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(GrabbedBody.GetComponent(), NAME_None, LineTraceEnd);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Released!"));
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics Handle is found.
	}
	else
	{
		UE_LOG(LogTemp, 
		Error, 
		TEXT("%s has no PhysicsHandle component! To fix this, double click on %s in the content browser, then Add Component -> PhysicsHandle."), 
		*GetOwner()->GetName(),
		*GetOwner()->GetName()
		)
	}
}

void UGrabber::SetupInputComponent()
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
	FVector ViewPointLocation;		// OUT parameter for player viewpoint
	FRotator ViewPointRotation;		// OUT parameter for player viewpoint

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);	// Player viewpoint

	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;

	FHitResult Hit;		// OUT parameter for the physics body hit result
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, ViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace hit actor: %s"), *HitActor->GetName());
	}
	else UE_LOG(LogTemp, Warning, TEXT("No actor with physics handle in reach."));
	
	return Hit;
	
}
