// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Ball_C.h"

#include "Math/Vector.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "Component/BallInputComponent.h"
#include "InfiniteTerrainGameMode.h"

// Sets default values
ABall_C::ABall_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BallRootComponent"));
	BallRoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = BallRoot;
	BallRoot->bGenerateOverlapEvents = true;
	BallRoot->SetSimulatePhysics(true);
	BallRoot->BodyInstance.AngularDamping = 0.1;
	BallRoot->BodyInstance.LinearDamping = 25.f;

	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	Ball->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Ball->SetupAttachment(RootComponent);
	/*RootComponent = Ball;
	Ball->bGenerateOverlapEvents = true;
	Ball->SetSimulatePhysics(true);
	Ball->BodyInstance.AngularDamping = 0.1;
	Ball->BodyInstance.LinearDamping = 25.f;*/

	JumpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("JumpEffect"));
	JumpEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	JumpEffect->SetupAttachment(RootComponent);
	JumpEffect->bAutoActivate = false;

	JumpSound = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpSound"));
	JumpSound->SetupAttachment(JumpEffect);
	JumpSound->bAutoActivate = false;

	ImpactEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ImpactEffect"));
	ImpactEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ImpactEffect->SetupAttachment(RootComponent);
	ImpactEffect->bAutoActivate = false;

	ImpactSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ImpactSound"));
	ImpactSound->SetupAttachment(ImpactEffect);
	ImpactSound->bAutoActivate = false;


	TraceEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TraceEffect"));
	TraceEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TraceEffect->SetupAttachment(RootComponent);
	TraceEffect->bAutoActivate = false;
	SparksVelocity = "SparksVelocity";

	traceSound = CreateDefaultSubobject<UAudioComponent>(TEXT("traceSound"));
	traceSound->SetupAttachment(TraceEffect);
	traceSound->bAutoActivate = false;

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


void ABall_C::Initialize(USceneComponent * scene, USpringArmComponent * springarm, UCameraComponent* camera)
{
	/* Get Components */
	//Ball = ball;
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
	/*Ball->SetSimulatePhysics(true);
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->BodyInstance.AngularDamping = 0.5;
	Ball->BodyInstance.LinearDamping = 0.5;*/

	//SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 50.f));
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
	InputForward = 1;

	
	UE_LOG(LogTemp, Warning, TEXT("Ball BeginPlay InputForward = %f"), InputForward);
	/*UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Ball_C_BeginPlay")));
	UE_LOG(LogTemp, Warning, TEXT("Ball_C_BeginPlay -- Time = %f"), GetWorld()->GetTimeSeconds());*/

}

// AActor interface
void ABall_C::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	/*if (!bStartHit) return;*/

	bNotifyHit = true;

	HitActor = Hit.GetActor();
	auto ArrayTags = HitActor->Tags;

	if (ArrayTags.Num() == 0) return;

	auto TagName = ArrayTags[0];

	if (TagName == "None") return;


	if (TagName == "Ground")
	{
		ForwardForce = MaxForwardForce;
		//UE_LOG(LogTemp, Warning, TEXT("Ground Condition"));

	}
	
	
	HitActor = Hit.GetActor();
	if (HitActor == nullptr) { return; }

	/// Find if it's Tag Actor. if it's Pillar and Jump
	//auto ArrayTags = HitActor->Tags;
	if (TagName == "Pillar")
	{

		Jump();
	}

	auto arrowComponent = HitActor->FindComponentByClass<UArrowComponent>();
	
	/// Hit Obstacles
	if (TagName == "Obstacle")
	{
		GetNotifyHitName(HitActor);
		InputForward = 0;
		//Ball->AddImpulse(FVector(0.f, JumpImpulse, 0.f));
		ImpactEffect->Activate(true);
		ImpactSound->Play(0.f);
		OnDeath.Broadcast();

	}
	
	
}



// Called every frame
void ABall_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TimeHit += DeltaTime;
	//if (TimeHit > 1)
	//	bStartHit = true;

	SimulateMove(DeltaTime);

}


void ABall_C::SimulateMove(float DeltaTime)
{



	/// 4 - More Simply Motion Code
	const FVector Torque = FVector(0.f, InputForward * RollTorque * DeltaTime, 0.f);
	auto ChangeLocation = GetActorLocation();
	auto Speed = InputForward * ForwardForce * DeltaTime;
	FVector Force = FVector(Speed, 0.f, 0.f);
	ChangeLocation += Force; // Because rotate of Ball, forward vector is not always in suitable scope
	//UE_LOG(LogTemp, Warning, TEXT("Speed = %f, ForwardForce = %f"), Speed, ForwardForce);

	UpdateLocation(ChangeLocation, Torque, DeltaTime);
	UpdateRotation(DeltaTime);

	
}

void ABall_C::UpdateLocation(FVector worldDirection, FVector torque, float DeltaTime)
{
	
	SetActorLocation(worldDirection);
	//Ball->AddTorque(torque);


}

void ABall_C::UpdateRotation(float DeltaTime)
{


	auto WorldDirection = GetActorLocation();
	WorldDirection += FVector(0.f, InputRight * DeltaTime * MaxRightForce, 0.f);

	//UE_LOG(LogTemp, Warning, TEXT("InputRight = %f"), InputRight);


	/// This is Updated forward and Right Rotation
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
	//UE_LOG(LogTemp, Warning, TEXT("Moveing Right -- bNotifyHit = %i,  Val = %f"), bNotifyHit, Val);
	Val = FMath::Clamp<float>(Val, -1.f, 1.f);
	if (Val < 0)
	{
		InputRight = -1;
		if (bNotifyHit)
		{
			TraceEffect->SetVectorParameter(SparksVelocity, -GetVelocity());
			TraceEffect->Activate();
		}
		return;
	}
		
	if (Val > 0)
	{
		InputRight = 1;
		if (bNotifyHit)
		{
			TraceEffect->SetVectorParameter(SparksVelocity, -GetVelocity());
			TraceEffect->Activate();
		}
		return;
	}
	//UE_LOG(LogTemp, Error, TEXT("Moveing Right -- bNotifyHit = %i,  Val = %f"), bNotifyHit, Val);
	InputRight = Val;

	if (Val == 0)
	{
		TraceEffect->SetVectorParameter(SparksVelocity, GetActorLocation());
		TraceEffect->Deactivate();
	}
	
}



void ABall_C::Jump()
{
	bNotifyHit = false;

	ForwardForce = 450;
	const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
	BallRoot->AddImpulse(Impulse);
	//bCanJump = false;
	JumpEffect->Activate();
	JumpSound->Play(0.f);
	//BallMotionEX->Deactivate();
	UE_LOG(LogTemp, Error, TEXT("MaxForwardForce = %f, ForwardForce = %f"), MaxForwardForce, ForwardForce);

}

void ABall_C::IncreaseMaxForwardForce()
{
	if (MaxForwardForce < 4500)
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


