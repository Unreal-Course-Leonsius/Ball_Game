// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"

#include "Math/Vector.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "Component/BallInputComponent.h"
#include "InfiniteTerrainGameMode.h"

// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Var
	RollTorque = 5000000.0f;
	JumpImpulse = 180000.0f;
	MaxRightForce = 2500.f;
	RotationSpeed = 750;
	MaxForwardForce = 3000.f;

	//BallMotionEX = CreateDefaultSubobject<UParticleSystemComponent>(FName("Effect Component"));

	//bCanJump = true; // Start being able to jump
	//bCanMoveForward = true;


	
}


void ABall_C::Initialize(UStaticMeshComponent * ball, USceneComponent * scene, USpringArmComponent * springarm, UCameraComponent* camera)
{
	/* Get Components */
	Ball = ball;
	Scene = scene;
	SpringArm = springarm;
	Camera = camera;

	RootComponent = Ball;
	/*BallMotionEX->SetupAttachment(RootComponent);

	if (BallMotionEX != nullptr)
	{
		BallMotionEX->SetRelativeLocation(GetActorLocation());
	}*/
	

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
	ForwardForce = MaxForwardForce;

	GameMode = Cast<AInfiniteTerrainGameMode>(GetWorld()->GetAuthGameMode());
	if (!ensure(GameMode != NULL)) return;

	GameMode->IncreaseSpeed.AddUniqueDynamic(this, &ABall_C::IncreaseMaxForwardForce);

	/// Move Value
	//InputForward = 1;

	

	/*UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Ball_C_BeginPlay")));
	UE_LOG(LogTemp, Warning, TEXT("Ball_C_BeginPlay -- Time = %f"), GetWorld()->GetTimeSeconds());*/

}

// AActor interface
void ABall_C::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (!bStartHit) return;

	HitActor = Hit.GetActor();
	auto ArrayTags = HitActor->Tags;

	if (ArrayTags.Num() == 0) return;

	auto TagName = ArrayTags[0];

	if (TagName == "None") return;

	/*UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;*/

	if (TagName == "Ground")
	{
		ForwardForce = MaxForwardForce;
		//UE_LOG(LogTemp, Warning, TEXT("Ground Condition"));

		//Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString::Printf(TEXT("TagName = %s"), *TagName.ToString()));
		if (ImpactEffect == nullptr) return;
		FVector ImpactEffectLocation = GetActorLocation();
		ImpactEffectLocation.Y = ImpactEffectLocation.Y - 50.f;
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, ImpactEffectLocation);


	}
	
	
	HitActor = Hit.GetActor();
	if (HitActor == nullptr) { return; }

	/// Find if it's Tag Actor. if it's Pillar and Jump
	//auto ArrayTags = HitActor->Tags;
	if (TagName == "Pillar")
	{
		//auto TagComponent = Cast<USceneComponent>(HitActor->GetComponentsByTag(USceneComponent::StaticClass(), FName("Pillar"))[0]);
		//if (TagComponent == nullptr) return;
		
		//auto TagName = ArrayTags[0];
		//UE_LOG(LogTemp, Warning, TEXT("TagArray = %s"), *TagName.ToString());
		Jump();
		//Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString::Printf(TEXT("TagName = %s"), *TagName.ToString()));
	}

	auto arrowComponent = HitActor->FindComponentByClass<UArrowComponent>();
	
	/// Hit Obstacles
	if (TagName == "Obstacle")
	{
		GetNotifyHitName(HitActor);
		InputForward = 0;
		Ball->AddImpulse(FVector(0.f, JumpImpulse, 0.f));
		OnDeath.Broadcast();
		//UE_LOG(LogTemp, Warning, TEXT("Death Event Trigger"));

		/*UEngine* Engine = GetGameInstance()->GetEngine();
		if (!ensure(Engine != nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Death Event Trigger")));

		Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString::Printf(TEXT("TagName = %s"), *TagName.ToString()));*/
	}
	
	
}



// Called every frame
void ABall_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeHit += DeltaTime;
	if (TimeHit > 1)
		bStartHit = true;

	SimulateMove(DeltaTime);

}


void ABall_C::SimulateMove(float DeltaTime)
{



	/// 4 - More Simply Motion Code
	const FVector Torque = FVector(0.f, InputForward * RollTorque * DeltaTime, 0.f);
	auto ChangeLocation = GetActorLocation();
	auto Speed = InputForward * ForwardForce * DeltaTime;
	FVector Force = FVector(Speed, 0.f, 0.f);
	ChangeLocation += Force; // Because rotate of Ball, forward vector is not alwayes in suitable scope
	//UE_LOG(LogTemp, Warning, TEXT("Speed = %f, ForwardForce = %f"), Speed, ForwardForce);

	UpdateLocation(ChangeLocation, Torque, DeltaTime);
	UpdateRotation(DeltaTime);

	
}

void ABall_C::UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime)
{
	
	SetActorLocation(worldDirection);
	Ball->AddTorque(torque);


}

void ABall_C::UpdateRotation(float DeltaTime)
{


	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, InputRight * DeltaTime * MaxRightForce, 0.f);

	//UE_LOG(LogTemp, Warning, TEXT("InputRight = %f"), InputRight);


	/// This is Updated forwrad and Right Rotation
	MyControlRotator += FRotator(-InputForward * RotationSpeed * DeltaTime, 0.f, InputRight * RotationSpeed * DeltaTime);

	if (FMath::Abs(MyControlRotator.Pitch) > 360)
		MyControlRotator.Pitch = 0;

	if (FMath::Abs(MyControlRotator.Roll) > 360)
		MyControlRotator.Roll = 0;



	Ball->SetRelativeRotation(MyControlRotator);
	SetActorLocation(WorldDirection);

	/*UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("InputRight = %f"), InputRight));
	UE_LOG(LogTemp, Error, TEXT("============== Input ================="));*/

}



void ABall_C::SetMoveForward(float Val)
{
	InputForward = Val;
}

void ABall_C::SetMoveRight(float Val)
{

	Val = FMath::Clamp<float>(Val, -1.f, 1.f);
	if (Val < 0)
	{
		InputRight = -1;
		return;
	}
		
	if (Val > 0)
	{
		InputRight = 1;
		return;
	}
		
	InputRight = Val;
}



void ABall_C::Jump()
{

	ForwardForce = 450;
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	Ball->AddImpulse(Impulse);
	//bCanJump = false;

	//BallMotionEX->Deactivate();
	UE_LOG(LogTemp, Error, TEXT("MaxForwardForce = %f, ForwardForce = %f"), MaxForwardForce, ForwardForce);

}

void ABall_C::IncreaseMaxForwardForce()
{
	if (MaxForwardForce < 5000)
	{
		MaxForwardForce += ChangeForce;
		UE_LOG(LogTemp, Error, TEXT("IncreaseMaxForwardForce"));
	}
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

void ABall_C::ForceApply()
{

	auto Direction = GetActorRightVector();
	float DragCoefficient = 6000;
	FVector Force = Direction * Ball->GetMass() * DragCoefficient * GetWorld()->GetDeltaSeconds();

	Ball->AddForce(Force);


}


