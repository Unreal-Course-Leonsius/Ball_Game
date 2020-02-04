// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "LookAround.h"
#include "../Public/Ball_C.h"



// Sets default values for this component's properties
ULookAround::ULookAround()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAround::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Player = Cast<ABall_C>(GetOwner());

	SetupInputComponent();
	
}

void ULookAround::SetupInputComponent()
{
	InputComponentt = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponentt)
	{
		/// Bind Input axis
		InputComponentt->BindAxis("Athimuth", this, &ULookAround::Azimuth); // qualify class name 1
		InputComponentt->BindAxis("Elevation", this, &ULookAround::Elevation);
		UE_LOG(LogTemp, Error, TEXT("%s - has Component InputComponent..."), *GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s - has not Component InputComponent..."), *GetOwner()->GetName())
	}
}


void ULookAround::Azimuth(float Val)
{
	//auto NewRotation = FRotator(0.f, 0.f, Val * GetWorld()->GetDeltaSeconds() * 50);
	//Scene->SetRelativeRotation(NewRotation);

	auto pitch = Val * GetWorld()->GetDeltaSeconds() * 50;
	auto NewRotation = FRotator(0.f, pitch, 0.f);

	if (Player == nullptr) return;

	Player->GetSceneComponent()->AddLocalRotation(NewRotation);


}

void ULookAround::Elevation(float Val)
{
	auto NewRotation = FRotator(Val * GetWorld()->GetDeltaSeconds() * 70, 0.f, 0.f);

	if (Player == nullptr) return;

	Player->GetSpringArmComponent()->AddLocalRotation(NewRotation);
}


// Called every frame
void ULookAround::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

