// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LookAround.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API ULookAround : public UActorComponent
{
	GENERATED_BODY()

private:

	UInputComponent* InputComponentt = nullptr;
	class ABall_C * Player = nullptr;

private:

	/// Setup (assumed) attached input component
	void SetupInputComponent();

	/// Function Ability
	void Azimuth(float Val);
	void Elevation(float Val);

public:	
	// Sets default values for this component's properties
	ULookAround();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
