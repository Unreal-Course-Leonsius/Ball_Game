// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Components/ArrowComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("Tile Construct C++"));

	//FTransform Transform(FVector(3000, 0, -50));

	NextTilePoint = FVector(0, 0, 0);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	//ArrowComponent = CreateDefaultSubobject<UArrowComponent>(this, TEXT("Arrow")); // CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));  // For ArrowComponent don't work DefaultSubobject<>() function
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(Root);
	ArrowComponent->SetRelativeLocation(FVector(3000, 0, 0));
	// ArrowComponent->AddRelativeLocation(FVector(3000, 0, -50));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Tile BeginPlay C++"));
	
}


// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

