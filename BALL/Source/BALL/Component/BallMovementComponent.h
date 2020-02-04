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

	void SetForwardForce(float Force) { ForwardForce = Force; }
	float GetMaxForwardForce() { return MaxForwardForce; }

	void SetInputForward(float input) { InputForward = input; }
	void SetInputRight(float input) { InputRight = input; }

	///** Handle jump action. Be Careful Jump function use Private member of Ball Class */
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Jump();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime);
	void UpdateRotation(float DeltaTime);

	void SimulateMove(float DeltaTime);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float MaxRightForce;

	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float RotationSpeed;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float RollTorque;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float JumpImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float MaxForwardForce;


	float InputForward;
	float InputRight;
	float ForwardForce;

	FRotator MyControlRotator;

	class ABall_C * Player;
	
};
