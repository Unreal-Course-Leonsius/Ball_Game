// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "BallMovementComponent.h"

#include "../Public/Ball_C.h"



// Sets default values for this component's properties
UBallMovementComponent::UBallMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	/*RollTorque = 500000000.0f;
	JumpImpulse = 450000.0f;
	MaxRightForce = 2500.f;
	RotationSpeed = 750;
	MaxForwardForce = 3000.f;*/

	InputForward = 1;
}



// Called when the game starts
void UBallMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//BallPlayer = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	MyControlRotator = GetOwner()->GetActorRotation();

	UEngine* Engine = GetOwner()->GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 20, FColor::Green, FString::Printf(TEXT("MovementComponentBeginPlay")));
	UE_LOG(LogTemp, Warning, TEXT("MovementComponentBeginPlay -- Time = %f"), GetWorld()->GetTimeSeconds());
}


// Called every frame
void UBallMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//if (!ensure(Player != nullptr)) return;

	if (!ensure(BallPlayer != nullptr)) return;
	SimulateMove(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TickComponent"));
}

void UBallMovementComponent::SimulateMove(float DeltaTime)
{
	/// 1 - First Version We need not it
	//const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	//Ball->AddTorque(Torque);	// Ball->AddTorque(Torque, FName(TEXT("None")), true); true means that torque apply no attantion Ball mass it rotate itself

	/// 2 - Second Version We need not it
	/*auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(Val * GetWorld()->GetDeltaSeconds() * MoveForward_Backward, 0.f, 0.f);
	const FVector Torque = FVector(0.f, InputForward * RollTorque * DeltaTime, 0.f);
	if(bCanActorLocation)
	{
		SetActorLocation(WorldDirection);
			AddActorWorldOffset(Translation, true);
		Ball->AddTorque(Torque);
			Ball->SetPhysicsLinearVelocity(Direction, true);
	}
	*/


	/// 3 - Calculate Acceleration -- No more need it 
	/*float Force = InputForward * MaxMoveForce;
	float Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;
	float Translation = Velocity * 100 * DeltaTime;

	Velocity = FMath::Clamp<float>(Velocity, -30.f, 30.f);
	Translation = FMath::Clamp<float>(Translation, -25.f, 25.f);*/

	/// Calculate Rotation Direction
	/*FRotator NewRotator;
	auto Direction = GetForwardVector_Roll(NewRotator);
	Direction = Direction * InputForward * 1000 * DeltaTime;*/


	/// 4 - More Simply Motion Code
	const FVector Torque = FVector(0.f, InputForward * Movement_RollTorque * DeltaTime, 0.f);
	auto ChangeLocation = GetOwner()->GetActorLocation();
	auto Speed = InputForward * ForwardForce * DeltaTime;
	FVector Force = FVector(Speed, 0.f, 0.f);
	ChangeLocation += Force; // Because rotate of Ball, forward vector is not alwayes in suitable scope
	//UE_LOG(LogTemp, Warning, TEXT("Speed = %f, ForwardForce = %f"), Speed, ForwardForce);

	UpdateLocation(ChangeLocation, Torque, DeltaTime);
	UpdateRotation(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("SimulateMove"));


}

void UBallMovementComponent::UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime)
{

	GetOwner()->SetActorLocation(worldDirection);
	BallPlayer->AddTorque(torque);

	//AddActorWorldOffset(Translation, true);

	//UE_LOG(LogTemp, Warning, TEXT("UpdateLocation"));

}

void UBallMovementComponent::UpdateRotation(float DeltaTime)
{

	/// 1 - First Version We need not this
	//const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	//Ball->AddTorque(Torque);
	//auto ForceApply = Ball->GetRightVector() * Val * 5000000; // We need not GetRightVector() because when Ball collide something right and forward vector is messed

	/// 2 - Second Code Version
	//auto WorldDirection = GetActorLocation();
	//	UE_LOG(LogTemp, Warning, TEXT("ControlRotation = %s"), *GetControlRotation().ToString());
	//	WorldDirection += FVector(0.f, Val * GetWorld()->GetDeltaSeconds() * MoveRightVector, 0.f);
	//
	//	FRotator ControlRotator = GetControlRotation();
	//	FRotator NewRotator = FRotator(0.f, 0.f, ControlRotator.Yaw);
	//	auto Direction = GetRightVector_Pitch(NewRotator);
	//	Direction = Direction * Val * 10;
	//
	//	Ball->SetPhysicsLinearVelocity(Direction, true);
	////	//Ball->SetWorldRotation(NewRotator * Val * 100000);
	//	SetActorLocation(WorldDirection);
	//	//Ball->SetPhysicsAngularVelocity(Direction, true);


	auto WorldDirection = GetOwner()->GetActorLocation();
	WorldDirection += FVector(0.f, InputRight * DeltaTime * Movement_MaxRightForce, 0.f);

	//UE_LOG(LogTemp, Warning, TEXT("InputRight = %f"), InputRight);


	/// This is Updated Forwrad and Right Rotation
	MyControlRotator += FRotator(-InputForward * Movement_RotationSpeed * DeltaTime, 0.f, InputRight * Movement_RotationSpeed * DeltaTime);

	if (FMath::Abs(MyControlRotator.Pitch) > 360)
		MyControlRotator.Pitch = 0;

	if (FMath::Abs(MyControlRotator.Roll) > 360)
		MyControlRotator.Roll = 0;



	BallPlayer->SetRelativeRotation(MyControlRotator);
	GetOwner()->SetActorLocation(WorldDirection);

	/*UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("InputRight = %f"), InputRight));
	UE_LOG(LogTemp, Error, TEXT("============== Input ================="));*/

	//UE_LOG(LogTemp, Warning, TEXT("UpdateRotation"));

}



void UBallMovementComponent::Jump()
{

	ForwardForce = MinForwardForce;
	const FVector Impulse = FVector(0.f, 0.f, Movement_JumpImpulse);
	BallPlayer->AddImpulse(Impulse);
	//bCanJump = false;

	UE_LOG(LogTemp, Error, TEXT("JumpImpulse = %f, ForwardForce = %f"), Movement_JumpImpulse, ForwardForce);

}

void UBallMovementComponent::ImpactBallPlayer(FVector Impulse)
{
	BallPlayer->AddImpulse(Impulse);
}

void UBallMovementComponent::SetMovementProperty(float rolltorque, float jumpimpulse, float maxrightforce, float rotationspeed, float maxforwardforce)
{
	Movement_MaxRightForce = maxrightforce;
	Movement_RotationSpeed = rotationspeed;
	Movement_RollTorque = rolltorque;
	Movement_JumpImpulse = jumpimpulse;
	Movement_MaxForwardForce = maxforwardforce;
}

