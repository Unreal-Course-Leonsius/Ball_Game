// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"


// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RollTorque = 500000000.0f;
	//JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump
}

void ABall_C::Initialize(UStaticMeshComponent * ball, USceneComponent * scene, USpringArmComponent * springarm, UCameraComponent* camera)
{
	/* Get Components */
	Ball = ball;
	Scene = scene;
	SpringArm = springarm;
	Camera = camera;

	Setting();
}

void ABall_C::Setting()
{
	Scene->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->bDoCollisionTest = false;
	//SpringArm->bAbsoluteRotation = true; // must not be bAbsolutRotation = true because Scene Component work
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

	// Mouse Movement
	PlayerInputComponent->BindAxis("Athimuth", this, &ABall_C::Azimuth);
	PlayerInputComponent->BindAxis("Elevation", this, &ABall_C::Elevation);

	// Forwad Backward
	PlayerInputComponent->BindAxis("WForward_Backward", this, &ABall_C::MoveForward);
	PlayerInputComponent->BindAxis("ALeft_DRight", this, &ABall_C::MoveRight);

}


void ABall_C::Azimuth(float Val)
{
	//auto NewRotation = FRotator(0.f, 0.f, Val * GetWorld()->GetDeltaSeconds() * 50);
	//Scene->SetRelativeRotation(NewRotation);

	auto pitch = Val * GetWorld()->GetDeltaSeconds() * 50;
	auto NewRotation = FRotator(0.f, pitch, 0.f); 
	Scene->AddLocalRotation(NewRotation);


}

void ABall_C::Elevation(float Val)
{
	auto NewRotation = FRotator(Val * GetWorld()->GetDeltaSeconds() * 50, 0.f, 0.f);
	SpringArm->AddLocalRotation(NewRotation);
}

void ABall_C::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	//Ball->AddTorque(Torque);
	//auto ForceApply = Ball->GetRightVector() * Val * 5000000; // We need not GetRightVector() because when Ball collide something right and forward vector is messed
	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, Val * GetWorld()->GetDeltaSeconds() * 1000, 0.f);

	SetActorLocation(WorldDirection);
}

void ABall_C::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Ball->AddTorque(Torque);	// Ball->AddTorque(Torque, FName(TEXT("None")), true); true means that torque apply no attantion Ball mass it rotate itself
	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(Val * GetWorld()->GetDeltaSeconds() * 2000, 0.f, 0.f);
	SetActorLocation(WorldDirection);
}
