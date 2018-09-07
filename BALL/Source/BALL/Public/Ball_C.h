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
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float RollTorque;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float JumpImpulse;

	/** MoveRight MoveForward Speed */
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float MoveRihgt_Left;

	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	float MoveForward_Backward;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	bool bCanJump;

	/** Indicates whether we can use SetActorLocation() Function */
	bool bCanActorLocation;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UStaticMeshComponent *ball, USceneComponent *scene, USpringArmComponent *springarm, UCameraComponent* camera);

	/* Put Components Settings */
	void Setting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;


	/// Function Ability

	void Azimuth(float Val);
	void Elevation(float Val);

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action. */
	void Jump();

protected:

	AActor * HitActor = nullptr;
	FString HitActorName;

	/// Rotator Transform Vectors
	/** FRotator Transform ForwardVector */
	FVector GetForwardVector(FRotator InRot);

	/** FRotator Transform UpVector*/
	FVector GetMyUpVector(FRotator InRot);

	/** FRotator Transform Right Vector*/
	FVector GetMyRightVector(FRotator InRot);


	void GetNotifyHitName(FHitResult& HitActorr);

	void ForceApply();


	//FName GetNotifyHitName(FHitResult& HitActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
