// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BallInputTouchComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALL_API UBallInputTouchComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBallInputTouchComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/// Setup (assumed) attached input component
	void SetupInputComponent();
	
	void SlideOnScreen();
	void EndSlideOnScreen();

	void MoveForward(float Val);

private:

	UInputComponent* InputComponent = nullptr;
	class ABallPlayerController_C* PlayerController;
	class ABall_C * Player = nullptr;

	FVector2D StartTouch;
	FVector2D SlideTouch;
	FVector2D EndTouch;

	bool bSlide;

	float MovePlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Touch", meta = (ClampMin = 0.f))
	float MaxSlideRange;

	UPROPERTY(EditDefaultsOnly, Category = "Touch", meta = (ClampMin = 100.f, ClampMax = 400.f))
	float StartTouchSpeed;
	
};
