// Copyright zuzze.tech 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	float InitialYaw;
	float CurrentYaw;

	// This unlocks "TargetYaw" property in UE editor when clicking OpenDoor Actor Component
	UPROPERTY(EditAnywhere)
	float DoorOpenAngle = -40.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpeningSpeed = 0.8f;

	UPROPERTY(EditAnywhere)
	float DoorClosingSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.8f; // seconds

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor *ActorThatOpens;
};
