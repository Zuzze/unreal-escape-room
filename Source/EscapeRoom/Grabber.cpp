// Copyright zuzze.tech 2020
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"

// this allows us to improve readability to show what is outparameter
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
	UE_LOG(LogTemp, Warning, TEXT("Grabber ready for duty"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// Log user location and rotation
	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
		   *PlayerViewPointLocation.ToString(),
		   *PlayerViewPointRotation.ToString());

	// Ray cast to certain distance (Reach)

	// See what the raycaster hits (what we can grab)
}
