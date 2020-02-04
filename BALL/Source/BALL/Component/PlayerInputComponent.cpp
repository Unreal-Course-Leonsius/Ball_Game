// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "PlayerInputComponent.h"

#include "../Public/Ball_C.h"


// Sets default values for this component's properties
UPlayerInputComponent::UPlayerInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Player = Cast<ABall_C>(GetOwner());

	SetupInputComponent();
	
}


// Called every frame
void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerInputComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// Bind Input axis
		//InputComponent->BindAxis("WForward_Backward", this, &UPlayerInputComponent::MoveForward);
		InputComponent->BindAxis("ALeft_DRight", this, &UPlayerInputComponent::MoveRight);
		UE_LOG(LogTemp, Error, TEXT("%s - has Component InputComponent..."), *GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s - has not Component InputComponent..."), *GetOwner()->GetName())
	}
}

void UPlayerInputComponent::MoveRight(float Val)
{
	Player->SetMoveRight(Val);
}

void UPlayerInputComponent::MoveForward(float Val)
{
	Player->SetMoveForward(Val);
}

