// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"


// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABall_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABall_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

