// Fill out your copyright notice in the Description page of Project Settings.

#include "BALL.h"
#include "Components/ArrowComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Tile.h"

#define OUT


// Sets default values
ATile::ATile(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//UE_LOG(LogTemp, Warning, TEXT("Tile Construct C++"));

	//FTransform Transform(FVector(3000, 0, -50));

	NextTilePoint = FVector(0, 0, 0);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	ArrowComponent = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("Arrow"));
	//ArrowComponent = CreateDefaultSubobject<UArrowComponent>(this, TEXT("Arrow")); // CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));  // For ArrowComponent don't work DefaultSubobject<>() function
	//ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow")); // the function work only Editor and therefor have problem when Game Packaging
	ArrowComponent->SetupAttachment(Root);
	ArrowComponent->SetRelativeLocation(ArrowLocation);  // Set location relative to the world
	// ArrowComponent->AddRelativeLocation(FVector(3000, 0, -50));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Tile BeginPlay C++"));

	
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//DestroySpawnActors();

	GetWorld()->GetTimerManager().ClearTimer(Timer);

	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void ATile::SpawnActorsRandomLocation(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, TArray<FVector> SpawnLocations)
{

	TArray<bool> IsTheLocationFree;
	FillBoolArray(IsTheLocationFree, SpawnLocations.Num());

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < NumberToSpawn; i++)
	{
		// Set Index is not selected before
		int32 SpawnLocationIndex;
		SpawnLocationIndex = SetUniqueIndext(SpawnLocations.Num()-1, IsTheLocationFree);

		// Check mark the selected Indesx
		IsTheLocationFree[SpawnLocationIndex] = true;

		GenerateSpawnActor_RandomLocation(ToSpawn, SpawnLocations[SpawnLocationIndex]);

		//GarbageObject.Add(Spawn);
	}

	//UE_LOG(LogTemp, Warning, TEXT("================= Spawn Actor Random Location ====================="));

	return;
}


int32 ATile::SetUniqueIndext(int32 MaxRange, TArray<bool> &IsTheLocationFree)
{
	int32 spawnlocalindex = 0; // SpawnLocationIndex
	bool InLoop = true;
	while(InLoop)
	{
		spawnlocalindex = FMath::RandRange(0, MaxRange);
		if (!IsTheLocationFree[spawnlocalindex])
		{
			InLoop = false;
		}

		UE_LOG(LogTemp, Warning, TEXT("spawnlocalindex ==== While = %i"), spawnlocalindex);
	}

	UE_LOG(LogTemp, Warning, TEXT("spawnlocalindex ==== Out = %i"), spawnlocalindex);

	return spawnlocalindex;
}

void ATile::FillBoolArray(TArray<bool> &IsTheLocationFree, int32 length)
{
	for (int i = 0; i < length; i++)
	{
		IsTheLocationFree.Add(false);
	}
}


void ATile::GenerateSpawnActor_RandomLocation(TSubclassOf<AActor> ToSpawn, FVector SpawnLocation)
{
	AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(ToSpawn);

	if (!ensure(SpawnActor)) { return; }
	GarbageObject.Add(SpawnActor);

	SpawnActor->SetActorRelativeLocation(SpawnLocation);
	SpawnActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	return;

}

void ATile::DestroySpawnActors()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Spawn Actors === Garbage = %i"), GarbageObject.Num());
	while (GarbageObject.Num() != 0)
	{
		AActor* Prop = nullptr;
		Prop = GarbageObject.Pop();
		Prop->Destroy();
	}
}

void ATile::SpawnActorConcretLocation(TSubclassOf<AActor> ToSpawn, int32 MaxSpawn, TArray<FVector> SpawnLocations)
{
	maxSpawnNumber = MaxSpawn;
	indexLocation = 0;

	//UE_LOG(LogTemp, Warning, TEXT("SpawnActorConcretLocatoin ======================"));
	TimerDel.BindUFunction(this, FName("GenerateSpawnActor_ConcretLocation"), ToSpawn, SpawnLocations);
	//AActor* Spawn = GenerateSpawnActor(ToSpawn, SpawnLocations[i]);
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDel, DelationTime, true);
	//GarbageObject.Add(Spawn);

	return;
}

void ATile::GenerateSpawnActor_ConcretLocation(TSubclassOf<AActor> ToSpawn, TArray<FVector> SpawnLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("==================== GenerateSpawnActor_ConcretLocatoin ======================"));

	if (indexLocation < maxSpawnNumber)
	{
		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		if (!ensure(SpawnActor)) { return; }
		GarbageObject.Add(SpawnActor);

		//UE_LOG(LogTemp, Warning, TEXT("SpawnActorConcretLocatoin if Statment ====================== %i"), indexLocation);
		SpawnActor->SetActorRelativeLocation(SpawnLocation[indexLocation]);
		SpawnActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		indexLocation++;
	}

	return;
}


/// This function is used for Tile only with TimeLine
void ATile::SpawnActors_TimeLineObjects(TSubclassOf<AActor> ToSpawn, TArray<AActor*> &SpawnActors, TArray<FVector> SpawnLocations)
{

	for (int32 index = 0; index < SpawnLocations.Num(); index++)
	{
		auto SpawnActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		if (!ensure(SpawnActor)) return;

		SpawnActors.Add(SpawnActor);
		SpawnActor->SetActorRelativeLocation(SpawnLocations[index]);
		SpawnActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	}

}


// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Delta Time = %f"), GetWorld()->GetTimeSeconds());

	/*for (int i = 0; i < 5; i++)
	{
		float delayTime = GetWorld()->GetTimeSeconds();
		if (delayTime > 2.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Delta Time = %f"), GetWorld()->GetTimeSeconds());
		}
	}*/

}

