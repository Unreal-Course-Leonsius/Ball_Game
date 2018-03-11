// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"
#include "BallPlayerController_C.h"


void ABallPlayerController_C::BeginPlay()
{
	Super::BeginPlay();

	auto Ball = GetPlayerBall();
	UE_LOG(LogTemp, Warning, TEXT("Ball Name: %s"), *Ball->GetName());
}

ABall_C* ABallPlayerController_C::GetPlayerBall() const
{
	return Cast<ABall_C>(GetPawn());
}

