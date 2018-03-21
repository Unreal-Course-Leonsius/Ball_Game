// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Ball_C.generated.h"

UCLASS()
class BALL_API ABall_C : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball -- we don't create we find Components */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *Ball;

	class USceneComponent *Scene;

	class USpringArmComponent *SpringArm;

	class UCameraComponent* Camera;

public:
	// Sets default values for this pawn's properties
	ABall_C();

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category = "Ball")
	float RollTorque;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	bool bCanJump;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent *ball, USceneComponent *scene, USpringArmComponent *springarm, UCameraComponent* camera);

	/* Put Components Settings */
	void Setting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Azimuth(float Val);
	void Elevation(float Val);

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
