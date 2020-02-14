// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BallMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API UBallMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallMovementComponent();

	//void SetForwardForce(float Force) { ForwardForce = Force; }

	//void SetInputForward(float input) { InputForward = input; }
	//void SetInputRight(float input) { InputRight = input; }

	//void SetBallPlayer(UStaticMeshComponent * Ball) { BallPlayer = Ball; }

	///** Handle jump action. Be Careful Jump function use Private member of Ball Class */
	//UFUNCTION(BlueprintCallable, Category = "Setup")
	//void Jump();

	//void ImpactBallPlayer(FVector Impulse);
	//void SetMovementProperty(float rolltorque, float jumpimpulse, float maxrightforce, float rotationspeed, float maxforwardforce);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//void UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime);
	//void UpdateRotation(float DeltaTime);

	//void SimulateMove(float DeltaTime);

private:

	
	//float Movement_MaxRightForce;
	//float Movement_RotationSpeed;

	///** Torque to apply when trying to roll ball */
	//float Movement_RollTorque;

	///** Vertical impulse to apply when pressing jump */
	//float Movement_JumpImpulse;
	//float Movement_MaxForwardForce;


	//float InputForward;
	//float InputRight;
	//float ForwardForce;
	//float MinForwardForce = 450;

	//FRotator MyControlRotator;
	//UStaticMeshComponent *BallPlayer;
	
};
