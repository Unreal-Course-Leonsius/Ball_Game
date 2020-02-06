// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Input.h"

#include "Components/Button.h"


//#include "../Component/PlayerInputComponent.h"


bool UInput::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	Player = Cast<ABall_C>(GetOwningPlayerPawn());
	if (!ensure(Player != nullptr)) return false;

	if (!ensure(Left != nullptr)) return false;
	Left->OnPressed.AddDynamic(this, &UInput::Left_Direction);
	Left->OnReleased.AddDynamic(this, &UInput::Stop);

	if (!ensure(Right != nullptr)) return false;
	Right->OnPressed.AddDynamic(this, &UInput::Right_Direction);
	Right->OnReleased.AddDynamic(this, &UInput::Stop);


	UE_LOG(LogTemp, Warning, TEXT("Widget_Input_C"));
	return true;
}

void UInput::Left_Direction()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftDirection"));
	Player->SetMoveRight(-1);
}

void UInput::Right_Direction()
{
	UE_LOG(LogTemp, Warning, TEXT("RightDirection"));
	Player->SetMoveRight(1);
}

void UInput::Stop()
{
	Player->SetMoveRight(0);
}

