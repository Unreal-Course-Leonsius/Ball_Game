// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "BallInputTouchComponent.h"

#include "../Public/Ball_C.h"
#include "../Public/BallPlayerController_C.h"

#define OUT

// Sets default values for this component's properties
UBallInputTouchComponent::UBallInputTouchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxSlideRange = 20.f;
	StartTouchSpeed = 250;
}


// Called when the game starts
void UBallInputTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABallPlayerController_C>(GetOwner());
	if (!ensure(PlayerController != nullptr)) return;

	Player = PlayerController->GetPlayerBall();

	PlayerController->bShowMouseCursor = true;

	SetupInputComponent();
	
}


// Called every frame
void UBallInputTouchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerController->GetInputTouchState(ETouchIndex::Touch1, OUT SlideTouch.X, OUT SlideTouch.Y, OUT bSlide);

	
	if (bSlide)
	{
		//PlayerController->GetInputTouchState(ETouchIndex::Touch1, OUT SlideTouch.X, OUT SlideTouch.Y, OUT bSlide);
		//UE_LOG(LogTemp, Warning, TEXT("Touch Slide = %s, bSlide = %i"), *SlideTouch.ToString(), bSlide);

		if (FMath::Abs(int32(SlideTouch.X - StartTouch.X)) <= MaxSlideRange)
		{
			//StartTouch = SlideTouch;
			MovePlayer = 0;
			Player->SetMoveRight(MovePlayer);
			UE_LOG(LogTemp, Error, TEXT("Touch Slide.x = %f, StartTouch.X = %f,  bSlide = %i"), SlideTouch.X, StartTouch.X, bSlide);
		}
		else if (FMath::Abs(int32(SlideTouch.X - StartTouch.X)) >= MaxSlideRange)
		{
			if ((SlideTouch - StartTouch).X > 0)
			{
				MovePlayer = 1;
				Player->SetMoveRight(MovePlayer);
				StartTouch.X += (DeltaTime * StartTouchSpeed);
			}
			else
			{
				MovePlayer = -1;
				Player->SetMoveRight(MovePlayer);
				StartTouch.X -= (DeltaTime * StartTouchSpeed);
			}

			UE_LOG(LogTemp, Warning, TEXT("Touch Slide.x = %i, StartTouch.X = %i,  bSlide = %i"), int32(SlideTouch.X), int32(StartTouch.X), bSlide);
			
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not any Condition"));
		}

		
	}

}

void UBallInputTouchComponent::SetupInputComponent()
{
	InputComponent = Player->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//InputComponent->BindAxis("WForward_Backward", this, &UBallInputTouchComponent::MoveForward);
		InputComponent->BindAction("Slide", IE_Pressed, this, &UBallInputTouchComponent::SlideOnScreen);
		InputComponent->BindAction("Slide", IE_Released, this, &UBallInputTouchComponent::EndSlideOnScreen);
		UE_LOG(LogTemp, Warning, TEXT("InputTouchComponent is Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is Null"));
	}
}

void UBallInputTouchComponent::MoveForward(float Val)
{
	Player->SetMoveForward(Val);
}

void UBallInputTouchComponent::SlideOnScreen()
{
	PlayerController->GetInputTouchState(ETouchIndex::Touch1, OUT StartTouch.X, OUT StartTouch.Y, OUT bSlide);
	UE_LOG(LogTemp, Warning, TEXT("Touch Start = %s,  bSlide = %i"), *StartTouch.ToString(), bSlide);
}

void UBallInputTouchComponent::EndSlideOnScreen()
{
	MovePlayer = 0;
	PlayerController->GetInputTouchState(ETouchIndex::Touch1, OUT EndTouch.X, OUT EndTouch.Y, OUT bSlide);
	UE_LOG(LogTemp, Warning, TEXT("Touch End = %s,  bSlide = %i"), *EndTouch.ToString(), bSlide);
	Player->SetMoveRight(MovePlayer);
}

