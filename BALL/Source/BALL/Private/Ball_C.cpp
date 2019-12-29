// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"

#include "Math/Vector.h"

// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Vairables
	RollTorque = 500000000.0f;
	JumpImpulse = 450000.0f;
	MoveRightVector = 2000.f;
	MoveForwardVector = 200.f;
	MaxMoveForce = 2500.f;

	bCanJump = true; // Start being able to jump
	bCanMoveForward = true;

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

	bCanMoveForward = true;
	//ChangeLocation = GetActorLocation();

	//UE_LOG(LogTemp, Warning, TEXT("ActorLocation = %s, JumMoveDestination = %s"), *GetActorLocation().ToString(), *JumpMoveDestination.ToString());
}

// AActor interface
void ABall_C::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	
	FString name = OtherComp->GetName();  //HitActor->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("1 NOTIFYHIT - CanMoveForward = %s"), *name);
	//UE_LOG(LogTemp, Error, TEXT("OtherCompName = %s"), *name);
	
	if (OtherComp->GetName() == "Ground")
	{
		bCanJump = true;
		//bCanMoveForward = true;
		MaxMoveForce = 2500;
		//UE_LOG(LogTemp, Warning, TEXT("HitComp = %s, MaxForce = %f"), *Hit.GetActor()->GetName(), MaxMoveForce);
	}
	else
	{
		//MaxMoveForce = 1000;
		//UE_LOG(LogTemp, Warning, TEXT("HitComp = %s, MaxForce = %f"), *Hit.GetActor()->GetName(), MaxMoveForce);

	}

	
	
	HitActor = Hit.GetActor();
	if (HitActor == nullptr) { return; }
	//if (ensure(HitActor == nullptr)) { return; }


	auto arrowComponent = HitActor->FindComponentByClass<UArrowComponent>();

	if (arrowComponent == nullptr)
	{
		GetNotifyHitName(HitActor);
		//OnDeath.Broadcast();
		//Death();
	}
	
	
	//UE_LOG(LogTemp, Warning, TEXT("1 NOTIFYHIT - CanMoveForward = %i"), bCanMoveForward);
	//UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = %s"),*HitActor->GetName());
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

	///// TEST

	//auto velocity = -GetVelocity().Size();
	//FVector Force = FVector(velocity * Ball->GetMass() * DragCoefficient, 0.f, 0.f);

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

	//if (bCanMoveForward)
	//{
		SimulateMove(DeltaTime);
	//}
	//else
	//{
		//UE_LOG(LogTemp, Warning, TEXT("Else Condition"));
		//MoveAfterJump();
	//}

	/*if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = %s"), *HitActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = NONE"));
	}*/
	//MoveForward(1.f);
	//GetNotifyHitName(HitActor);

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

	//if (bCanMoveForward)
	//{
	//	//Ball->AddTorque(Torque);
	//	Ball->SetPhysicsLinearVelocity(Direction, true);
	//	SetActorLocation(WorldDirection);
	//	//SetActorRelativeLocation(WorldDirection);
	//	//AddActorWorldOffset(Translation, true);
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Translation = %s"), *Translation.ToString());

	const FVector Torque = FVector(0.f, InputForward * RollTorque * DeltaTime, 0.f);
	auto ChangeLocation = GetActorLocation();
	FVector Force = FVector(InputForward * MaxMoveForce * DeltaTime, 0.f, 0.f);
	//UE_LOG(LogTemp, Warning, TEXT("ActorLocation_1 = %s"), *GetActorLocation().ToString());


	/*float Force = InputForward * MaxMoveForce;
	float Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;
	float Translation = Velocity * 100 * DeltaTime;

	Velocity = FMath::Clamp<float>(Velocity, -30.f, 30.f);
	Translation = FMath::Clamp<float>(Translation, -25.f, 25.f);*/

	ChangeLocation += Force; // Because rotate of Ball, forward vector is not alwayes in suitable scope
	//UE_LOG(LogTemp, Error, TEXT("ActorLocation_1 = %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Display, TEXT("Force = %s"), *Force.ToString());

		/*UE_LOG(LogTemp, Warning, TEXT("Translation = %f"), Translation);
		UE_LOG(LogTemp, Error, TEXT("Velocity = %f"), Velocity);*/


	/*FRotator NewRotator;
	auto Direction = GetForwardVector_Roll(NewRotator);
	Direction = Direction * InputForward * 1000 * DeltaTime;*/

	if (bCanMoveForward)
	{
		UpdateLocation(ChangeLocation, Torque, DeltaTime);
		UpdateRotation(DeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("MoveForward = %i,  MaxMoveForce = %f"), bCanMoveForward, MaxMoveForce);
	}
	/*else
	{
		UpdateRotation(DeltaTime);
		Ball->AddTorque(Torque);
		UE_LOG(LogTemp, Warning, TEXT("MoveForward = %i"), bCanMoveForward);

	}*/
	
}

void ABall_C::UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime)
{
	
	//MyControlRotator += FRotator(-InputForward * 750 * DeltaTime, 0.f, 0.f);


	//UE_LOG(LogTemp, Warning, TEXT("ControlRotator = %s"), *MyControlRotator.ToString());
	//Ball->SetRelativeRotation(MyControlRotator);


	SetActorLocation(worldDirection);
	Ball->AddTorque(torque);
	//SetActorRelativeLocation(WorldDirection);
	//AddActorWorldOffset(Translation, true);


}

void ABall_C::MoveAfterJump()
{

	//while (FMath::Abs(GetActorLocation().X) <= FMath::Abs(JumpMoveDestination.X))
	//{
		FVector LocalworldDirection = GetActorLocation();
		LocalworldDirection += FVector(InputForward * GetWorld()->GetDeltaSeconds() * MoveForwardVector, 0.f, 0.f);
		SetActorLocation(LocalworldDirection);
		Velocity = 0;
	//}
	
	
	/*UE_LOG(LogTemp, Warning, TEXT("JumoMoveDestination = %s"), *JumpMoveDestination.ToString());
	UE_LOG(LogTemp, Error, TEXT("ActorLocation = %s"), *GetActorLocation().ToString());*/

	//JumpMoveDestination = JumpMoveDestination.ZeroVector;
}


void ABall_C::SetJumpMoveDestination(FVector Destination)
{
	JumpMoveDestination = Destination;
}

void ABall_C::SetMoveForward(bool bValue)
{
	bCanMoveForward = bValue;
	//UE_LOG(LogTemp, Error, TEXT("SetMoveForward Function"));
}

void ABall_C::ForceApply()
{
	auto velocity = -GetVelocity().Size();
	FVector Force = FVector(velocity * Ball->GetMass() * DragCoefficient * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
	//UE_LOG(LogTemp, Warning, TEXT("VelocitySize = %f,  Mass = %f"), velocity, Ball->GetMass());
	//UE_LOG(LogTemp, Error, TEXT("ActorVelocitySize = %f,  ActorLocationSize = %f"), GetVelocity().Size(), GetActorLocation().Size());
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
	InputRight = Val;
}

//void ABall_C::MoveRight(float Val)
//{
//	/// 1 - First Version We need not this
//	//const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
//	//Ball->AddTorque(Torque);
//	  //auto ForceApply = Ball->GetRightVector() * Val * 5000000; // We need not GetRightVector() because when Ball collide something right and forward vector is messed
//
//
//	auto WorldDirection = GetActorLocation();
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
//
//}



void ABall_C::UpdateRotation(float DeltaTime)
{
	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, InputRight * DeltaTime * MoveRightVector, 0.f);

	MyControlRotator += FRotator(-InputForward * 750 * DeltaTime, 0.f, InputRight * 750 * DeltaTime);

	if (FMath::Abs(MyControlRotator.Pitch) > 360)
		MyControlRotator.Pitch = 0;

	if (FMath::Abs(MyControlRotator.Roll) > 360)
		MyControlRotator.Roll = 0;

	//FRotator ControlRotator = GetControlRotation();
	//FRotator NewRotator;
	//NewRotator = FRotator(0.f, 0.f, ControlRotator.Yaw);
	////UE_LOG(LogTemp, Warning, TEXT("ControlRotator = %s"), *ControlRotator.ToString());
	//auto Direction = GetRightVector_Pitch(NewRotator);
	//Direction = Direction * InputRight * 10;

	//UE_LOG(LogTemp, Error, TEXT("Direction = %s"), *Direction.ToString());

	
	//Ball->SetPhysicsAngularVelocity(Direction);
	//Ball->Setre

	Ball->SetRelativeRotation(MyControlRotator);
	SetActorLocation(WorldDirection);
	//Ball->SetPhysicsLinearVelocity(Direction, true);
	//Ball->SetPhysicsAngularVelocity(Direction, true);
}

void ABall_C::Jump()
{
	//UE_LOG(LogTemp, Warning, TEXT("1 JUMP - CanMoveForward = %i"), bCanMoveForward);
	//if (bCanJump)
	//{
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	Ball->AddImpulse(Impulse);
	bCanJump = false;
	//bCanMoveForward = false;
	Velocity = 0;
	MaxMoveForce = 500;
	UE_LOG(LogTemp, Error, TEXT("JumpImpulse = %f"), JumpImpulse);
	//SetActorLocation(GetActorLocation());
	//GetAirResistance();
		//ForceApply();
	//}

	//UE_LOG(LogTemp, Warning, TEXT("JUMP - CanMoveForward = %i,  MaxForce = %f"), bCanMoveForward, MaxMoveForce);
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

	//auto HA = HitActorr.GetActor();
	//auto arrowComponent = HitActorr->FindComponentByClass<UArrowComponent>();
	
	HitActorName = HitActorr->GetName();
	UE_LOG(LogTemp, Error, TEXT("BALL HIT = %s"), *HitActorName);
}

/*FName ABall_C::GetNotifyHitName(FHitResult & HitActor)
{
	auto HA = HitActor.GetActor();
	if (HA == nullptr)
	{
		return FName("None");
	}
	else
	{
		return FName(*HA->GetName());
	}
	
}*/


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

/*void ABall_C::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}*/