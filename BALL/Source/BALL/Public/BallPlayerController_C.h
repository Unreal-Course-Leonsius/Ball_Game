// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BallPlayerController_C.generated.h"

// Forward Declaration
class ABall_C;

/**
 * 
 */
UCLASS()
class BALL_API ABallPlayerController_C : public APlayerController
{
	GENERATED_BODY()
	
public:

	ABallPlayerController_C();

	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	ABall_C* GetPlayerBall() const;
	
	UFUNCTION()
	void OnPossessedBallDeath();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void CreateInputWidget();

private:

	TSubclassOf<class UUserWidget> BPInput;
	class UInput* Input = nullptr;
	
};
