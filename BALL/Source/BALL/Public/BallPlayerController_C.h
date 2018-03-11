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

	virtual void BeginPlay() override;

	ABall_C* GetPlayerBall() const;
	
	
};
