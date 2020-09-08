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
	UPROPERTY(VisibleAnywhere, Category = "RootMesh")
	USphereComponent *BallRoot;

	UPROPERTY(VisibleAnywhere, Category = "RootMesh")
	class UStaticMeshComponent *Ball;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *JumpEffect = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAudioComponent *JumpSound;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *ImpactEffect = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAudioComponent *ImpactSound;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent *TraceEffect = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAudioComponent *traceSound;
	FName SparksVelocity;

	class USceneComponent *Scene;

	class USpringArmComponent *SpringArm;

	class UCameraComponent* Camera;

	class AInfiniteTerrainGameMode * GameMode;
	

	FRotator MyControlRotator;

	float DeathTime;

	float InputForward;
	float InputRight;
	float ForwardForce;
	float ChangeForce = 200;

	/*bool bStartHit = false;
	float TimeHit = 0.f;*/


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
	/* Limit is 4500 */
	UPROPERTY(EditDefaultsOnly, Category = "Movement Property")
	float MaxForwardForce;


	bool bNotifyHit;
	/*UPROPERTY(VisibleAnywhere, Category = "Particle System")
	UParticleSystemComponent *BallMotionEX = nullptr;*/

	/*UPROPERTY(VisibleAnywhere)
	class UBallInputComponent * MyInputComponent;*/



private:

	void SimulateMove(float DeltaTime);
	void UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime);
	void UpdateRotation(float DeltaTime);

	UFUNCTION()
	void IncreaseMaxForwardForce();

protected:


	// AActor interface
	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;


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

protected:

	//UPROPERTY(EditDefaultsOnly, Category = "Effects")
	//UParticleSystem* ImpactEffect;


public:
	// Sets default values for this pawn's properties
	ABall_C();


	void SetMoveRight(float Val);
	void SetMoveForward(float Val);

	UPROPERTY(BlueprintAssignable)
	FBallDelegate OnDeath;

	UStaticMeshComponent * GetBall() { return Ball; }

	/// Use LookAround Component
	USceneComponent * GetSceneComponent() { return Scene; }
	USpringArmComponent *GetSpringArmComponent() { return SpringArm; }


	/* Game Start from Firts_Level Blueprint */
	UFUNCTION(BlueprintCallable, Category = "Start Game")
	void SetInputForward(float Val = 0) { InputForward = Val; }

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(USceneComponent *scene, USpringArmComponent *springarm, UCameraComponent* camera);

	/* Put Components Settings */
	void Setting();

	///** Handle jump action. Be Careful Jump function use Private member of Ball Class */
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Jump();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ForceApply();


public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};




