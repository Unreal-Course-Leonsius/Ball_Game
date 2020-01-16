// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"

#include "Math/Vector.h"
#include "Engine/Engine.h"

// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Vairables
	RollTorque = 500000000.0f;
	JumpImpulse = 450000.0f;
	MoveRightVector = 2500.f;
	RotationSpeed = 750;
	MaxMoveForce = 3000.f;

	/// Move Value
	InputForward = 1;

	//bCanJump = true; // Start being able to jump
	//bCanMoveForward = true;

	JumpMoveDestination = FVector(0, 0, 0);
	
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
	//SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = false; // must not be bAbsolutRotation = true because Scene Component work
	//RootComponent = Ball;
	Ball->SetSimulatePhysics(true);
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->BodyInstance.AngularDamping = 0.5;
	Ball->BodyInstance.LinearDamping = 0.5;

	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 50.f));
}

// Called when the game starts or when spawned
void ABall_C::BeginPlay()
{
	Super::BeginPlay();

	MyControlRotator = GetActorRotation();

}

// AActor interface
void ABall_C::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


	if (OtherComp->GetName() == "Ground")
	{
		ForwardForce = MaxMoveForce;
	}
	else
	{
		//MaxMoveForce = MaxMoveForce / 6;
	}
	
	
	HitActor = Hit.GetActor();
	if (HitActor == nullptr) { return; }

	/// Find if it's Tag Actor
	auto ArrayTags = HitActor->Tags;
	if (ArrayTags.Num() > 0)
	{
		//auto TagComponent = Cast<USceneComponent>(HitActor->GetComponentsByTag(USceneComponent::StaticClass(), FName("Pillar"))[0]);
		//if (TagComponent == nullptr) return;
		
		auto TagName = ArrayTags[0];
		UE_LOG(LogTemp, Warning, TEXT("TagArray = %s"), *TagName.ToString());
		Jump();
	}

	auto arrowComponent = HitActor->FindComponentByClass<UArrowComponent>();
	
	/// Hit Obstacles
	if (arrowComponent == nullptr && ArrayTags.Num() <= 0)
	{
		GetNotifyHitName(HitActor);
		//OnDeath.Broadcast();
		//Death();
		
	}
	
	
}


// Called to bind functionality to input
void ABall_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mouse Movement
	/*PlayerInputComponent->BindAxis("Athimuth", this, &ABall_C::Azimuth);
	PlayerInputComponent->BindAxis("Elevation", this, &ABall_C::Elevation);*/

	// Forwad Backward
	//PlayerInputComponent->BindAxis("WForward_Backward", this, &ABall_C::MoveForward);
	PlayerInputComponent->BindAxis("ALeft_DRight", this, &ABall_C::MoveRight);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABall_C::Jump);
}

// Called every frame
void ABall_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("UPvector = %s"), *GetActorUpVector().ToString());

	//DrawDebugLine
	//(
	//	GetWorld(),
	//	GetActorLocation() + FVector(0,0,50.f),
	//	GetActorLocation() + (GetActorForwardVector() * 1000),
	//	FColor(255, 0, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	5.f
	//);

	//DrawDebugLine
	//(
	//	GetWorld(),
	//	GetActorLocation() + FVector(0, 0, 50.f),
	//	GetVelocity(),
	//	FColor(0, 0, 255),
	//	false,
	//	0.f,
	//	0.f,
	//	5.f
	//);



	//DrawDebugLine
	//(
	//	GetWorld(),
	//	Force + FVector(0, 0, 50.f), //GetActorLocation() + FVector(100.f, 0, 50.f),
	//	GetActorLocation() - FVector(100.f, 0, 0),
	//	FColor(0, 255, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	5.f
	//);

	DrawDebugLine
	(
		GetWorld(),
		GetActorLocation() + FVector(0, 0, 50.f),
		GetActorLocation() + GetActorRotation().Vector() * 1000,
		FColor(0, 255, 0),
		false,
		0.f,
		0.f,
		5.f
	);


	SimulateMove(DeltaTime);

	

}


void ABall_C::SimulateMove(float DeltaTime)
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
	const FVector Torque = FVector(0.f, InputForward * RollTorque * DeltaTime, 0.f);
	auto ChangeLocation = GetActorLocation();
	auto Speed = InputForward * ForwardForce * DeltaTime;
	FVector Force = FVector(Speed, 0.f, 0.f);
	ChangeLocation += Force; // Because rotate of Ball, forward vector is not alwayes in suitable scope
	UE_LOG(LogTemp, Warning, TEXT("Speed = %f, ForwardForce = %f"), Speed, ForwardForce);

	UpdateLocation(ChangeLocation, Torque, DeltaTime);
	UpdateRotation(DeltaTime);

	
}

void ABall_C::UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime)
{
	
	SetActorLocation(worldDirection);
	Ball->AddTorque(torque);

	//AddActorWorldOffset(Translation, true);

}


void ABall_C::ForceApply()
{
	auto velocity = -GetVelocity().Size();
	float DragCoefficient = 60;
	FVector Force = FVector(velocity * Ball->GetMass() * DragCoefficient * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);

	Ball->AddForce(Force);

	//DrawDebugLine
	//(
	//	GetWorld(),
	//	Force, //GetActorLocation() + FVector(100.f, 0, 50.f),
	//	GetActorLocation(),
	//	FColor(0, 255, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	5.f
	//);
}



void ABall_C::MoveForward(float Val)
{
	InputForward = Val;
}

void ABall_C::MoveRight(float Val)
{

	Val = FMath::Clamp<float>(Val, -1.f, 1.f);
	/*if (Val < 0)
	{
		InputRight = -1;
		return;
	}
		
	if (Val > 0)
	{
		InputRight = 1;
		return;
	}*/
		
	InputRight = Val;
}


void ABall_C::UpdateRotation(float DeltaTime)
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

	
	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, InputRight * DeltaTime * MoveRightVector, 0.f);


	/// This is Updated Forwrad and Right Rotation
	MyControlRotator += FRotator(-InputForward * RotationSpeed * DeltaTime, 0.f, InputRight * RotationSpeed * DeltaTime);

	if (FMath::Abs(MyControlRotator.Pitch) > 360)
		MyControlRotator.Pitch = 0;

	if (FMath::Abs(MyControlRotator.Roll) > 360)
		MyControlRotator.Roll = 0;



	Ball->SetRelativeRotation(MyControlRotator);
	SetActorLocation(WorldDirection);
	
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("InputRight = %f"), InputRight));
	UE_LOG(LogTemp, Error, TEXT("============== Input ================="));
	
}

void ABall_C::Jump()
{
	
	ForwardForce = 450;
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	Ball->AddImpulse(Impulse);
	//bCanJump = false;

	UE_LOG(LogTemp, Error, TEXT("JumpImpulse = %f, ForwardForce = %f"), JumpImpulse, ForwardForce);

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
	auto NewRotation = FRotator(Val * GetWorld()->GetDeltaSeconds() * 70, 0.f, 0.f);
	SpringArm->AddLocalRotation(NewRotation);
}


/// Rotator Transform Vectors
FVector ABall_C::GetForwardVector_Roll(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::X);
}

FVector ABall_C::GetUpVector_Yaw(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Z);
}

FVector ABall_C::GetRightVector_Pitch(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Y);
}



void ABall_C::GetNotifyHitName(AActor* HitActorr)
{

	HitActorName = HitActorr->GetName();
	UE_LOG(LogTemp, Error, TEXT("BALL HIT = %s"), *HitActorName);
}

/// Remove Functions
//void ABall_C::ForceApply()
//{
//
//	auto BallVelocity = GetVelocity();
//	auto DeltaTime = GetWorld()->GetDeltaSeconds();
//
//	/// Get Vector which is Permentikular of Velocity and Right Vector's
//	auto ProductUpVector = FVector::CrossProduct(BallVelocity, Ball->GetRightVector());
//
//	/// Create Acceleration and it's direction (Vector)
//	auto Acceleration = -((BallVelocity.X / DeltaTime) * ProductUpVector.GetSafeNormal());
//
//	auto BallMass = Ball->GetMass();
//	Ball->AddForce(Acceleration * BallMass);
//
//	/*UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ DeltaTime = %f"), DeltaTime);
//	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ BallVelocity = %s"), *BallVelocity.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ ProductUpVector = %s"), *ProductUpVector.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ Acceleration = %s"), *Acceleration.ToString());*/
//
//}

///
//void ABall_C::MoveAfterJump()
//{
//
//	//while (FMath::Abs(GetActorLocation().X) <= FMath::Abs(JumpMoveDestination.X))
//	//{
//		FVector LocalworldDirection = GetActorLocation();
//		LocalworldDirection += FVector(InputForward * GetWorld()->GetDeltaSeconds() * MoveForwardVector, 0.f, 0.f);
//		SetActorLocation(LocalworldDirection);
//		Velocity = 0;
//	//}
//	
//	
//	/*UE_LOG(LogTemp, Warning, TEXT("JumoMoveDestination = %s"), *JumpMoveDestination.ToString());
//	UE_LOG(LogTemp, Error, TEXT("ActorLocation = %s"), *GetActorLocation().ToString());*/
//
//	//JumpMoveDestination = JumpMoveDestination.ZeroVector;
//}


//void ABall_C::SetJumpMoveDestination(FVector Destination)
//{
//	JumpMoveDestination = Destination;
//}

///
//void ABall_C::SetMoveForward(bool bValue)
//{
//	bCanMoveForward = bValue;
//	//UE_LOG(LogTemp, Error, TEXT("SetMoveForward Function"));
//}

/*void ABall_C::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}*/