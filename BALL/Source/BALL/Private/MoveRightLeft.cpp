// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "MoveRightLeft.h"


// Sets default values for this component's properties
UMoveRightLeft::UMoveRightLeft()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


void UMoveRightLeft::ReadyMove()
{
	bRight = true;
	return;
}

// Called when the game starts
void UMoveRightLeft::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner();
	OwnerOriginLocation = GetOwner()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("OwnerClass Name %s"), *Owner->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OwnerOriginLocation %s"), *OwnerOriginLocation.ToString());
	
}

void UMoveRightLeft::MoveLeftRight()
{
	auto StartLocation = Owner->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("1 OwnerActor Location: %s"), *StartLocation.ToString());
	auto Direction = Owner->GetActorForwardVector().GetSafeNormal() * 5000;

	auto EndLocation = StartLocation + Direction;

	Owner->SetActorLocation(EndLocation);

	/*UE_LOG(LogTemp, Warning, TEXT("2 OwnerActor Location: %s"), *Owner->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("OwnerActor RightVectorNormal: %s"), *Direction.ToString());
	UE_LOG(LogTemp, Warning, TEXT("EndLocation: %s"), *EndLocation.ToString()); */
}


// Called every frame
void UMoveRightLeft::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	auto OwnerCurrentLocation = GetOwner()->GetActorLocation();
	//OwnerCurrentLocation.Y > (OwnerOriginLocation.Y - 300);
	TickTime += DeltaTime;
	if (TickTime > 5.f && bRight)
	{
		//MoveLeftRight();
		Right.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("OwnerCurrentLocation %s"), *OwnerCurrentLocation.ToString());
		bRight = false;
		bLeft = true;
		//UE_LOG(LogTemp, Warning, TEXT("bRight -- OwnerCurrentLocation %s"), *OwnerCurrentLocation.ToString());
	}
	else if (bLeft && OwnerCurrentLocation.Y < (OwnerOriginLocation.Y - 4000))
	{
		Left.Broadcast();
		bLeft = false;
		//UE_LOG(LogTemp, Warning, TEXT("bLeft -- OwnerCurrentLocation %s"), *OwnerCurrentLocation.ToString());
	}
	else if (OwnerCurrentLocation.Y == OwnerOriginLocation.Y || OwnerCurrentLocation.Y > OwnerOriginLocation.Y)
	{
		//auto Right = GetOwner()->GetComponents<TimeLineComponent>();
		bRight = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("OwnerCurrentLocation %s"), *OwnerCurrentLocation.ToString());
	UE_LOG(LogTemp, Error, TEXT("OwnerOriginLocation %s"), *OwnerOriginLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("bRight %i"), bRight);
}

