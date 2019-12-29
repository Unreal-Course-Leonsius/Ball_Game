// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Ball_C.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBallDelegate);

UCLASS()
class BALL_API ABall_C : public APawn
{
	GENERATED_BODY()

private:

	/** StaticMesh used for the ball -- we don't create we find Components */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *Ball;

	class USceneComponent *Scene;

	class USpringArmComponent *SpringArm;

	class UCameraComponent* Camera;

	float DeathTime;

	float InputForward;
	float InputRight;

	float Velocity = 0;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	bool bCanJump;

protected:

	/** Indicates whether we can use MoveForward() Function */
	UPROPERTY()
	bool bCanMoveForward;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float Mass = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float DragCoefficient = 60;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float RollTorque;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float JumpImpulse;

	/** MoveRight MoveForward Speed */
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float MoveRightVector;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float MoveForwardVector;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float MaxMoveForce;

	UPROPERTY()
	FVector JumpMoveDestination;

	FRotator MyControlRotator;
	//FVector ChangeLocation;

protected:


private:

	void SimulateMove(float DeltaTime);
	void UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime);
	void UpdateRotation(float DeltaTime);
	//void GetAirResistance();

	void MoveAfterJump();

protected:

	/// Function Ability
	void Azimuth(float Val);
	void Elevation(float Val);

	/** Called for side to side input */
	void MoveRight(float Val);



	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);



	// AActor interface
	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// We may Use this Function instead of NotifyHit BECAUSE there was problem about Jump Function()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/// nomore need Event we substitution it UnPosses() function in BallPlayerController
	//UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	//void Death();

	AActor * HitActor = nullptr;
	FString HitActorName;


	/// Rotator Transform Vectors
	/** FRotator Transform ForwardVector */
	FVector GetForwardVector_Roll(FRotator InRot);

	/** FRotator Transform UpVector*/
	FVector GetUpVector_Yaw(FRotator InRot);

	/** FRotator Transform Right Vector*/
	FVector GetRightVector_Pitch(FRotator InRot);


	void GetNotifyHitName(AActor* HitActorr);


public:
	// Sets default values for this pawn's properties
	ABall_C();

	
	FBallDelegate OnDeath;

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent *ball, USceneComponent *scene, USpringArmComponent *springarm, UCameraComponent* camera);

	/* Put Components Settings */
	void Setting();

	/** Handle jump action. Be Careful Jump function use Private member of Ball Class */
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Jump();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ForceApply();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetJumpMoveDestination(FVector Destination);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetMoveForward(bool bValue);


public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};