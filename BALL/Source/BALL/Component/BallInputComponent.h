// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "../Public/Ball_C.h"
#include "BallInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API UBallInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallInputComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/// Setup (assumed) attached input component
	void SetupInputComponent();

public:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

private:

	UInputComponent* InputComponent = nullptr;
	class ABall_C * Player = nullptr;

};
