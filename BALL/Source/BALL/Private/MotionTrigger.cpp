// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "MotionTrigger.h"


// Sets default values for this component's properties
UMotionTrigger::UMotionTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	//WallMovingTrigger = CreateDefaultSubobject<FWallMoving>(FName("Wall Moving Array"));
	
}


void UMotionTrigger::GrowingWall()
{
	TimerDel.BindUFunction(this, FName("StartGrowing"));
	//TimerDel.BindUFunction(this, FName("StartGrowing"));
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDel, growing_Delation_Time, true);
}

void UMotionTrigger::StartGrowing()
{
	if (growing_Wall_Number_Index < growing_Wall_Number)
	{
		WallMovingTrigger[growing_Wall_Number_Index].Broadcast();
		growing_Wall_Number_Index++;
	}
}

// Called when the game starts
void UMotionTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...

	WallMovingTrigger.Add(MovingRightWall_1);
	WallMovingTrigger.Add(MovingLeftWall);
	WallMovingTrigger.Add(MovingRightWall_2);

	Owner = GetOwner();
	OwnerOriginLocation = GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("OwnerClass Name %s"), *Owner->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("OwnerOriginLocation %s"), *OwnerOriginLocation.ToString());
	
}


// Called every frame
void UMotionTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("1_bRight %i"), bRight);
	auto OwnerCurrentLocation = GetOwner()->GetActorLocation();
	TickTime += DeltaTime;
	if (TickTime > 1.f)
	{
		Move.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("2_bRight %i"), bRight);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("OwnerCurrentLocation %s"), *OwnerCurrentLocation.ToString());
	//UE_LOG(LogTemp, Error, TEXT("OwnerOriginLocation %s"), *OwnerOriginLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("GetActorLocation %s"), *GetOwner()->GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("========================================================="));

}


// ========================= Need not any more this function ========================= //


/*void UMotionTrigger::MoveLeftRight()
{
auto StartLocation = Owner->GetActorLocation();
//UE_LOG(LogTemp, Warning, TEXT("1 OwnerActor Location: %s"), *StartLocation.ToString());
auto Direction = Owner->GetActorForwardVector().GetSafeNormal() * 5000;

auto EndLocation = StartLocation + Direction;

Owner->SetActorLocation(EndLocation);

UE_LOG(LogTemp, Warning, TEXT("2 OwnerActor Location: %s"), *Owner->GetActorLocation().ToString());
UE_LOG(LogTemp, Warning, TEXT("OwnerActor RightVectorNormal: %s"), *Direction.ToString());
UE_LOG(LogTemp, Warning, TEXT("EndLocation: %s"), *EndLocation.ToString());
}*/

