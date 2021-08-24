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

	// UE_LOG(LogTemp, Warning, TEXT("Grabber component initialized on %s"), *GetOwner()->GetName());

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
	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Input Component on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
	
	
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

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,ViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Trace hit actor: %s"), *HitActor->GetName());
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Pressed!"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Grabber Released!"));
}
