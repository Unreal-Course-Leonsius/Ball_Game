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


// Called when the game starts
void UMoveRightLeft::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoveRightLeft::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

