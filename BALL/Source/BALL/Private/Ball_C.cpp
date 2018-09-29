// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"


// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Vairables
	RollTorque = 500000000.0f;
	JumpImpulse = 350000.0f;
	MoveRihgt_Left = 20.f;
	MoveForward_Backward = 200.f;

	bCanJump = true; // Start being able to jump
	bCanMoveForward = true;
	
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
}

// Called when the game starts or when spawned
void ABall_C::BeginPlay()
{
	Super::BeginPlay();

}

// AActor interface
void ABall_C::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	HitActor = Hit.GetActor();
	FString name = OtherComp->GetName();  //HitActor->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("1 NOTIFYHIT - CanMoveForward = %s"), *name);

	if (OtherComp->GetName() == "Ground")
	{
		bCanJump = true;
		bCanMoveForward = true;
		//UE_LOG(LogTemp, Warning, TEXT("otherComp = %s"), *OtherComp->GetName());
	}

	//UE_LOG(LogTemp, Warning, TEXT("1 NOTIFYHIT - CanMoveForward = %i"), bCanMoveForward);
	//UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = %s"),*HitActor->GetName());
}

/*void ABall_C::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}*/


// Called every frame
void ABall_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = %s"), *HitActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor's Name = NONE"));
	}*/
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
	/// 1 - First Version We need not this
	//const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	//Ball->AddTorque(Torque);
	  //auto ForceApply = Ball->GetRightVector() * Val * 5000000; // We need not GetRightVector() because when Ball collide something right and forward vector is messed

	
	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, Val * GetWorld()->GetDeltaSeconds() * MoveRihgt_Left, 0.f);

	FRotator ControlRotator = GetControlRotation();
	FRotator NewRotator = FRotator(0.f, 0.f, ControlRotator.Yaw);
	auto Direction = GetMyRightVector(NewRotator);
	Direction = Direction * Val * 10;

	Ball->SetPhysicsLinearVelocity(Direction, true);
	SetActorLocation(WorldDirection);
	//Ball->SetPhysicsAngularVelocity(Direction, true);

}

void ABall_C::MoveForward(float Val)
{
	/// 1 - First Version We need not it
	//const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	//Ball->AddTorque(Torque);	// Ball->AddTorque(Torque, FName(TEXT("None")), true); true means that torque apply no attantion Ball mass it rotate itself

	/// 2 - Second Version We need not it
	/*auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(Val * GetWorld()->GetDeltaSeconds() * MoveForward_Backward, 0.f, 0.f);
	if(bCanActorLocation)
	{
		//SetActorLocation(WorldDirection);
	}
	*/

	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(Val * GetWorld()->GetDeltaSeconds() * MoveForward_Backward, 0.f, 0.f);

	FRotator ControlRotator = GetControlRotation();
	FRotator NewRotator = FRotator(0.f, 0.f, ControlRotator.Yaw);
	auto Direction = GetForwardVector(NewRotator);
	Direction = Direction * Val * 15;

	if (bCanMoveForward)
	{
		//auto Primitive = GetRootPrimitiveComponent();
		Ball->SetPhysicsLinearVelocity(Direction, true);
		SetActorLocation(WorldDirection);
	}
	

	
}


void ABall_C::Jump()
{
	//UE_LOG(LogTemp, Warning, TEXT("1 JUMP - CanMoveForward = %i"), bCanMoveForward);
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
		bCanMoveForward = false;
		//ForceApply();
	}

	UE_LOG(LogTemp, Warning, TEXT("2 JUMP - CanMoveForward = %i"), bCanMoveForward);
}

/// Rotator Transform Vectors
FVector ABall_C::GetForwardVector(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::X);
}

FVector ABall_C::GetMyUpVector(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Z);
}

FVector ABall_C::GetMyRightVector(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Y);
}


void ABall_C::ForceApply()
{

	auto BallVelocity = GetVelocity();
	auto DeltaTime = GetWorld()->GetDeltaSeconds();

	/// Get Vector which is Permentikular of Velocity and Right Vector's
	auto ProductUpVector = FVector::CrossProduct(BallVelocity, Ball->GetRightVector());
	
	/// Create Acceleration and it's direction (Vector)
	auto Acceleration = -((BallVelocity.X / DeltaTime) * ProductUpVector.GetSafeNormal());

	auto BallMass = Ball->GetMass();
	Ball->AddForce(Acceleration * BallMass);

	/*UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ DeltaTime = %f"), DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ BallVelocity = %s"), *BallVelocity.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ ProductUpVector = %s"), *ProductUpVector.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ForceApply ------ Acceleration = %s"), *Acceleration.ToString());*/
	
}

void ABall_C::GetNotifyHitName(FHitResult & HitActorr)
{
	auto HA = HitActorr.GetActor();
	if (HA == nullptr)
	{
		HitActorName = "None";
	}
	else
	{
		HitActorName = HA->GetName();
	}
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
