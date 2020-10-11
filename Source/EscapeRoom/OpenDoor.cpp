// Copyright zuzze.tech 2020

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	DoorOpenAngle += DoorOpenAngle;
	FindPressurePlate();
	FindAudioComponent();
	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	if (TotalMassOfActors() > MassNeededToOpenDoorKg)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

	// In unreal x = roll, y = pitch, z = yaw
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{

	// Adding DeltaTime, it is ensured that the time is same regardless of computer and framerate
	CurrentYaw = FMath::Lerp(CurrentYaw, DoorOpenAngle, DeltaTime * DoorOpeningSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSoundPlayed = false;
	if (!DoorAudio)
	{
		return;
	}
	if (!OpenDoorSoundPlayed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Open sound"));
		DoorAudio->Play();
		OpenDoorSoundPlayed = true;
	}

	// Alternatives
	// OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, DoorOpenAngle, DeltaTime, 45);
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, DoorOpenAngle, DeltaTime, 2);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorClosingSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSoundPlayed = false;
	if (!DoorAudio)
	{
		return;
	}
	if (!CloseDoorSoundPlayed)
	{
		UE_LOG(LogTemp, Warning, TEXT("close sound"));
		DoorAudio->Play();
		CloseDoorSoundPlayed = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float MassOfPressurePlateItemsKg = 0.f;

	// find all actors that overlap with pressure plate
	TArray<AActor *> OverlappingActors;
	if (!PressurePlate)
	{
		return MassOfPressurePlateItemsKg;
	}
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// same as let .. of in js
	for (AActor *Actor : OverlappingActors)
	{
		MassOfPressurePlateItemsKg += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return MassOfPressurePlateItemsKg;
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate set to trigger it"), *GetOwner()->GetName())
	}
}

void UOpenDoor::FindAudioComponent()
{
	DoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!DoorAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Missing audio component"), *GetOwner()->GetName());
	}
}