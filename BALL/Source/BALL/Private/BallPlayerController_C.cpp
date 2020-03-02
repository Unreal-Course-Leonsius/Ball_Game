// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"
#include "BallPlayerController_C.h"

#include "../Widget/Input.h"
#include "../Component/BallInputComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"



ABallPlayerController_C::ABallPlayerController_C()
{
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("PlayerController_Constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> InputBPClass(TEXT("/Game/Widget/WBP_Input"));
	if (!ensure(InputBPClass.Class != nullptr)) return;

	BPInput = InputBPClass.Class;

	//BallInputComponent = CreateDefaultSubobject<UBallInputComponent>(TEXT("BallInputComponent"));
}

void ABallPlayerController_C::BeginPlay()
{
	Super::BeginPlay();

	auto Ball = GetPlayerBall();
	//UE_LOG(LogTemp, Warning, TEXT("Ball Name: %s"), *Ball->GetName());

	CreateInputWidget();
	
}

void ABallPlayerController_C::CreateInputWidget()
{

	if (!ensure(BPInput != nullptr)) return;
	Input = CreateWidget<UInput>(this, BPInput);
	
	if (Input != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input = %s"), *Input->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input = NULL"));
	}
		

	//bShowMouseCursor = true;

	Input->SetGameUIMode();
	Input->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("PlayerController_BeginPlay_BP"));
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

