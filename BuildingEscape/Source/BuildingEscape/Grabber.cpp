// Copyright Paolo Ratti Tamayo

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Define an empty macro to label outcomponents
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

	UE_LOG(LogTemp, Warning, TEXT("DefaultPawn has a grabber :D"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player's point of view 
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	//UE_LOG(LogTemp, Warning, TEXT("Player's location: %s. Player's viewpoint: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());

	// Draw a red trace	

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector()*Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); 
	// Text:  
	// blnTraceComplex: false, because we only need simple collisions with the chairs and tables
	// IgnoreActor: GetOwner, AKA the player, since the player will always be touching the trace line

	// Line-trace (Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation, //Start location of the ray
		LineTraceEnd, //End location of the ray
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //List of Objects it is looking for
		TraceParameters
	);
	
	// See what we hit
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line Trace hit: %s"), *(ActorHit->GetName()));



}

