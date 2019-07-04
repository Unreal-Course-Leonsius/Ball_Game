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

void ABallPlayerController_C::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedBall = Cast<ABall_C>(InPawn);
		if (!ensure(PossessedBall)) return;

		PossessedBall->OnDeath.AddUniqueDynamic(this, &ABallPlayerController_C::OnPossessedBallDeath);
	}
}

ABall_C* ABallPlayerController_C::GetPlayerBall() const
{
	return Cast<ABall_C>(GetPawn());
}

void ABallPlayerController_C::OnPossessedBallDeath()
{

	//StartSpectatingOnly();
	UnPossess();

}

