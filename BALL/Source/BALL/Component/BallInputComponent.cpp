// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "BallInputComponent.h"

#include "../Public/Ball_C.h"
#include "../Public/BallPlayerController_C.h"


// Sets default values for this component's properties
UBallInputComponent::UBallInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBallInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PlayerController = Cast<ABallPlayerController_C>(GetOwner());
	Player = PlayerController->GetPlayerBall();

	SetupInputComponent();
}


// Called every frame
void UBallInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBallInputComponent::SetupInputComponent()
{
	InputComponent = Player->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// Bind Input axis
		InputComponent->BindAxis("WForward_Backward", this, &UBallInputComponent::MoveForward);
		InputComponent->BindAxis("ALeft_DRight", this, &UBallInputComponent::MoveRight);
		InputComponent->BindAction("Jump", IE_Pressed, this, &UBallInputComponent::PlayerJump);
		UE_LOG(LogTemp, Error, TEXT("%s - has Component InputComponent..."), *GetOwner()->GetName());
	}

}


void UBallInputComponent::MoveRight(float Val)
{
	Player->SetMoveRight(Val);
}

void UBallInputComponent::MoveForward(float Val)
{
	Player->SetMoveForward(Val);
}

void UBallInputComponent::PlayerJump()
{
	Player->Jump();
}